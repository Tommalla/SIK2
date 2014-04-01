/* Tomasz Zakrzewski, tz336079
 *  SIK2, utils
 */
#include <string.h>
#include <stdlib.h>

#include "err.h"
#include "utils.h"

int str_to_short(char* str, const char* error) {
	char * endptr;
	short int res;

	endptr = str + strlen(str);
	res = (short int)strtol(str, &endptr, 10);

	if (endptr != str + strlen(str))
		fatal(error);
	return res;
}