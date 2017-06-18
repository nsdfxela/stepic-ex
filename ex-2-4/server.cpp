#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main (int argc, char **argv) {
	std::cout << "This is server" << std::endl;	
	int masterSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		
	sockaddr_in Sockaddr;
	Sockaddr.sin_family = AF_INET;
	Sockaddr.sin_port = htons(12345);
	Sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	bind(masterSocket, (const sockaddr*)&Sockaddr, sizeof(Sockaddr));
	listen(masterSocket, SOMAXCONN);
	
	while(true){
		auto slaveSocket = accept(masterSocket, 0, 0);
		std::cout << "Slave socket descriptor: " << slaveSocket << std::endl;
		char buffer [1024];
		recv(slaveSocket, buffer, 1024, MSG_NOSIGNAL);
		std::cout << "Data: " << std::string(buffer);
		shutdown(slaveSocket, SHUT_RDWR);
		close(slaveSocket);
	}
	return 0;
}