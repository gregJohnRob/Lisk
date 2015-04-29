#ifndef COMS_H
#define COMS_H

/* Includes */
#include <stdio.h>
#include <sys/socket.h>

#include "Encode.h"
#include "Protocol.h"
#include "Server.h"

/* Defines */

/* Declarations */


/* Prototypes */
void cBroadcastMsg(short *, int *);             //Broadcast Message to all clients
int  cSendClientCapacityMsg(int);               //Sends message to new client that we're full
int  cSendClientIdMsg(int, int);                //Sends the client their ID :)
void cSendGameStateChangeMsg(int , int *);      //Sends clients a notice of the game state being updated
int cSendAttackMsg(int, Msg_t *);               //Sends the result of the attack to current player

#endif