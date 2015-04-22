#ifndef MAP_H
#define MAP_H

/* Includes */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../Server.h"

#if MAP_DEBUG
  #include "../Utils/Utils.h"
#endif

/* Declarations */
#define NPC_ID 0xEE   //ID used if there is an NPC controlled Node on Map

//LISK Map Node
typedef struct Node
{
  char Id;                //Node Id
  char Owner;             //Id of Player who owns it
  short Troops;           //No. of Troops at this node
  short X;                //Gird X location of Node
  short Y;                //Grid Y location of Node
  short EdgeCount;        //Number of Edges
  char *Edges;            //Pointer to Edges
} Node_t;



//LISK Map Description
typedef struct Map
{
  char Name[24];          //Map name
  short Img;               //Associated Image ID of Map
  Node_t *Nodes;          //Pointer to list of all Nodes
  char NodeCount;         //Number of Nodes
} Map_t;

#define CONNECTED     99  //Node Connected value
#define NOT_CONNECTED -1  //Node not connected value


/* Prototypes */
Map_t *mCreate(char *);                 //Creates a lovely new map from file
void mDestory(Map_t *);               //Free memory from Map

short IsConnected(Node_t*, Node_t*);  //Are the two given nodes connected?

#endif
