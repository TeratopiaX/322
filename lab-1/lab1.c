#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
  time_t startTime = time(NULL);
  printf("START: %ld\n",startTime);
  
  pid_t childProcessId = fork();
  int status;
  waitpid(childProcessId, &status, 0);
  
  pid_t parentProcessId = getppid();
  pid_t processId = getpid();
  printf("PPID: %5d, PID: %5d", parentProcessId, processId);
  if(childProcessId == 0) {
    printf("\n");
    exit(EXIT_SUCCESS);
  }
  printf(", CPID: %4d, RETVAL: %d", childProcessId, status); 
  printf("\n");
  
  struct tms timesObj;
  times(&timesObj);
  printf("USER:  %4ld, SYS:  %4ld\n", timesObj.tms_utime, timesObj.tms_stime); 
  printf("CUSER: %4ld, CSYS: %4ld\n", timesObj.tms_cutime, timesObj.tms_cstime); 

  time_t stopTime = time(NULL);
  printf("STOP: %ld\n",stopTime);
  
  return 0;
}