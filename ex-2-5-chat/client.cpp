#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <stdio.h>
#include <arpa/inet.h>

#define MAX_EVENTS 32

int set_nonblock (int fd){
int flags;
#if defined (O_NONBLOCK)
	if(-1==(flags=fcntl(fd, F_GETFL, 0))) {
		flags = 0;
	}
	return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
	flags = 1;
	return ioctl(fd, FIOBIO, &flags);
#endif
}

int main (int argc, char** argv) {

	if(argc != 3) {
		std::cout << "error! invalid arguments" << std::endl;
		return -1;
	}

	std::cout << "I'am chat client" << std::endl;
	std::cout << "server ip: " << argv[1] << " server port: " << argv[2] << std::endl;
	int masterSocket = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in sockAddr;

	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(12345);
	inet_pton(AF_INET, "127.0.0.1", &(sockAddr.sin_addr));

	std::cout << "trying to connect..." << std::endl;
	connect(masterSocket, (sockaddr*) (&sockAddr), sizeof(sockAddr));
	std::cout << "connected" << std::endl;

	std::string message;
	while(true) {
		std::cin >> message;
		message = message + '\r' + '\n';
		send(masterSocket, message.c_str(), message.size(), MSG_NOSIGNAL);
	}
/////////////////////////////////DEBUG
	return 0;
	listen(masterSocket, SOMAXCONN);

	int epoll = epoll_create1(0);
	epoll_event event;
	event.data.fd = masterSocket;
	event.events = EPOLLIN;
	epoll_ctl(epoll, EPOLL_CTL_ADD, masterSocket, &event);

	while(true) {
		epoll_event events[MAX_EVENTS];
		int N = epoll_wait(epoll, events, MAX_EVENTS, -1);
		for(int i = 0; i != N; i++) {
			if(events[i].data.fd == masterSocket){
				int slaveSocket = accept(masterSocket, 0, 0);
				set_nonblock(slaveSocket);
				std::cout << "accept!" << std::endl;

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
				} else {
					printf("%s", buffer);
				}
			}
		}
	}

}
