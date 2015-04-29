#ifndef UTILS_H
#define UTILS_H
/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Server.h"

/* Prototypes */

void uRSeed(void);                   //Seeds the Random Numbers with the current time
unsigned int uDiceRoll(void);        //Rolls a random dice number
void uShuffle(int *, size_t);        //Shuffles a given array
void uSortHigh(unsigned int*, int);  //Insertion sort of array
#endif
