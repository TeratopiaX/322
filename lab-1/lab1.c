#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>


/**
 * Reports info about the current process and its child (if applicable).
 *
 * @param childProcId the process id of the current process' child
 */
void reportProcInfo(pid_t childProcId) {
    // Wait until the child process is completed and store its return status
    int status;
    waitpid(childProcId, &status, 0);

    // Store and print out id of this process and its parent
    pid_t parentProcId = getppid();
    pid_t procId = getpid();
    printf("PPID: %5d, PID: %5d", parentProcId, procId);
  
    // This is the child process if childProcId is 0
    if(childProcId == 0) {
        printf("\n");
        return;
    }
    // Print out the process id and return value of the child process
    printf(", CPID: %4d, RETVAL: %d\n", childProcId, status);

    // Store and print out process times
    struct tms timeObj;
    times(&timeObj);
    printf("USER:  %4ld, SYS:  %4ld\n", timeObj.tms_utime, timeObj.tms_stime);
    printf("CUSER: %4ld, CSYS: %4ld\n", timeObj.tms_cutime, timeObj.tms_cstime);
}


int main(int argc, char const *argv[]) {
    // Print the start time of the program in unix time (seconds)
    time_t startTime = time(NULL);
    printf("START: %ld\n",startTime);

    // Fork the process into a parent and child
    pid_t childProcId = fork();
    // Report info about the current process and its child (if applicable)
    reportProcInfo(childProcId);

    // This is the parent process if childProcId is not 0
    if (childProcId != 0) {
        // Print the stop time of the program in unix time (seconds)
        time_t stopTime = time(NULL);
        printf("STOP: %ld\n",stopTime);
    }

    return 0;
}