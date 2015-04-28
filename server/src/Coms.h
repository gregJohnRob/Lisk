#ifndef COMS_H
#define COMS_H

/* Includes */
#include <stdio.h>
#include <sys/socket.h>
#include "Protocol.h"
#include "Server.h"

/* Defines */

/* Declarations */


/* Prototypes */
void cEncodeMessage(short *, Msg_t *);          //Encodes the start and end of a message to be sent
void cDecodeMessage(short *, Msg_t *);          //Decodes the given message into a structure

void cBroadcastMsg(short *, int *);             //Broadcast Message to all clients
int  cSendClientCapacityMsg(int);               //Sends message to new client that we're full
int  cSendClientIdMsg(int, int);                //Sends the client their ID :)
void cSendGameStateChangeMsg(int , int *);      //Sends clients a notice of the game state being updated

#endif
