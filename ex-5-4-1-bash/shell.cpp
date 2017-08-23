#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <cstring>

class cmd {
public:
	std::string command;
	char **params = NULL;
cmd () { }
~cmd() { }	
};


// cat t1 | grep 2015 | sort
void create_pipe(std::vector <cmd> &commands) {
	int pfds [commands.size()][2];
	int file = open("./res.out", O_WRONLY | O_CREAT, 0666);
	
	int *n_pfd_1  = pfds[0];
	int *n_pfd_2  = pfds[1];
	
	for(int i = 0; i < commands.size(); i++) {
	
	
	pipe(n_pfd_1);
	printf("cycle %d \n", i);
		if(!fork()) {			
			if(!i) {
				printf("thread 0 \n");	
				close(STDOUT_FILENO);
				dup2(n_pfd_1[1], STDOUT_FILENO);
				close(n_pfd_1[0]);				
				close(n_pfd_1[1]);
				execvp(commands[i].command.c_str(), commands[i].params);
			} else {
				bool last = (i == (commands.size() - 1) );
				printf("thread %d \n", i+1);	
			
				close(STDIN_FILENO);
				close(STDOUT_FILENO);
			
				dup2(n_pfd_1[0], STDIN_FILENO);			
				dup2( last ? file : n_pfd_2[1], STDOUT_FILENO);
				close(n_pfd_1[1]);
				close(n_pfd_1[0]);
				close(n_pfd_2[0]);
				close(n_pfd_2[1]);
				if(last) {
					close(file); 
				} else { 
					pipe(n_pfd_2); 
					n_pfd_1 = pfds[i];
					n_pfd_2 = pfds[i + 1];
				}
				execvp(commands[i].command.c_str(), commands[i].params);						
			}
		} else {
			printf("thread %d breaking \n", i);	
			/*close(STDOUT_FILENO);
			dup2(n_pfd_1[1], STDOUT_FILENO);
			close(n_pfd_1[1]);
			close(n_pfd_1[0]);
			execvp(commands[i].command.c_str(), commands[i].params);*/						
			break;
		}
	}
	

	/*int pfd[2];
	int pfd1[2];
	int pfd2[2];
	//int file = open("/home/box/res.out", O_WRONLY | O_CREAT, 0666);
	
	pipe(pfd);
	//printf("cmd: %s param: %s \n", commands[0].command.c_str(), commands[0].params[0]);
	if (!fork()) {
		close(STDOUT_FILENO);
		dup2(pfd[1], STDOUT_FILENO);
		close(pfd[1]);
		close(pfd[0]);		
		
		//execlp("cat", "cat", "/home/nsdfxela/workspace/task1-20170604/ex-5-4-1-bash/t1", NULL);
		execvp(commands[0].command.c_str(), commands[0].params);
	} else {
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
		} else {
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
	}*/
	//delete [] commands[0].params;
}

// cat t1 | grep 2015 | sort
void parse_cmd(std::vector <cmd> &pipe) {
	cmd cmd1, cmd2, cmd3;
	
	cmd1.command = "cat";
	cmd1.params = new char *[2];
	cmd1.params[0] = new char[4];
	strcpy(cmd1.params[0], "cat\0");
	cmd1.params[1] = new char[3];
	strcpy(cmd1.params[1], "t1\0");	
	pipe.push_back(cmd1);
	//printf("pipe created: %s \n", cmd1.params[1]);	
	cmd2.command = "grep";
	cmd2.params = new char *[2];
	cmd2.params[0] = new char[5];
	strcpy(cmd2.params[0], "grep\0");
	cmd2.params[1] = new char[5];
	strcpy(cmd2.params[1], "2015\0");	
	pipe.push_back(cmd2);
	
	cmd3.command = "sort";
	cmd3.params = new char *[1];
	cmd3.params[0] = new char[5];
	strcpy(cmd3.params[0], "sort\0");		
	pipe.push_back(cmd3);
	
	/*pipe.push_back("grep 2015");
	pipe.push_back("sort");*/
}

int main(int argc, char **argv) {
	std::vector<cmd> commands;
	parse_cmd(commands);
	create_pipe(commands);
	return 0;
}
