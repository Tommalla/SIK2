/* Tomasz Zakrzewski, tz336079
 *  SIK2, utils
 */
#ifndef UTILS_H
#define UTILS_H

#define CORRECT_ANSWER "OK"

/**
 * @brief Returns the decimal value of a string or stops the program if the conversion failed.
 */
int str_to_short(char* str, const char* error);

void tcp_write(int sock, char* msg);

#endif // UTILS_H
