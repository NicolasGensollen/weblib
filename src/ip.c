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

/* Disable all IP on port 22 (SSH) */

void disable_all_ip()
{
    char* rule = "sudo iptables -t filter -A INPUT -p tcp --dport 22 -j DROP";
    apply_rule(rule);
}

/* Return the accept rule for a given IP address */

char* forge_ip_authorization(char* ip)
{

    char* rule = malloc( sizeof(char) * (IP_RULE_MAX_SIZE+1));
    char* begin = "sudo iptables -t filter --src";
    char* end = "-A INPUT -p TCP --dport 22 -j ACCEPT";
    snprintf(rule, IP_RULE_MAX_SIZE+1, "%s %s %s", begin, ip, end);
    return rule;
}

