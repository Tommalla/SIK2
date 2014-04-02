/* Tomasz Zakrzewski, tz336079
 *  SIK2, utils
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "err.h"
#include "utils.h"

int str_to_short(char* str, const char* error) {
	short int res;

	res = safe_str_to_short(str);
	if (res < 0)
		fatal(error);

	return res;
}

int safe_str_to_short(char* str) {
	char * endptr;
	short int res;
	size_t len = strlen(str);

	endptr = str + len;
	res = (short int)strtol(str, &endptr, 10);

	return (endptr != str + len) ? -1 : res;
}

void tcp_write(int sock, char* msg) {
	ssize_t len = strlen(msg);
	if (write(sock, msg, len) != len)
		syserr("partial / failed TCP write");
}
