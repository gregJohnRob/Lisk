#include "Utils.h"

#if MAP_DEBUG
void printMap(Map_t *Map)
{
   char Name[25];
   strcpy(Name, Map->Name);
   Name[24] = '\0';



  printf("Map Output>:\n Name:%s Image ID: %d\n Node Count: %d\n",
                                                  Name,
                                                  Map->Img,
                                                  Map->NodeCount);
}
#endif
