#ifndef MAP_HEADER_G
#define MAP_HEADER_G

/* Includes */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Utils.h"


/* Declarations */
#define NPC_ID 0xEE   //ID used if there is an NPC controlled Node on Map

//LISK CARD COLOURS
#define CARD_RED              0x1
#define CARD_BLUE             0x2
#define CARD_GREEN            0x3
#define CARD_YELLOW           0x4
#define CARD_BLACK            0x0

//LISK Map Node
typedef struct Node
{
  short Id;               //Node Id
  short Owner;            //Id of Player who owns it
  short Troops;           //No. of Troops at this node
  short X;                //Gird X location of Node
  short Y;                //Grid Y location of Node
  short Colour;           //Node Colour
  short EdgeCount;        //Number of Edges
  short Edges[128];       //Pointer to Edges
} Node_t;


//LISK Map Description
typedef struct LiskMap
{
  char Name[24];          //Map name
  short Img;               //Associated Image ID of Map
  struct Node Nodes[128];          //Pointer to list of all Nodes
  short NodeCount;         //Number of Nodes
} LiskMap_t;

//LISK Linked List Node, when pathfinding between nodes
typedef struct LLNode
{
  struct LLNode *Next;
  short Element;
} LLNode_t;


//LISK Linked List, used when pathfinding between nodes
typedef struct LList
{
  LLNode_t *Start;
  short Size;
} LList_t;


#define CONNECTED     99  //Node Connected value
#define NOT_CONNECTED -1  //Node not connected value

#define STARTING_TROOPS 3 //Number of starting troops


/* Prototypes */
LiskMap_t *mCreate(char *);                   //Creates a lovely new map from file
void mDestory(LiskMap_t *);                   //Free memory from Map

short mIsConnected(LiskMap_t*, short, short); //Are the two given nodes connected?
short mGetNode(LiskMap_t*, short);            //Fetches a node by it's ID.
short IsInSet(LList_t *, short);          //Is the value in a set

short mCanAttack(LiskMap_t*, short, short);    //Checks if 2 nodes can attack each other
short mCanMoveUnits(LiskMap_t*, short, short); //Checks if one can move units between 2 nodes

void mPopulate(LiskMap_t *, short, short);     //Populates the given Map at the start of a game


//Linked List Functions
void lAdd(LList_t *, short);              //Add item to Linked List
short lRemove(LList_t *);                 //Remove Item from Linked list

#endif
