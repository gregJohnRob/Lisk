#include "Utils.h"

#if MAP_DEBUG
/* printMap
 *
 * Map DEBUG function to print out the contents of a Map
 */
void printMap(Map_t *Map)
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
