/*
Anh Doan
Ilia Benson 322
Lab 3
This lab catches signals and then prints out information
about those signals such as its process ID and the time it was caught
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

static int signalCount = 0;
static int sigTermCount = 0;

void signal_handler(int signal) { 
	char sigName[10] = "";
	signalCount++;

	if (signal == SIGTERM) {
		sigTermCount++;
	} else {
		sigTermCount = 0;
	}

	switch(signal) {
		case SIGHUP: 
			strncpy(sigName, "SIGHUP", 10);
			break;
		case SIGINT:
			strncpy(sigName, "SIGINT", 10);
			break;
		case SIGQUIT: 
			strncpy(sigName, "SIGQUIT", 10);
			break;
		case SIGILL:
			strncpy(sigName, "SIGILL", 10);
			break;
		case SIGTRAP: 
			strncpy(sigName, "SIGTRAP", 10);
			break;
		case SIGABRT:
			strncpy(sigName, "SIGABRT", 10);
			break;
		// SIGIOT shares the value of 6 with SIGABRT
		// case SIGIOT: 
			// strncpy(sigName, "SIGIOT", 10);
			// break;
		case SIGBUS:
			strncpy(sigName, "SIGBUS", 10);
			break;
		case SIGFPE: 
			strncpy(sigName, "SIGFPE", 10);
			break;
		case SIGUSR1:
			strncpy(sigName, "SIGUSR1", 10);
			break;
		case SIGSEGV: 
			strncpy(sigName, "SIGSEGV", 10);
			break;
		case SIGUSR2:
			strncpy(sigName, "SIGUSR2", 10);
			break;
		case SIGPIPE: 
			strncpy(sigName, "SIGPIPE", 10);
			break;
		case SIGALRM:
			strncpy(sigName, "SIGALRM", 10);
			break;
		case SIGTERM: 
			strncpy(sigName, "SIGTERM", 10);
			break;
		// SIGSTKFLT not defined on this architecture
		// case SIGSTKFLT:
			// strncpy(sigName, "SIGSTKFLT", 10);
			// break;
		case SIGCHLD: 
			strncpy(sigName, "SIGCHLD", 10);
			break;
		case SIGCONT:
			strncpy(sigName, "SIGCONT", 10);
			break;
		case SIGTSTP: 
			strncpy(sigName, "SIGTSTP", 10);
			break;
		case SIGTTIN:
			strncpy(sigName, "SIGTTIN", 10);
			break;
		case SIGTTOU: 
			strncpy(sigName, "SIGTTOU", 10);
			break;
		case SIGURG:
			strncpy(sigName, "SIGURG", 10);
			break;
		case SIGXCPU: 
			strncpy(sigName, "SIGXCPU", 10);
			break;
		case SIGXFSZ:
			strncpy(sigName, "SIGXFSZ", 10);
			break;
		case SIGVTALRM: 
			strncpy(sigName, "SIGVTALRM", 10);
			break;
		case SIGPROF:
			strncpy(sigName, "SIGPROF", 10);
			break;
		case SIGWINCH: 
			strncpy(sigName, "SIGWINCH", 10);
			break;
	}
	time_t signalCaughtTime = time(NULL);
	fprintf(stdout, "%s caught at %ld\n", sigName, signalCaughtTime);
}

int sigNametoNumber(const char sigName[]) {
	if (strcmp(sigName, "HUP") == 0) {
		return SIGHUP;
	} else if (strcmp(sigName, "INT") == 0) {
		return SIGINT;
	}
	 else if (strcmp(sigName, "QUIT") == 0) {
		return SIGQUIT;
	}
	else if (strcmp(sigName, "ILL") == 0) {
		return SIGILL;
	}
	else if (strcmp(sigName, "TRAP") == 0) {
		return SIGTRAP;
	}
	else if (strcmp(sigName, "ABRT") == 0) {
		return SIGABRT;
	}
	else if (strcmp(sigName, "IOT") == 0) {
		return SIGIOT;
	}
	else if (strcmp(sigName, "BUS") == 0) {
		return SIGBUS;
	}
	else if (strcmp(sigName, "FPE") == 0) {
		return SIGFPE;
	}
	else if (strcmp(sigName, "USR1") == 0) {
		return SIGUSR1;
	}
	else if (strcmp(sigName, "SEGV") == 0) {
		return SIGSEGV;
	}
	else if (strcmp(sigName, "USR2") == 0) {
		return SIGUSR2;
	}
	else if (strcmp(sigName, "PIPE") == 0) {
		return SIGPIPE;
	}
	else if (strcmp(sigName, "ALRM") == 0) {
		return SIGALRM;
	}
	else if (strcmp(sigName, "TERM") == 0) {
		return SIGTERM;
	}
	// else if (strcmp(sigName, "STKFLT") == 0) {
		// SIGSTKFLT not defined on this architecture
		// return SIGSTKFLT;
	// }
	else if (strcmp(sigName, "CHLD") == 0) {
		return SIGCHLD;
	}
	else if (strcmp(sigName, "CONT") == 0) {
		return SIGCONT;
	}
	else if (strcmp(sigName, "TSTP") == 0) {
		return SIGTSTP;
	}
	else if (strcmp(sigName, "TTIN") == 0) {
		return SIGTTIN;
	}
	else if (strcmp(sigName, "TTOU") == 0) {
		return SIGTTOU;
	}
	else if (strcmp(sigName, "URG") == 0) {
		return SIGURG;
	}
	else if (strcmp(sigName, "XCPU") == 0) {
		return SIGXCPU;
	}
	else if (strcmp(sigName, "XFSZ") == 0) {
		return SIGXFSZ;
	}
	else if (strcmp(sigName, "VTALRM") == 0) {
		return SIGVTALRM;
	}
	else if (strcmp(sigName, "PROF") == 0) {
		return SIGPROF;
	}
	else if (strcmp(sigName, "WINCH") == 0) {
		return SIGWINCH;
	}

	fprintf(stderr, "Invalid signal: %s", sigName);
	return -1;
}

void registerHandler(const char sigName[]) {
	int sigNo = sigNametoNumber(sigName);
	if (signal(sigNo, signal_handler) == SIG_ERR) {
        fprintf(stderr, "Can't catch signal SIG%s\n", sigName);
	}
}

void registerHandlers(int argc, char const *argv[]) {
    for(int i = 1; i < argc; i++) {
    	registerHandler(argv[i]);
    }
}

int main(int argc, char const *argv[])
{
	if (argc <= 1) {
    	fprintf(stderr, "Error, no signals specified.\n");
    	return -1;
  	}

	pid_t pid = getpid();
	fprintf(stderr, "catcher: $$ = %d\n", pid);
	fprintf(stdout, "\n");

    while(sigTermCount < 3) {
    	registerHandlers(argc, argv);
        pause();
    }

    fprintf(stderr, "catcher: Total signals count = %d\n", signalCount);
    return 0;
}