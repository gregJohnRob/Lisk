#include "Utils.h"

/* rSeed
 *  Seeds the Random number generator with the current system time.
 */
void uRSeed()
{
  srand(time(NULL));
}


/* DiceRoll
 * Returns a number in the range  1->6 as if a dice was being rolled.
 */
unsigned int uDiceRoll(void) {
    return 1 + (rand() * 6.0 / RAND_MAX);
}



/* Shuffle
 * Arrange the N elements of ARRAY in random order, only effective if N is
 * less than RAND_MAX
 */
void uShuffle(int *array, size_t n)
{
    if (n > 1)
    {
       size_t i;
	     for (i = 0; i < n - 1; i++)
       {
	        size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
	        int t = array[j];
	        array[j] = array[i];
	        array[i] = t;
	     }
    }
}


/* Sort Highest
 *  Simple insertion sort on a given array */
void uSortHigh(unsigned int *array, int size)
{
  int i = 0;
  int j = 0;

  for(int i = 0; i < size; i++)
  {
    unsigned int m = i;
    unsigned int t = 0;

    for(j = i; i < size; j++)
    {
      if (array[j] > array[m]) { m = j; }
    }

    t = array[i];
    array[i] = array[m];
    array[m] = t;
  }
}
