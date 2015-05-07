#ifndef COMS_H
#define COMS_H

/* Includes */
#include <stdio.h>
#include <sys/socket.h>

#include "Protocol.h"

/* Defines */

/* Declarations */


/* Prototypes */
void cBroadcastMsg(short *, int *);             //Broadcast Message to all clients
int  cSendClientCapacityMsg(int);               //Sends message to new client that we're full
int  cSendClientIdMsg(int, int);                //Sends the client their ID :)
void cSendGameStateChangeMsg(int , int *);      //Sends clients a notice of the game state being updated
int cSendAttackMsg(int, Msg_t *);               //Sends the result of the attack to current player



/* Message Processing Functions */
void cProcessMessage(Msg_t *, int);             //General processor
void cPServ(Msg_t *, int);                      //Server code processor
void cPGame(Msg_t *, int );                     //Game code processor
void cPAdmin(Msg_t *, int);                     //Admin code processor
void cPUnknown(Msg_t *, int);                   //Unknown code processor
#endif
