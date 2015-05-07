#ifndef CLIENT_H
#define CLIENT_H

/* Declarations */
#define VERSION     "V0.0"
#define VERSION_NUM 0


//Sates. Copied from Game.H
#define STATE_WAITING_PLAYERS 0x1        //Waiting for player
#define STATE_INGAME          0x2        //Playing state


/* Prototypes */
void error(const char *);     //Prints and error message and closes curses window
void readMessage(int, short *);    //Reads message from Server

#endif
