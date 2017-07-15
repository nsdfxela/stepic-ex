#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <thread>

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

char inMessage [1024];
int masterSocket;

void doProcessInMsg () {

	while(true) {
	int recvResult = recv(masterSocket, inMessage, 1024, MSG_NOSIGNAL);
		if(recvResult == 0 && errno != EAGAIN) {
			shutdown(masterSocket, SHUT_RDWR);
			close(masterSocket);
			break;
		} else if(recvResult > 0) {
			printf("message : %s \n", inMessage);					
		}
	}
}

std::string getAddr(int socket) {
	sockaddr_in saddr;
	socklen_t saddrLen = sizeof(saddr);
	getsockname(socket, (sockaddr*)(&saddr), &saddrLen);
	char buffer [1024];
	inet_ntop(AF_INET, &(saddr.sin_addr), buffer, 1024);
	return std::string(buffer);
}

int main (int argc, char** argv) {

	if(argc != 3) {
		std::cout << "error! invalid arguments" << std::endl;
		return -1;
	}

	std::cout << "I'am chat client" << std::endl;
	std::cout << "server ip: " << argv[1] << " server port: " << argv[2] << std::endl;
	masterSocket = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in sockAddr;

	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(atoi(argv[2]));
	inet_pton(AF_INET, argv[1], &(sockAddr.sin_addr));

	std::cout << "trying to connect..." << std::endl;
	connect(masterSocket, (sockaddr*) (&sockAddr), sizeof(sockAddr));
	std::cout << "connected" << std::endl;
	std::string ipAndPort = getAddr(masterSocket);
	std::cout << "my ip is : " << ipAndPort << std::endl;	
	std::thread inMsgProcessing (doProcessInMsg);

	std::string outMessage;
	
	while(true) {
		std::cin >> outMessage;
		outMessage = ipAndPort + ": " + outMessage + '\r' + '\n';
		send(masterSocket, outMessage.c_str(), outMessage.size(), MSG_NOSIGNAL);
	}
	return 0;
}

