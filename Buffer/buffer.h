/*************************************************************************************************************************************************************************************
File name: buffer.h
Author: James Freille, ID# 040590657
Course: CST8152 - Compilers
Assignment: Assignment 1
Date: January 29th 2013
Professor: Sv. Ranev
Purpose: Complete a "buffer utitity", which includes the buffer data structure and the following functions, following strictly given specififications. Using the given data
	      declarations and function prototypes, without any modification to them, or their names. Along with actively asserting a "excessively defensive programming" ability to
		  help maintain the bonderies of the program within the allocated memory, with no creation of memory leaks.
Function list: [Buffer * b_create (), Buffer * ca_addc (), b_reset (), b_destroy(), ca_isfull(), ca_getsize(), ca_getcapacity(), ca_setmark(), ca_getmark(), ca_getmode(),
			   Buffer *capack(), ca_print(), ca_load()]
*************************************************************************************************************************************************************************************/
/* standard header files */
#define BUFFER_H_
#include <stdlib.h>
#include <stdio.h>/* standard input/output */
#include <malloc.h>/* for dynamic memory allocation*/
#include <limits.h>/* implementation-defined data type ranges and limits */
#include <string.h>
#include <errno.h>
/* constant definitions */
#define FIXED 'f' /* defines the flag f for fixed */
#define ADDITIVE 'a'/* defines the flag a for fixed */
#define MULTI 'm'/* defines the flag m for fixed */
#define BUF_SIZE_HUND 100
#define BUF_SIZE_ZERO 0
#define BUF_SIZE_MAX INT_MAX
#define BUF_SIZE_MIN 1
#define R_FAIL_1 -1
#define R_FAIL_2 -2
#define LOAD_FAIL -2
#define SET_R_FLAG 1
/**************************************************************************************************
********************************		STRUCT BUFFER DEFINE	  *********************************
**************************************************************************************************/
typedef struct BufferDescriptor {
       char * ca_head;      /* pointer to the beginning of character array (character buffer) */
       int capacity;        /* current dynamic memory size (in bytes) allocated to character buffer */
       char inc_factor;      /* character array increment factor */
       int addc_offset;     /* the offset (in char elements) to the add-character location */
       int mark_offset;     /* the offset (in chars elements) to the mark location */
       char r_flag;          /* reallocation flag */
       char mode;            /* operational mode indicator*/
   }Buffer;
/**************************************************************************************************
********************************		FUNCTION PROTOTYPES			*******************************
**************************************************************************************************/
Buffer * b_create ( int init_capacity, char inc_factor,char o_mode );
Buffer * ca_addc ( Buffer *pB, char symbol );
int b_reset ( Buffer *pB );
void b_destroy (Buffer *pB);
char ca_isfull (Buffer *pB);
int ca_getsize (Buffer *pB);
int ca_getcapacity (Buffer *pB);
int ca_setmark (Buffer *pB, int mark);
int ca_getmark (Buffer *pB);
int ca_getmode (Buffer *pB);
Buffer *ca_pack( Buffer *pB );
int ca_print (Buffer *pB);
int ca_load (FILE *fi,Buffer *pB);
