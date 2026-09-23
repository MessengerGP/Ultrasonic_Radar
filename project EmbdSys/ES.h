#ifndef ES_H
#define ES_H

#include "TM4C129.h" 
#include <stdint.h>
#include <stdbool.h>
#include <string.h>


// Clock Frequency functions
extern int ES_setSystemClockFrequency(uint32_t freqMHz);					// Sets the system clock to the speoied frequency in MHz eg 80 is used to represent 80000000Hz. Allowed frequencies in MHz are: 120, 100, 80, 60, 50, 40, 30, 25, 20, 16
extern uint32_t ES_getSystemClockFrequency(void) ;								// Reads the system clock frequency and returns the frequency in Hertz eg 80000000 represents 80Hz

// serial functions 
extern void ES_Serial(int UARTnumber, char line_format[20]) ; 		// configures UARTx to the format speciifed in the format string ( "baudrate, parity, wordlength, stopbits" )
extern char ES_getchar(int UARTnumber);														// blocking. Reads a character from UARTx with echo
extern void ES_putchar(int UARTnumber, char c) ;									// blocking. Writes the specified character to UARTx 
extern void ES_readLine(int UARTnumber, char *buf, int maxLen) ;  // blocking. Reads a line from UARTx with echo, backspace / delete editing and new line termination of entry.
extern void ES_printf(int UARTnumber,const char *format, ...) ;		// blocking. Writes formatted text to UARTx with the same syntax as fprint eg ES_printf(UART,Format String, arguments)
extern void ES_scanf(int UARTnumber,const char *format, ...) ;		// blocking. reads and echoes formatted text from UARTx with the same syntax as fscanf eg ES_scanf(UART,Format String, arguments)

// blocking delay functions
extern void ES_startDelayTimer(void);															// starts the Delay timer (Timer7) that ES_msDelay(), ES_usDelay(), ES_sleep() use. Use after changes to the system clock setting. Must be run before any of the Delay() or sleeps() functions.
extern void ES_msDelay(uint32_t milliseconds);										// blocking. Waits the specified number of milliseconds before continuing. eg ES_msDelay(500) waits 500ms ~ 1/2 second
extern void ES_usDelay(uint32_t microseconds) ;										// blocking. Waits the specified number of microseconds before continuing. eg ES_usDelay(500) waits 500us ~ 1/2 millisecond
extern void ES_sleep(uint32_t seconds) ;													// blocking. Waits the specified number of seconds before continuing. eg ES_usDelay(500) waits 500us ~ 1/2 millisecond


#define CONSOLE 0																									// This represents UART0 when used as the UARTnumber argument. (Tiva USB serial connection to PC is UART 0)


#endif /* ES_H */


