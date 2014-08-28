/***********************************************************************************
File name: buffer.c
Author: James Freill, ID# 040590657
Course: CST8152 - Compilers
Assignment: Assignment 1
Date: January 29th 2013
Professor: Sv. Ranev
Purpose: Complete a "buffer utitity", which includes the buffer data structure and
         the following functions, following strictly given specififications. Using
         the given data declarations and function prototypes, without any
         modification to them, or their names. Along with actively asserting a
         "excessively defensive programming" ability to help maintain the bonderies
         of the program within the allocated memory, with no creation of memory leaks.
Function list: Buffer * b_create (), Buffer * ca_addc (), b_reset (), b_destroy(),
               ca_isfull(), ca_getsize(), ca_getcapacity(), ca_setmark(),
               ca_getmark(), ca_getmode(), Buffer *capack(), ca_print(), ca_load()
*************************************************************************************/
/************************************************************************************
****************************		INCLUDE FILES		*****************************
************************************************************************************/
#include "buffer.h"
/************************************************************************************
Purpose: Creates a new buffer in memory(on the heap), after checking for abnormalities
Author: James Freill
History/Versions: 1.0 - 01/19/2013
Called functions: calloc();, malloc();
Parameters: int init_capacity, char inc_factor, char o_mode, or null
Return value: memory address of the buffer on the heap
Algorithm: allocates mem. for sizeof buffer struct. && gives it a pointer.
		   allocates mem. for char * in struct to hold char array
		   sets mode
		   sets increment factor
************************************************************************************/
Buffer * b_create(int init_capacity, char inc_factor,char o_mode){

	int num_elements = 1;
	Buffer *pB;

	if(init_capacity < 0) return NULL;
	if(o_mode != 'a' && o_mode != 'f' && o_mode != 'm') return NULL;

	pB = (Buffer *)calloc(num_elements, sizeof(Buffer));
	if(pB == NULL && init_capacity > 0) return NULL;

	pB->ca_head = (char *)malloc(init_capacity * (sizeof(char)));

	if(pB->ca_head == NULL && init_capacity > 0){
		free(pB);
		return NULL;
	}

	if(o_mode == 'f' || inc_factor == BUF_SIZE_ZERO){
		if(init_capacity <= 0){
			free(pB->ca_head);
			free(pB);
			return NULL;
		}
		pB->mode = pB->inc_factor = 0;
	}else if(o_mode == 'a'){
		pB->mode = 1;
		pB->inc_factor = inc_factor;
	}else if(o_mode == 'm' && (inc_factor >= 1 && inc_factor <= 100)){
		pB->mode = -1;
		pB->inc_factor = inc_factor;
	}else{
		free(pB->ca_head);
		free(pB);
		return NULL;
	}

	pB->capacity = init_capacity;
	pB->addc_offset=pB->mark_offset=pB->r_flag=0;

	return pB;
}
/************************************************************************************
Purpose: Resets r_flag to 0, tries to add char symbol to char array of the given Buffer
		 pointed by pB. If Buffer is operational, its added and stored in char Buffer.
		 If the Buffer is full, it will try to resize the buffer based on the operation
		 mode of the buffer.
Author: James Freill
History/Versions: 1.0 - 01/19/2013
Called functions: ca_isfull();, realloc(), sizeof();
Parameters: Buffer *pB, char symbol.
Return value: returns the address of the struct where memory has been created for the symbol
Algorithm: check for NULL pointer, and mode value
		   if mode is fixed then NULL is returned
		   if capacity is not full assign "symbol" to char array
		   set mode
		   set new capacity
		   set flag value
		   set increment factor
		   return buffer pointer address
************************************************************************************/
Buffer * ca_addc (Buffer *pB, char symbol)
{
    int nCap ;
    int aSpace;
    double nInc;
    char* n_caHead;
    if (pB == NULL)
        return NULL;

    if (pB->addc_offset*sizeof(char) == INT_MAX)      /* if there is no character */
        return NULL;
    pB->r_flag = 0;

    if ((signed)(pB->addc_offset*sizeof(char)) < pB->capacity){    /*Checks the buffer size*/
        *(pB->ca_head + pB->addc_offset) = symbol; /*sets the buffer character   */
        pB->addc_offset++;  /*increments the offset of the buffer   */
        return pB;
    }

    switch (pB->mode){
        case 1:
            nCap = pB->capacity + ((unsigned char)pB->inc_factor * sizeof(char));
            if (nCap < 0) /*checks capacity*/
                return NULL;
            break;
        case -1:
           aSpace = INT_MAX - pB->capacity;
           nInc = (double)aSpace * (double)((unsigned char)pB->inc_factor/100.0);
            if ((int)nInc == 0){
                nCap = INT_MAX;
                break;
            }
            nCap = (int)(nInc + pB->capacity);
            break;
        default:
            return NULL;
    }

    n_caHead=(char*)realloc(pB->ca_head, nCap);
    if(n_caHead == NULL)
        return NULL;
	if(n_caHead != pB->ca_head){
        pB->r_flag= SET_R_FLAG; /*sets the flag of the buffer*/
        pB->ca_head=n_caHead; /*assigns the array the buffer to new character */
    }

    pB->ca_head[pB->addc_offset] = symbol; /* assign the character to the new allocated buffer array */
    ++pB->addc_offset;   /*increments offset     */
    pB->capacity = nCap;  /*sets new capacity  */

    return pB;
}
/************************************************************************************
Purpose: The function retains the memory space allocated to the buffer, but reinitializes
		 all appropriate members.
Author: James Freill
History/Versions: 1.0 - 01/19/2013
Called functions: none
Parameters: Buffer *pB.
Return value: -1 if address is NULL, 0 if PASS
Algorithm: none
************************************************************************************/
int b_reset( Buffer *pB ){
	if(pB == NULL)return R_FAIL_1;
	pB->addc_offset = 0;
	pB->mark_offset = 0;
	pB->r_flag = 0;
	return 0;
}
/************************************************************************************
Purpose: The function deallocates the memory occupied by the character buffer the structure
Author: James Freill
History/Versions: 1.0 - 01/19/2013
Called functions: free();
Parameters: Buffer *pB;
Return value: none
Algorithm: none
************************************************************************************/
void b_destroy(Buffer *pB){
	if(pB != NULL){
		if(pB->ca_head != NULL)
			free(pB->ca_head);
		free(pB);
	}
}
/************************************************************************************
Purpose: The function determines if the character buffer is full
Author: James Freill
History/Versions: 1.0 - 01/19/2013
Called functions: none
Parameters: Buffer *pB;
Return value: -1 if NULL, 1 if capacity == offset, 0 if no conditions apply
Algorithm: none
************************************************************************************/
char ca_isfull(Buffer *pB){
	if(pB == NULL)return R_FAIL_1;
	if(pB->addc_offset == pB->capacity) return SET_R_FLAG;
	return 0;
}
/************************************************************************************
Purpose: The function returns the current size of the character buffer. The size is
		 measured in chars, that are currently being used/added.
Author: James Freill
History/Versions: 1.0 - 01/19/2013
Called functions: none
Parameters: Buffer *pB
Return value: -1 if NULL, OR "pB->addc_offset" == sizeof BUFFER
Algorithm: none
************************************************************************************/
int ca_getsize(Buffer *pB){
	if(pB == NULL)return R_FAIL_1;
	return pB->addc_offset;
}
/************************************************************************************
Purpose: The function returns the current capacity of the buffer.
Author: James Freill
History/Versions: 1.0
Called functions: none
Parameters: Buffer *pB;
Return value: -1 if NULL, OR "pB->capacity"
Algorithm: none
************************************************************************************/
int ca_getcapacity(Buffer *pB){
	if(pB == NULL)return R_FAIL_1;
	return pB->capacity;
}
/************************************************************************************
Purpose: The function sets mark_offset to mark. The mark must be with the current cap.
Author: James Freill
History/Versions: 1.0 - 01/19/2013
Called functions: sizeof();
Parameters: Buffer *pB, int mark;
Return value: -1 if NULL, OR "mark_offset" of the BUFFER
Algorithm: none
************************************************************************************/
int ca_setmark(Buffer *pB, int mark){

	if(pB == NULL)return R_FAIL_1;

	if(mark <0 || (mark*sizeof(char)) > pB->capacity)return R_FAIL_1;

	pB->mark_offset = mark;
	return mark;

}
/************************************************************************************
Purpose: The function returns mark_offset to calling function.
Author: James Freill
History/Versions: 1.0 - 01/19/2013
Called functions: none
Parameters: Buffer *pB;
Return value: -1 if NULL, OR "pB->mark_offset" outlining the offset if BUFFER exits
Algorithm: none
************************************************************************************/
int ca_getmark(Buffer *pB){
	if(pB == NULL)return R_FAIL_1;
	return pB->mark_offset;
}
/************************************************************************************
Purpose: The function returns the mode to the calling function.
Author: James Freill
History/Versions: 1.0 - 01/19/2013
Called functions: none
Parameters: Buffer *pB;
Return value: -1 if NULL, OR "pB->mode" mode of the BUFFER
Algorithm: none
************************************************************************************/
int ca_getmode(Buffer *pB){
	if(pB == NULL)return R_FAIL_1;
	return pB->mode;
}
/************************************************************************************
Purpose: For all operational modes of the buffer the function shrinks or expands the
		 buffer to a new capacity. The new capacity is the current size plus a space
		 for one more char. It also realloc()'s to adjust for the new capacity, and
		 updates all necessary members of the buffer structure.
Author: James Freill
History/Versions: 1.0 - 01/19/2013
Called functions: realloc();, sizeof();
Parameters: Buffer *pB
Return value: -1 if NULL, OR "pB" address of BUFFER
Algorithm: if expands, realloc()'s mem. of char array
		   copies char's into new realloc() space
		   set flag value
		   set the capacity
************************************************************************************/
Buffer *ca_pack( Buffer *pB ){

	char *pBtmp;
	int new_capacity;

	if(pB == NULL)return NULL;
	if(pB->addc_offset == INT_MAX)return NULL;

	pB->r_flag = 0;
	new_capacity = sizeof(char)*(pB->addc_offset +1);

	pBtmp = (char *) realloc (pB->ca_head, new_capacity);
	if(pBtmp == NULL)return NULL;

	if(pBtmp != pB->ca_head){
		pB->r_flag = SET_R_FLAG;
		pB->ca_head = pBtmp;
	}

	pB->capacity = new_capacity;
	return pB;
}
/************************************************************************************
Purpose: Using the printf(), library function the function prints character by character
		 the contents of the character buffer to the standard output. Also used for
		 diagnostic purposes, it returns the number of characters displayed.
Author: James Freill
History/Versions: 1.0 - 01/19/2013
Called functions: none
Parameters: Buffer *pB;
Return value: -1 if NULL/FAIL, OR num of char's in array
Algorithm: read char's from char arrary
		   print char by char
************************************************************************************/
int ca_print(Buffer *pB){

	int i = 0;
	int size = pB->addc_offset/sizeof(char);

	if(pB == NULL)return R_FAIL_1;
	for(i = 0;i < size; i++){
		printf("%c", pB->ca_head[i]);
	}
	printf("\n");

	return i;
}
/************************************************************************************
Purpose: The function reads an open input file specified by the fi into a buffer specified
		 by the pB. The function must use the standard function fgetc(fi) to read one
		 character at a time and the function ca_addc() to add the character to the buffer.
		 Also detecting if there is any failure to load the character buffer, or if some
		 other operation fails. Completes once EOF is returned, without storing EOF to
		 character buffer.
Author: James Freill
History/Versions: 1.0 - 01/19/2013
Called functions: fgetc();, ca_addc();
Parameters: FILE *fi,Buffer *pB
Return value: -1 if NULL/FAIL, OR num of char's read in
Algorithm: read char by char from file
		   check if EOF
		   if !EOF pass to ca_addc(), with BUFFER pointer and char
		   increment num of chars read in
************************************************************************************/
int ca_load(FILE *fi,Buffer *pB){

	int i = 0;
	char c;

	if(pB == NULL || fi == NULL)return R_FAIL_1;

	while(1){
		c = (unsigned char)fgetc(fi);
		if(feof(fi))break;
		if(ca_addc(pB, c) == NULL)return LOAD_FAIL;
		i++;
	}

	return i;
}
