#ifndef GAME_H
#define GAME_H

/* Includes */
#include "Map.h"
#include "Utils.h"


/* Declarations */
#define STATE_INIT            0x0        //Setup
#define STATE_WAITING_PLAYERS 0x1        //Waiting for player
#define STATE_INGAME          0x2        //Playing state

/* Prototypes */
void gPickMap(void);          //Picks a map from a selection in Lisk
short gGenId(void);           //Gives a client a unique ID to use in-game
short gState(void);           //Returns current state of the game
void gSetState(int, int *);   //Set game's current state
#endif
