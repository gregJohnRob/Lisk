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

  fgets(Line, LINE_MAX, mFile);   //Read over NULL #!Nodes Line

  for(Count = 0; Count < Map->NodeCount && fgets(Line, LINE_MAX, mFile) != NULL; Count++)
  {
    Node_t Node = Map->Nodes[Count];
    short NodeEdges = 0;
    sscanf(Line, "%hd %hd %hd %hd", &Node.Id,
                                   &Node.X,
                                   &Node.Y,
                                   &NodeEdges);

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

/* mIsConnected
 * Check if two given nodes in a Map are connected or not.
 */
short mIsConnected(Map_t *Map, short A, short B)
{
  //First get Start Node
  Node_t Start = Map->Nodes[mGetNode(Map, A)];
  LList_t ToDoSet; LList_t *pToDoSet = &ToDoSet;
  LList_t DoneSet; LList_t *pDoneSet = &DoneSet;

  ToDoSet.Size = 0; ToDoSet.Start = NULL;
  DoneSet.Size = 0; DoneSet.Start = NULL;

  lAdd(pToDoSet, Start.Id);

  while(ToDoSet.Size > 0)
  {
	short Value = lRemove(pToDoSet);
    Node_t Node = Map->Nodes[mGetNode(Map, Value)];
    int i = 0;
	lAdd(&DoneSet, Node.Id);


    for(i = 0; i < Node.EdgeCount; i++)
    {
       short Location = mGetNode(Map, Node.Edges[i]);

       if(Map->Nodes[Location].Id == B) { return CONNECTED; }

       if(IsInSet(pDoneSet, Map->Nodes[Location].Id) == 0)
       {
         lAdd(pToDoSet, Map->Nodes[Location].Id);
       }
    }

  }

  return NOT_CONNECTED;
}

/* GetNode
 *  Finds a node in a Map by it's Id
 */
short mGetNode(Map_t *Map, short N)
{
  int i = 0;
  for(i =0; i < Map->NodeCount; i++)
  {
    if (Map->Nodes[i].Id == N) { return i; }
  }
  return -1;
}


//Returns 1 if the value is already in the set
short IsInSet(LList_t *Set, short V)
{
  int i = 0;
  LLNode_t *Node = Set->Start;
  while(Node != NULL)
  {
	  if (Node->Element == V) { return 1; }
	  Node = Node ->Next;
  }
  return 0;
}


/* LAdd
 * Adds item to Linked list, updating pointers and size
 */
void lAdd(LList_t *List, short Element)
{
	LLNode_t *Node = (LLNode_t *) calloc(1, sizeof(LLNode_t));
	Node->Element = Element;
	Node->Next = NULL;

	LLNode_t *Start = List->Start;
  List->Start = Node;
  Node->Next = Start;

	List->Size++;
}

/* LRemove
 * Removes item from Linked list, remembering to free memory and update pointers
 */
short lRemove(LList_t *List)
{
	LLNode_t *Element = List->Start;
	List->Start = Element->Next;
	List->Size--;

	short value = Element->Element;
	free(Element);

	return value;
}
