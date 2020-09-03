#include <stdio.h>
#include <stdlib.h>

#ifndef IP_H
#define IP_H

#define IP_RULE_MAX_SIZE 500

void apply_rule(char* rule);			/* Apply a given rule by calling system */
void flush(); 					/* Flush INPUT chain of FILTER */
void disable_all_ip();				/* Disable all IP on port 22 (SSH) */
char* forge_ip_authorization(char* ip);   /* Return the accept rule for a given IP address */

#endif
