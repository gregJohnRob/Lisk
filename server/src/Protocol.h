#ifndef PROTOCOL_H
#define PROTOCOL_H
/* Defines the entire LISK Protocol message values */

/* Client Incomming
 *    All messages we should expect to recieve from Client
 */

/* General Server Commands */
#define CODE_SERV         0x0   //Server message code for all SERV_ commands

#define SERV_GAMELIST     0x0   //Fetch Gamelist
#define SERV_PLAYERINFO   0x1   //Fetch Player Info
#define SERV_GAMEINFO     0x2   //Fetch Game Info (Players, Rounds, Time) NOTE: May join A
#define SERV_GAME         0x3   //Fetch Game (Map ID / Placements)        NOTE: May join A
#define SERV_JOIN         0x4   //Join a Game
#define SERV_LEAVE        0x5   //Leave Game

/* Admin Commands */
#define ADMIN_CODE        0x1   //Special Admin code, reserved if required


/* Game Specific Commands */
#define GAME_CODE         0x2   //Server message code for all GAME_ commands

#define GAME_START        0x0   //Start Turn
#define GAME_END          0x1   //End Turn
#define GAME_P1           0x2   //Start Phase1 -> Cards
#define GAME_P2           0x3   //Start Phase2 -> Placement
#define GAME_P3           0x4   //Start Phase3 -> Attack
#define GAME_P4           0x5   //Start Phase4 -> Fortify
#define GAME_PLACE        0x6   //Place Troops
#define GAME_ATTACK       0x7   //Attack
#define GAME_FORTIFY      0x8   //Fortify
#define GAME_TRADE        0x9   //Trade in cards
#define GAME_INFO         0xA   //Returns all game info (Current round, ID, Player's Turn)
#define GAME_TIME         0xB   //Returns Target time for move/turn


/* Server Return Values */
#define CODE_STATUS       0x0   //Server message code for returning STATUS_ OK/FAIL/UNKNOWN

#define STATUS_OK         0x0   //Return OK
#define STATUS_FAIL       0x1   //Return FAIL
#define STATUS_UNKNOWN    0x2   //Return Unknown
#define STATUS_FULL       0x3   //Return Server is under FULL CAPACITY
#define STATUS_STATE      0x4   //Notifies of a state change
#define STATUS_ERROR      0xF   //An error occured in packet. You need to resend.

#define CODE_DATA         0x1   //Server message code for returning DATA_
#define CODE_NODATA       0x2   //Server message code to say no DATA_ has been returned


/* Data Type Values */
#define DATA_ID           0x21   //Client ID number
#define DATA_VERSION      0x22   //LISK Version Number
#define DATA_STATE        0x23   //Current Game State


//Special Message End Constant
#define MSG_END           0xEE  //Plonk at end of each message


/* Declarations */

//Defines a message from Clients
typedef struct Msg
{
  short Code;            //Protocol code
  short Op;              //Operation code
  short Data[128];        //Data from message
  short DataSize;        //Number of bits of data to send
} Msg_t;

#endif
