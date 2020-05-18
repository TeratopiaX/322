/* 
Anh Doan
Ilia Benson 322
Lab 2 - launch program: a program that sets to programs into motion that are connected via
a tube that transports inter-process communication, while also report some basic information.
*/
#include <stdio.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <unistd.h>

/************ launchChildProgram *****************/ 
void launchChildProgram(const char *pathname, char *const *argv) {
  // The child prepare the new argv structure
  char *const *childArgs = &argv[1];
  char *const envParams[1] = { NULL };
  // The child process executes the supplied command   
  execve(pathname, childArgs, envParams);
}

/************ logChildStatus *****************/ 
void logChildStatus(const char *pathname, pid_t cpid) {
  // The parent process prints the PID of the child on stderr  
  fprintf(stderr, "%s: $$ = %d\n", pathname, cpid); 
  int status;
  // The parent prints the return value of the child on stderr 
  waitpid(cpid, &status, 0);
  fprintf(stderr, "%s: $? = %d\n", pathname, status);
}

/************ birth *****************/      
void birth(char *const *argv) {
  /* fork a child process */ 
  pid_t cpid = fork();
  const char *pathname = argv[1];
  if (cpid == 0) {
    launchChildProgram(pathname, argv);
  } else {
    logChildStatus(pathname, cpid);
  }
}

int main(int argc, char *const *argv) {
  if (argc > 1) {
    birth(argv);
  } else {
    printf("Error, no program to launch specified.");
  }
  
  return 0;
}
