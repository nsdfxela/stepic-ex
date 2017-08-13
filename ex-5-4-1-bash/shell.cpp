#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <vector>
#include <string>

// cat t1 | grep 2015 | sort
void create_pipe() {
	int pfd[2];
	int pfd1[2];
	int pfd2[2];
	//int file = open("/home/box/res.out", O_WRONLY | O_CREAT, 0666);
	int file = open("./res.out", O_WRONLY | O_CREAT, 0666);
	pipe(pfd);
	if (!fork()) {
		close(STDOUT_FILENO);
		dup2(pfd[1], STDOUT_FILENO);
		close(pfd[1]);
		close(pfd[0]);
		execlp("cat", "cat", "/home/nsdfxela/workspace/task1-20170604/ex-5-4-1-bash/t1", NULL);
	}
	else {
		printf("fork1 \n");
		close(STDIN_FILENO);
		dup2(pfd[0], STDIN_FILENO);
		close(pfd[1]);
		close(pfd[0]);
		execlp("grep", "grep", "2015", NULL);
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
			execlp("grep", "grep", "2015", NULL);
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
				execlp("sort", "sort", NULL);
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
				execlp("sort", "sort", NULL);
			}
		}
	}
}

// cat t1 | grep 2015 | sort
void parse_cmd(std::vector <std::string> &pipe) {
	pipe.push_back("cat t1");
	pipe.push_back("grep 2015");
	pipe.push_back("sort");
}

int main(int argc, char **argv) {
	create_pipe();
	return 0;
}
