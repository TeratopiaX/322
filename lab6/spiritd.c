// Name: Anh (Teresa) Doan
// Ilia Benson 322
// Project: Lab 6 Whack-a-Mole

#include <fcntl.h> 
#include <signal.h>
#include <stdbool.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/stat.h> 
#include <syslog.h> 
#include <time.h>
#include <unistd.h>

#define MOLES_NUM 2

bool terminated = false;

pid_t moles[] = {-1, -1};
char molePath[206];
char logPath[209];

void createMole() {
  int moleNumber = rand() % MOLES_NUM;
  // Mole already exists
  if (moles[moleNumber] > 0) {
    return;
  }
  
  //create child and store process id for mole
  pid_t child = fork();
  if (child < 0) {
      fprintf(stderr, "Fork failed\n");
      return;
  } else if (child > 0) {
      moles[moleNumber] = child;
      return;
  }
  
  char moleName[6];
  sprintf(moleName, "mole%d", moleNumber + 1);
  char *const argv[] = { moleName, logPath, NULL};
  execv(molePath, argv);
}

// kill the mole
void whackMole(int moleNumber) {
    if (moles[moleNumber] > 0) {
        kill(moles[moleNumber], SIGTERM);
        moles[moleNumber] = -1;
    }
}

void sigTermHandler(int signal) {
    if (signal != SIGTERM) {
        return;
    }
    
    for(int i = 0; i < MOLES_NUM; i++) {
        whackMole(i);
    }
    // set global program termination flag
    terminated = true;
}

void sigUsr1Handler(int signal) {
  if(signal != SIGUSR1) {
      return; 
  }
  
  whackMole(0);
  createMole();
}

void sigUsr2Handler(int signal) {
  if(signal != SIGUSR2) {
      return; 
  }
  
  whackMole(1);
  createMole();
}

int registerHandlers() {
  if (signal(SIGTERM, sigTermHandler) == SIG_ERR) {
      fprintf(stderr, "Can't catch signal SIGTERM\n");
      return -1;
  }
  if (signal(SIGUSR1,sigUsr1Handler ) == SIG_ERR) {
      fprintf(stderr, "Can't catch signal SIGUSR1\n");
      return -1;
  }
  if (signal(SIGUSR2,sigUsr2Handler ) == SIG_ERR) {
      fprintf(stderr, "Can't catch signal SIGUSR2\n");
      return -1;
  }

  return 0;
}

// create daemon
int daemonize(const char *cmd) {
    unsigned long long i; 
    int fd0, fd1, fd2;
    pid_t pid;
    struct rlimit rl;
    struct sigaction sa;

    /** Clear file creation mask. */
    umask(0);

    /** Get maximum number of file descriptors. */
    if (getrlimit(RLIMIT_NOFILE, &rl) < 0) {
        fprintf(stderr, "%s: Failed to get file limit, exiting\n", cmd);
        return -1;
    }

    /** Become a session leader to lose controlling TTY. */
    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "%s: Fork failed, exiting\n", cmd);
        return -1;
    } else if (pid != 0) { /* parent */ 
        return pid;
    }
    setsid();

    /** Ensure future opens won’t allocate controlling TTYs. */
    sa.sa_handler = SIG_IGN; 
    sigemptyset(&sa.sa_mask);

    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0) {
      fprintf(stderr, "%s: Ignoring SIGHUP failed, exiting\n", cmd);
        return -1;
    }
    
    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "%s: Fork failed, exiting\n", cmd);
        return -1;
    } else if (pid != 0) { /* parent */ 
        fprintf(stderr, "spiritd: $$ = %d\n", pid);
        return pid;
    }

    /**
     * Change the current working directory to the root so 
     * we won’t prevent file systems from being unmounted. 
     */
    if (chdir("/") < 0) {
        fprintf(stderr, "%s: Can't change directory to /, exiting\n", cmd);
        return -1;
    }
        
    /** Close all open file descriptors. */
    if (rl.rlim_max == RLIM_INFINITY) {
      rl.rlim_max = 1024;
    }
    for (i = 0; i < rl.rlim_max; i++) {
      close(i);
    }

    /** Attach file descriptors 0, 1, and 2 to /dev/null. */
    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);
    
    /** Initialize the log file. */
    openlog(cmd, LOG_CONS, LOG_DAEMON);
    if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
        syslog(LOG_ERR, "unexpected file descriptors %d %d %d\n",     
               fd0, fd1, fd2);
        return -1; 
    }
  
    return 0;
}

void setMolePaths() {
    char path[200];
    getcwd(path, 200);
    sprintf(molePath, "%s/moles", path);
    sprintf(logPath, "%s/lab6.log", path);
}

int main() {
    setMolePaths();
  
    pid_t child_pid = daemonize("spiritd");
    if (child_pid < 0) {
        return -1;
    } else if (child_pid > 0) { /* parent */ 
        return 0;
    }
  
    srand(time(0));
    if (registerHandlers() != 0) {
        return -1;
    }

    while (!terminated) {
        sleep(1); /* wait 1 second */
    }
    return 0;
}
