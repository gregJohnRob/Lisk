#ifndef GAME_H
#define GAME_H

/* Includes */
#include "Map.h"
#include "Utils.h"


/* Declarations */
#define STATE_INIT            0x0        //Setup
#define STATE_WAITING_PLAYERS 0x1        //Waiting for player
#define STATE_INGAME          0x2        //Playing state


// Player Information Struct
typedef struct Player
{
  short Id;       //Player ID
  int Troops;     //Total number of Troops
  int Areas;      //Total number of Territories owned
} Player_t;

// Game Information Struct
typedef struct Game
{
  short Map;      //ID of Map
  short State;    //Game state
} Game_t;


/* Prototypes */
void gPickMap(void);                    //Picks a map from a selection in Lisk
short gGenId(void);                     //Gives a client a unique ID to use in-game
short gState(void);                     //Returns current state of the game
void gSetState(Game_t *, int, int *);   //Set game's current state

short gTroopsPerTurn(LiskMap_t *, Player_t *);  //Returns the number of troops player gets this turn

#endif
