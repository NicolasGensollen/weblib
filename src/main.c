#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>

#include "ip.h"

#define MSG_SIZE 14

/* Callback when daemon is killed. */

void sighandler(int signum)
{
	FILE *fp = NULL;
	fp = fopen("Log.txt", "a");
	fprintf(fp, "Caught signal %d, killing daemon...\n", signum);    
	fclose(fp);
	exit(1);
}

int main ()
{
	// Configure pipe
	pid_t pid = 0;
	pid_t sid = 0;
	int mypipe[2];

	FILE *fp = NULL;

	/* Create the pipe. */
	if (pipe (mypipe))
	{
		fprintf (stderr, "Pipe failed.\n");
		return EXIT_FAILURE;
	}

	/* Create the child process. */
	pid = fork ();

	if (pid == (pid_t) 0)
	{
		/* CHILD PROCESS */
		/* Daemonize it first */
            
		// unmask the file mode
		umask(0);
            
		// Set new session
		sid = setsid();
		if (sid < 0)
		{
			return EXIT_FAILURE;
		}

		// Change the curent directory to root
		chdir("/");

		close (mypipe[1]);
            
		// Close stdin. stdout and stderr
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);

		// Callback on SIGTERM
		signal(SIGTERM, sighandler);

		// Flush FILTER
		flush();

		// Open a log file in write mode
		fp = fopen("Log.txt", "w+");
		fprintf(fp, "Deamon started!\n");
            
		while (1)
		{
			sleep(2);

			fprintf(fp, "Listening...\n");
			fflush(fp);
		
			char msg[MSG_SIZE];
			int read_value;
			read_value = read(mypipe[0], msg, MSG_SIZE);
		
			if (read_value > 0)
			{
				fprintf(fp, "Message received = %s\n", msg);
				fflush(fp);         
			}
		
		}
		close(mypipe[0]);
		fclose(fp);
		return EXIT_SUCCESS;
	}
	else if (pid < (pid_t) 0)
	{
		/* The fork failed. */
		fprintf (stderr, "Fork failed.\n");
		return EXIT_FAILURE;
	}
	else
	{
		/* PARENT PROCESS */
		close (mypipe[0]);
		printf("Process_id of child process %d \n", pid);

		char* msg1 = "Hello, World!"; 
		printf("Sending to daemon : %s\n", msg1);
		write(mypipe[1], msg1, MSG_SIZE);

		sleep(5);

		char* msg2 = "Good bye !!!!"; 
		printf("Sending to daemon : %s\n", msg2);
		write(mypipe[1], msg2, MSG_SIZE);
	    
		close(mypipe[1]);
		printf("Done!\n");
            
		return EXIT_SUCCESS;
	}
}

