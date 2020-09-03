# include "ip.h"



/* Apply a given rule by calling system */

void apply_rule(char* rule)
{
    if (system(rule) == -1)
	{
	    FILE *fp = NULL;
	    fprintf (stderr, "Failure when applying rule: %s.\n", rule);
	    fp = fopen("Log.txt", "a");
    	    fprintf(fp, "Failure when applying rule: %s.\n", rule);
	    fclose(fp);
	}
}

/* Flush INPUT chain of FILTER */

void flush()
{
    char* rule = "sudo iptables -t filter -F INPUT";
    apply_rule(rule);
}
