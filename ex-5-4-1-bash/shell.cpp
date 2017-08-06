#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

void create_pipe() {
	int pfd[2];
	int pfd1[2];
	int pfd2[2];
	int file = open("/home/box/res.out", O_WRONLY | O_CREAT, 0666);
	pipe(pfd);
	if (!fork()) {
		close(STDOUT_FILENO);
		dup2(pfd[1], STDOUT_FILENO);
		close(pfd[1]);
		close(pfd[0]);
		execlp("who", "who", NULL);
	}
	else {
		printf("fork1 \n");
		pipe(pfd1);
		if (!fork()) { // create unique
			close(STDIN_FILENO);
			close(STDOUT_FILENO);
			dup2(pfd[0], STDIN_FILENO);
			dup2(pfd1[1], STDOUT_FILENO);
			close(pfd[1]);
			close(pfd[0]);
			close(pfd1[0]);
			close(pfd1[1]);
			execlp("sort", "sort", NULL);
		}
		else {
			printf("fork2 \n");
			pipe(pfd2);
			if (!fork()) {
				close(STDIN_FILENO);
				close(STDOUT_FILENO);
				dup2(pfd1[0], STDIN_FILENO);
				dup2(pfd2[1], STDOUT_FILENO);
				close(pfd1[1]);
				close(pfd1[0]);
				close(pfd2[1]);
				close(pfd2[0]);
				execlp("unique", "unique", "-c", NULL);
			}
			else {
				printf("fork3 \n");
				close(STDIN_FILENO);
				close(STDOUT_FILENO);
				dup2(pfd2[0], STDIN_FILENO);
				dup2(file, STDOUT_FILENO);
				close(pfd2[1]);
				close(pfd2[0]);
				close(file);
				execlp("sort", "sort", "-nk1", NULL);
			}
		}
	}
}

int main(int argc, char **argv) {
	create_pipe();
	return 0;
}
