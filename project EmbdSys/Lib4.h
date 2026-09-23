#ifndef ES_LIB_H
#define ES_LIB_H

#include "TM4C129.h" 
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


// serial functions
extern void ES_Serial(int number, char line_format[20]) ;  // eg "9600,5,N,1" is 9600 baud, 5 bit word, no parity, 1 stop bit
extern char ES_getchar(int number) ;
extern void ES_putchar(int number, char c) ;
extern void ES_printf(const char *format, ...) ;
extern void ES_scanf(const char *format, ...) ;
extern int readDec(void) ; 									// read decimal value from serial port

extern void msDelay(int msec) ;

#endif /* ES_LIB_H */