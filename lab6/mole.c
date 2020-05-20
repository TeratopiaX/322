#include <stdbool.h>
#include <stdio.h> 
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>

bool terminated = false;

void sigTermHandler(int signal) {
    if (signal == SIGTERM) {
    	// set global program termination flag
        terminated = true;
    }
}

int main(int argc, char* argv[]) {
	if (argc <= 1) {
    	fprintf(stderr, "Error, not enough arguments.\n");
    	return -1;
  	}

	FILE *fp;
	fp = fopen (argv[1], "a"); 
	fprintf(fp, "Pop %s\n", argv[0]);
	fclose(fp);

	if (signal(SIGTERM, sigTermHandler) == SIG_ERR) {
	    fprintf(stderr, "Can't catch signal SIGTERM\n");
     	return -1;
	}

	while(!terminated) {
		sleep(1);
	}
	return 0;
}
