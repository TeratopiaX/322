/*

Anh Doan
lab 4
Ilia Benson 
322
*/
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>          
#include <semaphore.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> 
#include <time.h>
#include <unistd.h>

#define SEM_FILE_ROOT "/chopstick_"
#define SEM_GLOBAL_FILE "/chopstick_global"

bool terminated = false;

void eat(int position) {
	if(terminated) {
		return;
	}

	fprintf(stdout, "Philosopher #%d is eating\n", position);
	// Sleep for up to 5 seconds
	usleep(rand() % 5000000);
}

void think(int position) {
	if(terminated) {
		return;
	}

	fprintf(stdout, "Philosopher #%d is thinking\n", position); 
	// Sleep for up to 5 seconds
	usleep(rand() % 5000000);
}

void waitForSemaphore(sem_t * semaphore) {
	if(terminated) {
		return;
	}

	sem_wait(semaphore);
}

void signalForSemaphore(sem_t * semaphore) {
	if(terminated) {
		return;
	}

	sem_post(semaphore);
}

void signalHandler(int signal) {
	if(signal == SIGTERM) {
		// set global program termination flag
		terminated = true;
	}
}

sem_t * openSemaphore(char * semaphorePath) {
	sem_t * returnVal = sem_open(semaphorePath, O_CREAT|O_EXCL, 0666, 1);
	if (returnVal == SEM_FAILED ) {
    	perror(NULL);
    	returnVal = sem_open(semaphorePath, 0);
  	}
  	return returnVal;
}

int philosopherLoop(
	int position, 
	sem_t * firstChopstick, 
	sem_t * secondChopstick,
	sem_t * globalLock) {

	int cycleCount = 0;
	do {
		// set globalLock while acquiring resources
		waitForSemaphore(globalLock);

		// acquire left and right chopsticks
		waitForSemaphore(firstChopstick);
		waitForSemaphore(secondChopstick);

		// release globalLock
		signalForSemaphore(globalLock);

		// eat for a while
		eat(position);

		// release left and right chopsticks
		signalForSemaphore(firstChopstick);
		signalForSemaphore(secondChopstick);

		// think for a while
		think(position);

		if (terminated) {
			return cycleCount;
		}
		cycleCount++;
	} while (true);
}

void philosopher(int seats, int position) {
	char firstChopstickPath[20];
	char secondChopstickPath[20];

    // determine correct paths for chopsticks
    sprintf(firstChopstickPath,"%s%d", SEM_FILE_ROOT, position);
    sprintf(secondChopstickPath,"%s%d", SEM_FILE_ROOT, (position + 1) % seats);

	sem_t * firstChopstick = openSemaphore(firstChopstickPath);
  	sem_t * secondChopstick = openSemaphore(secondChopstickPath);
  	sem_t * globalLock = openSemaphore(SEM_GLOBAL_FILE);

	int cycleCount = philosopherLoop(
		position, firstChopstick, secondChopstick, globalLock);

	fprintf(stderr, "Philosopher #%d completed %d cycles \n", 
		position, cycleCount);

	// close and unlink all semaphores
	sem_close(firstChopstick);
	sem_close(secondChopstick);
	sem_close(globalLock);
	sem_unlink(firstChopstickPath);
	sem_unlink(secondChopstickPath);
	sem_unlink(SEM_GLOBAL_FILE);
}

  
int main(int argc, char const *argv[]) 
{
	if (argc <= 2) {
    	fprintf(stderr, "Error, not enough arguments.\n");
    	return -1;
  	}
 
  	for(int i = 0; argv[1][i] != '\0'; i++) {
  		if(!isdigit(argv[1][i])){
    		fprintf(stderr, "Error, seats value is not an integer.\n");
    		return -1;
  		}
  	}

  	for(int i = 0; argv[2][i] != '\0'; i++) {
  		if(!isdigit(argv[2][i])){
    		fprintf(stderr, "Error, position value is not an integer.\n");
    		return -1;
  		}
  	}

	int seats;
    int position;
  	sscanf(argv[1], "%d", &seats);
  	sscanf(argv[2], "%d", &position);

  	pid_t pid = getpid();
	fprintf(stderr, "Philosopher #%d: $$ = %d\n", position, pid);

    // register handler for SIGTERM
  	if (signal(SIGTERM, signalHandler) == SIG_ERR) {
        fprintf(stderr, "Can't catch SIGTERM\n");
        return -1;
	}

    // seed random number generator with current time
	srand(time(0));
	philosopher(seats, position);
	return 0;
} 



