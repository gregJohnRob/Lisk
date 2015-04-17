#ifndef MAP_H
#define MAP_H

/* Includes */

/* Declarations */
#define NPC_ID 0xEE   //ID used if there is an NPC controlled Node on Map

//LISK Map Node
typedef struct Node
{
  char Id;            //Node Id
  char Owner;         //Id of Player who owns it
  short Troops;       //No. of Troops at this node
  short X;            //Gird X location of Node
  short Y;            //Grid Y location of Node
} Node_t;

//LISK Map Edge
typedef struct Edge
{
  char IdA;           //Node (A) connected to (B)
  char IdB;           //Node (B) connected to (A)
} Edge_t;


//LISK Map Description
typedef struct Map
{
  char Name[24];      //Map name
  char Img;           //Associated Image ID of Map
  Edge_t *Edges;      //Pointer to array of Edges
  Node_t *Nodes;      //Pointer to array of Nodes
} Map_t;



/* Prototypes */


#endif
