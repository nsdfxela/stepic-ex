#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>

int set_non_block (int fd){
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
	std::cout << "I'am chat client" << std::endl;
}