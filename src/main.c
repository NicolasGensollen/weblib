#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>

#include "ip.h"

#define MSG_SIZE 13

/* Callback when daemon is killed. */

void sighandler(int signum)
{
	FILE *fp = NULL;
	fp = fopen("Log.txt", "a");
	fprintf(fp, "Caught signal %d, killing daemon...\n", signum);   

	// Save rules to file
	apply_rule("sudo iptables-save > iptables.rules");
	fprintf(fp, "Saving rules to config file iptables.rules...\n");

	// Flush and lock port 22
	flush();
	disable_all_ip();
	fprintf(fp, "Locking all IPs before dying.\n");
 
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

		// Open a log file in write mode
		fp = fopen("Log.txt", "w+");
		fprintf(fp, "Deamon started!\n");

		// Initially block all IPs
		fprintf(fp, "Block all IPs.\n");
		
		// Flush FILTER
		flush();

		// Port 22 (SSH) is blocked
		disable_all_ip();

		// Load previously saved config
		apply_rule("sudo iptables-restore < ./iptables.rules");
		fprintf(fp, "Load previous config from file iptables.rules...\n");
            
		while (1)
		{
			sleep(2);

			fprintf(fp, "Listening...\n");
			fflush(fp);
		
			char ip[MSG_SIZE];
			int read_value;
			read_value = read(mypipe[0], ip, MSG_SIZE);
		
			if (read_value > 0)
			{
				fprintf(fp, "IP received = %s\n", ip);
				char* new_rule = forge_ip_authorization(ip);
				fprintf(fp, "New rule : %s\n", new_rule);
				fflush(fp);
				apply_rule(new_rule);      
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

		char* ip1 = "192.168.1.18"; 
		char* new_rule = forge_ip_authorization(ip1);
                printf("New rule : %s\n", new_rule);
		printf("Sending to daemon : %s\n", ip1);
		write(mypipe[1], ip1, MSG_SIZE);

		sleep(5);

		char* ip2 = "192.168.1.20"; 
		char* new_rule2 = forge_ip_authorization(ip2);
                printf("New rule : %s\n", new_rule2);
		printf("Sending to daemon : %s\n", ip2);
		write(mypipe[1], ip2, MSG_SIZE);
	    
		close(mypipe[1]);
		printf("Done!\n");
            
		return EXIT_SUCCESS;
	}
}

