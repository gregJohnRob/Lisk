#include "Map.h"
/* LISK MAP.C
 *
 * Provides some backend work to do with Lisk Maps
 *
 * Author: Ewan McCartney
 */

/* mCreate
 *
 * Creates a Map struct out of the given Map file as param
 * Returns either a pointer to the Map or NULL if there was an error
 */
LiskMap_t *mCreate(char* Filename)
{
  //char Buffer[256];     //Buffer for Connections
  const int LINE_MAX = 1024;  //Maximum line size
  char Line[LINE_MAX];  //Temp store for Line
  int Count = 0;
  FILE *mFile = fopen(Filename, "r");
  LiskMap_t *Map = (LiskMap_t *) calloc(1, sizeof(LiskMap_t));

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
void mDestory(LiskMap_t *Map) { free(Map); }




/* mIsConnected
 * Check if two given nodes in a Map are connected or not.
 *
 * Based on Simple pathfinding:
 *
 *  We start at the beginning (A)
 *      Add it to the DoneSet
 *      Check all it's connections to see if they are equal to (B)
 *        If so - return
 *        Else - Add those connections to ToDoSet
 *      Loop back until ToDoSet is empty
 *
 */
short mIsConnected(LiskMap_t *Map, short A, short B)
{
  //First get Start Node
  Node_t Start = Map->Nodes[mGetNode(Map, A)];

  //Setup the 2 linked lists we need.
  LList_t ToDoSet; LList_t *pToDoSet = &ToDoSet;
  LList_t DoneSet; LList_t *pDoneSet = &DoneSet;
  ToDoSet.Size = 0; ToDoSet.Start = NULL;
  DoneSet.Size = 0; DoneSet.Start = NULL;

  //Add starting Node
  lAdd(pToDoSet, Start.Id);

  //Keep looping while we have nodes to check.
  while(ToDoSet.Size > 0)
  {
	   short Value = lRemove(pToDoSet);
     Node_t Node = Map->Nodes[mGetNode(Map, Value)];
     int i = 0;
	   lAdd(&DoneSet, Node.Id);

     //CHecking each other the Edges on this node
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

/* CanAttack
 *  Checks if 2 adjacent nodes can attack each other
 *  Checks ID and also the Owner of the Node(s)
 *    Returns CONNECTED if they can attack or NOT_CONNECTED otherwise
 */
short mCanAttack(LiskMap_t *Map, short A, short B)
{
    Node_t Node = Map->Nodes[mGetNode(Map, A)];
    Node_t NodeB = Map->Nodes[mGetNode(Map,B)];
    int i = 0;
    for(i = 0; i < Node.EdgeCount; i++)
    {
      if (Node.Edges[i] == A)
      {
         if (Node.Owner != NodeB.Owner) { return CONNECTED; }
      }
    }
    return NOT_CONNECTED;
}


/* CanMoveUnits
 *  Checks if you can move Units between the 2 given nodes
 *  Similar to mIsConnected but checks ownership of the Node as well
 *  Returns CONNECTED if they are linked, or NOT_CONNECTED if not
 */
short mCanMoveUnits(LiskMap_t* Map, short A, short B)
{
  //First get Start Node
  Node_t Start = Map->Nodes[mGetNode(Map, A)];

  //Setup the 2 linked lists we need.
  LList_t ToDoSet; LList_t *pToDoSet = &ToDoSet;
  LList_t DoneSet; LList_t *pDoneSet = &DoneSet;
  ToDoSet.Size = 0; ToDoSet.Start = NULL;
  DoneSet.Size = 0; DoneSet.Start = NULL;

  //Add starting Node
  lAdd(pToDoSet, Start.Id);

  //Keep looping while we have nodes to check.
  while(ToDoSet.Size > 0)
  {
	   short Value = lRemove(pToDoSet);
     Node_t Node = Map->Nodes[mGetNode(Map, Value)];
     int i = 0;
	   lAdd(&DoneSet, Node.Id);

     //CHecking each other the Edges on this node
     for(i = 0; i < Node.EdgeCount; i++)
     {
       short Location = mGetNode(Map, Node.Edges[i]);

       if(Map->Nodes[Location].Id == B) { return CONNECTED; }

       //Added extra check for if the nodes are owned by the player
       if((IsInSet(pDoneSet, Map->Nodes[Location].Id) == 0) && ( Map->Nodes[Location].Owner == Start.Owner))
       {
         lAdd(pToDoSet, Map->Nodes[Location].Id);
       }
     }
  }
  return NOT_CONNECTED;
}


/* mPopulate
 *    Populates the given Map at the start of a game.
 *    Specify the number of players and the number of nodes/player
 *
 */
void mPopulate(LiskMap_t *Map, short Players, short Nodes)
{
    short NodesPerPlayer = Nodes/Players;
    short GivenNodes = 0;
    int NodeIds[128];
    int i = 0;
    int j = 0;

    //Fill up Ids into buffer then shuffle it for population
    for(i = 0; i < Map->NodeCount; i++) { NodeIds[i] = i; }
    Shuffle(&NodeIds[0], Map->NodeCount);

    //Setting up all player locations
    while( GivenNodes < NodesPerPlayer)
    {
      for(i = 0; i < Players; i++)
      {
        short Location = mGetNode(Map, NodeIds[j]);
        Map->Nodes[Location].Owner = i;
        Map->Nodes[Location].Troops = STARTING_TROOPS;
        j++;
      }
      GivenNodes++;
    }

    //Filling out any of the rest as NPCs
    while ( j <= Map->NodeCount)
    {
      short Location = mGetNode(Map, NodeIds[j]);
      Map->Nodes[Location].Owner = NPC_ID;
      Map->Nodes[Location].Troops = STARTING_TROOPS;
    }
}






/* GetNode
 *  Finds a node in a Map by it's Id
 * Returns position if found, else -1
 */
short mGetNode(LiskMap_t *Map, short N)
{
  int i = 0;
  for(i =0; i < Map->NodeCount; i++)
  {
    if (Map->Nodes[i].Id == N) { return i; }
  }
  return -1;
}


/* IsInSet
 * Performs a Linear search on the given Linked List to find the given node
 * Returns 1 if found or 0 otherwise
 */
short IsInSet(LList_t *Set, short V)
{
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
 * Returns element at the head of the list
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
