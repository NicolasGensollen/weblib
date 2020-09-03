#include <stdio.h>
#include <stdlib.h>

#ifndef IP_H
#define IP_H

void apply_rule(char* rule);	/* Apply a given rule by calling system */
void flush(); 			/* Flush INPUT chain of FILTER */

#endif
