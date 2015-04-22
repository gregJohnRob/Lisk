#include "Map.h"
/* LISK MAP.C
 *
 * Provides some backend work to do with Lisk Maps
 * Author: Ewan McCartney
 */

/* mCreate
 *
 * Creates a Map struct out of the given Map file as param
 * Returns either a pointer to the Map or NULL if there was an error
 *
 * TODO Actually load in the Nodes
 */
Map_t *mCreate(char* Filename)
{
  //char Buffer[256];     //Buffer for Connections
  const int LINE_MAX = 1024;  //Maximum line size
  char Line[LINE_MAX];  //Temp store for Line
  int Count = 0;
  FILE *mFile = fopen(Filename, "r");
  Map_t *Map = (Map_t *) calloc(1, sizeof(Map_t));

  //Error checking the file was opened right
  if(NULL == mFile) { printf("%s\n", "Map->mCreate: Failed to open map file."); return NULL; }

  //Fill out generic struct details
  for(Count = 0; Count < 3 && fgets(Line, LINE_MAX, mFile) != NULL; Count++)
  {
    switch(Count % 3)
    {
      //Copy Name
      case 0:
      {
        strcpy(Map->Name, Line);
        break;
      }

      //Assign Image Code
      case 1:
      {
        Map->Img = atoi(Line);
        break;
      }

      //Number of Nodes
      case 2:
      {
        Map->NodeCount = atoi(Line);
        break;
      }

      //Oh-Oh we have an error
      default: printf("%s Step:(%d)\n", "Map->mCreate: Failed to read input file." , Count);
    }
  }
  if (Count != 3) { return NULL; }

  //Generate Number of Nodes
  Node_t Nodes[Map->NodeCount];
  Node_t *pNodes = (Node_t *) calloc(1, sizeof(Nodes));
  Map->Nodes = pNodes;

  fgets(Line, LINE_MAX, mFile);   //Read over NULL #!Nodes Line

  for(Count = 0; Count < Map->NodeCount && fgets(Line, LINE_MAX, mFile) != NULL; Count++)
  {
    Node_t Node = Map->Nodes[Count];
    short NodeEdges = 0;
    sscanf(Line, "%c %hd %hd %hd", &Node.Id,
                                   &Node.X,
                                   &Node.Y,
                                   &NodeEdges);

    char Edges[NodeEdges];
    Node.Edges = (char *) calloc(1, sizeof(Edges));

    Map->Nodes[Count] = Node;
  }

  fgets(Line, LINE_MAX, mFile);   //Read over NULL #!Edges Line

  //Load in the Edges within the Map and stores them in the correct Nodes
  while(fgets(Line, LINE_MAX, mFile) != NULL)
  {
      short nId = 0;
      short nCon = 0;
      sscanf(Line, "%hd %hd",&nId, &nCon);

      Map->Nodes[nId].Edges[Map->Nodes[nId].EdgeCount] = nCon;
      Map->Nodes[nId].EdgeCount++;
  }

  fclose(mFile);
  return Map;
}

//Frees all memory associated with the Map!
void mDestory(Map_t *Map) { free(Map); }
