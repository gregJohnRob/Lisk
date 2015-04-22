#ifndef UTILS_H
#define UTILS_H
/* Includes */
#include <stdio.h>

#include "../Server.h"

#if MAP_DEBUG
  #include "../Game/Map.h"
#endif


/* Prototypes */
#if MAP_DEBUG
  void printMap(Map_t *);
#endif

#endif
