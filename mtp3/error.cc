#include        "error.h"
#include	<errno.h>
#include        <stdarg.h>
#include        <stdio.h> 
#include        <stdlib.h>
#include 	<string.h>

#define MAXLINE 1024


void errorPrint(int errnoFlag, char *format, va_list vl)
{
    int	en;
    char	buf[MAXLINE];

    en = errno;	     
    vsprintf(buf, format, vl);
    if (errnoFlag)
    {
    	sprintf(buf+strlen(buf), ": %d %s",en, strerror(en));
    };
    strcat(buf, "\n");
    fflush(stdout);	   
    fputs(buf, stderr);
    fflush(NULL);	       
};


void log(int level, char *format, ...) 
{
   va_list vl;

   /*
    * In here I must add some stuff to add in the level 
    */
   va_start(vl, format);
   errorPrint(0, format, vl);
   va_end(vl);
};    

void errorSysRet(char *format, ...)
{
   va_list		vl;
   va_start(vl, format);
   errorPrint(1, format, vl);
   va_end(vl);
};


void errorSysExit(char *format, ...)
{
    va_list		vl;
    va_start(vl, format);
    errorPrint(1, format, vl);
    va_end(vl);
    exit(1);
};


void errorRet(char *format, ...)
{
    va_list		vl;
    va_start(vl, format);
    errorPrint(0, format, vl);
    va_end(vl);
};


void errorExit(char *format, ...)
{
    va_list		vl;
    va_start(vl, format);
    errorPrint(0, format, vl);
    va_end(vl);
    exit(1);
};


