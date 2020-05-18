/* 
Anh Doan
Ilia Benson 322
Lab 2 - pipe program: write a command-line tool that launches two child processes 
and sets up a pipe between them for inter-process communication.  
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <unistd.h>

/************ launchChildProgram *****************/ 
void launchChildProgram(const char *pathname, char *const *argv, int pipefd[]) {
  dup2(pipefd[0], STDIN_FILENO);
  dup2(pipefd[1], STDOUT_FILENO);
  char *const envParams[1] = { NULL };
  // The child process executes the supplied command   
  execve(pathname, argv, envParams);
}

/************ logChildPid *****************/ 
void logChildPid(const char *pathname, pid_t cpid) {
  // The parent process prints the PID of the child on stderr  
  fprintf(stderr, "%s: $$ = %d\n", pathname, cpid); 
  
}
/************ waitForChild *****************/ 
int waitForChild(pid_t cpid) {
  // The parent prints the return value of the child on stderr  
  int status;
  waitpid(cpid, &status, 0);
  return status;
}
/************ logChildStatus *****************/ 
void logChildStatus(const char *pathname, int status) {
  // The parent prints the return value of the child on stderr 
  fprintf(stderr, "%s: $? = %d\n", pathname, status);
}

int getCommandSeparatorIndex(int argc, char **argv) {
  for (int i = 1; i < argc; i++) {
  	if (strncmp(argv[i], ",", 2) == 0) {
  	  return i;
  	}
  }
  return -1;
}

/************ tubeParent *****************/        
void tubeParent(int argc, char **argv) {
  //create pipe
  int pipefd[2];
  pipe(pipefd);

  int separatorIndex = getCommandSeparatorIndex(argc, argv); 
  if (separatorIndex == -1) {
  	printf("Error, no comma separating listed commands");
  	return;
  }
  argv[separatorIndex] = NULL;
  //fork first child
  pid_t cpid1 = fork();
  char *const *argv1 = &argv[1];
  const char *pathname1 = argv1[0];
  if (cpid1 == 0) {
    launchChildProgram(pathname1, argv1, pipefd);
    return;
  }
  //fork second child
  pid_t cpid2 = fork();
  char *const *argv2 = &argv[separatorIndex + 1];
  const char *pathname2 = argv2[0];
  if (cpid2 == 0) {
    launchChildProgram(pathname2, argv2, pipefd);
    return;
  }

  logChildPid(pathname1, cpid1);
  logChildPid(pathname2, cpid2);

  int childStatus1 = waitForChild(cpid1);
  int childStatus2 = waitForChild(cpid2);

  //close pipes for writing and reading
  close(pipefd[0]);
  close(pipefd[1]);

  logChildStatus(pathname1, childStatus1);
  logChildStatus(pathname2, childStatus2);
}

int main(int argc, char **argv) {
  if (argc > 3) {
    tubeParent(argc, argv);
  } else {
    printf("Error, not enough arguments specfied.");
  }
  
  return 0;
}
