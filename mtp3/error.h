#ifndef _ERROR_H_
#define _ERROR_H_

#include <stdarg.h>

/*
 * Log types
 */
#define DEBUG 0
#define INFO 1
#define MINOR 2
#define MAJOR 3
#define CRITICAL 4
#define SWERR 5


void log(int, char*, ...);
void errorSysRet(char*, ...);
void errorSysExit(char*, ...);
void errorRet(char*, ...);
void errorExit(char*, ...);
void errorPrint(int, char*, va_list);

#endif
