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
void BroadcastMsg(Msg_t *, int *);                           //Broadcast Message to all clients
void EncodeMessage(unsigned char *, Msg_t *);      //Encodes the start and end of a message to be sent
void DecodeMessage(unsigned char *, Msg_t *);               //Decodes the given message into a structure


#endif
