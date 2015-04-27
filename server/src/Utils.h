#ifndef UTILS_H
#define UTILS_H
/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Server.h"

#if MAP_DEBUG
  #include "Map.h"
#endif


/* Prototypes */
#if MAP_DEBUG
  void printMap(Map_t *);
#endif


void rSeed(void);             //Seeds the Random Numbers with the current time
unsigned int DiveRoll(void);  //Rolls a random dice number
void Shuffle(int *, size_t);  //Shuffles a given array
#endif
