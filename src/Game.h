#ifndef GAME_H
#define GAME_H

/* Includes */
#include "Map.h"
#include "Utils.h"


/* Declarations */
#define STATE_INIT            0x0        //Setup
#define STATE_WAITING_PLAYERS 0x1        //Waiting for player
#define STATE_INGAME          0x2        //Playing state

#define STATE_T_CARD          0x0
#define STATE_T_PLACE         0x1
#define STATE_T_ATTACK        0x2
#define STATE_T_FORTIFY       0x3


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
  short Map;                //ID of Map
  short State;              //Game state
  short CardBonus;          //Bonus for cards
  Player_t CurrentPlayer;   //Current Player of Turn
  short TurnState;          //State of given turn
} Game_t;

//Lisk Card Struct
typedef struct Card
{
  short Id;       //Card ID
  char *Name;     //Card Name
  short Colour;   //Card Colour
} Card_t;


/* Prototypes */
void gPickMap(void);                    //Picks a map from a selection in Lisk
short gGenId(void);                     //Gives a client a unique ID to use in-game
short gState(void);                     //Returns current state of the game
void gSetState(Game_t *, int, int *);   //Set game's current state


short gPlaceTroops(int, int, int);          //Place troops at given Id
short gTroopsPerTurn(int);                  //Returns the number of troops player gets this turn
short gTradeCards(Game_t *, Card_t *, int); //Trades in the number of cards
Msg_t gAttack(struct LiskMap *, int, int);  //Attack Command


#endif
