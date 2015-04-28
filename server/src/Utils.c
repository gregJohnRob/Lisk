#include "Utils.h"

#if MAP_DEBUG
/* printMap
 *
 * Map DEBUG function to print out the contents of a Map
 */
void printMap(LiskMap_t *Map)
{
   char Name[25];
   int Count = 0;
   int iCount = 0;
   strcpy(Name, Map->Name);
   Name[24] = '\0';

  printf("Map Output>:\n Name:%s Image ID: %d\n Node Count: %d\n",
                                                  Name,
                                                  Map->Img,
                                                  Map->NodeCount);

  printf("%s\n", "[Nodes]");

  for(Count = 0; Count < Map->NodeCount; Count++)
  {
    Node_t Node = Map->Nodes[Count];
    char sID[2];
    sID[0] = Node.Id;
    sID[1] = '\0';

    printf("Node (%d)\n Owner: %d\n Troops: %d\n (X,Y): (%d,%d)\n Has %d Edge(s)\n--\n",
                                                  atoi(sID),
                                                  Node.Owner,
                                                  Node.Troops,
                                                  Node.X,
                                                  Node.Y,
                                                  Node.EdgeCount);

    printf("%s\n", "[Edges]");
    for(iCount = 0; iCount < Node.EdgeCount; iCount++)
    {
      printf("->%d\n", Node.Edges[iCount]);

    }

    printf("%s\n", "--");
  }
}
#endif

/* rSeed
 *  Seeds the Random number generator with the current system time.
 */
void rSeed()
{
  srand(time(NULL));
}


/* DiceRoll
 * Returns a number in the range  1->6 as if a dice was being rolled.
 */
unsigned int DiceRoll(void) {
    return 1 + (rand() * 6.0 / RAND_MAX);
}



/* Shuffle
 * Arrange the N elements of ARRAY in random order, only effective if N is
 * less than RAND_MAX
 */
void Shuffle(int *array, size_t n)
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
