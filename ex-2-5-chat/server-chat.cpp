#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <vector>

#define MAX_EVENTS 32


int set_nonblock(int fd)
{
int flags;
#if defined(O_NONBLOCK)
 	if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
		flags = 0;
	return  fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
	﻿flags = 1;
	﻿return ioctl(fd, FIOBIO, &flags);
#endif
}

int main (int argc, char** argv) {

	std::cout << "I'am chat server" << std::endl;

	int masterSocket = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in sockAddr;

	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(12345);
	sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(masterSocket, (sockaddr*) (&sockAddr), sizeof(sockAddr));
	set_nonblock(masterSocket);
	listen(masterSocket, SOMAXCONN);

	int epoll = epoll_create1(0);
	epoll_event event;
	event.data.fd = masterSocket;
	event.events = EPOLLIN;
	epoll_ctl(epoll, EPOLL_CTL_ADD, masterSocket, &event);
	std::vector<int> acceptedSockets;
	while(true) {
		epoll_event events[MAX_EVENTS];
		int N = epoll_wait(epoll, events, MAX_EVENTS, -1);
		for(int i = 0; i != N; i++) {
			if(events[i].data.fd == masterSocket){
				int slaveSocket = accept(masterSocket, 0, 0);
				set_nonblock(slaveSocket);
				std::cout << "accept: " << slaveSocket << std::endl;
				acceptedSockets.push_back(slaveSocket);
				epoll_event event;
				event.data.fd = slaveSocket;
				event.events = EPOLLIN;
				epoll_ctl(epoll, EPOLL_CTL_ADD, slaveSocket, &event);

			} else {
				char buffer[1024];
				int recvResult = recv(events[i].data.fd, buffer, 1024, MSG_NOSIGNAL);
				if(recvResult == 0 && errno != EAGAIN) {
					shutdown(events[i].data.fd, SHUT_RDWR);
					close(events[i].data.fd);
				} else if(recvResult > 0) {					
					printf("Sockets known: %d \n", (int)acceptedSockets.size());
					//retranslate message to other clients
					for(int j = 0; j != acceptedSockets.size(); j++) {
						if(acceptedSockets[j] != events[i].data.fd) {						
							std::cout << "sending to " << acceptedSockets[i] << std::endl;
							send(acceptedSockets[j], buffer, recvResult, MSG_NOSIGNAL);
						}
						
					}
				}
			}
		}
	}

}
