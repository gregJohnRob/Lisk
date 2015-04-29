#ifndef ENCODE_H
#define ENCODE_H

/* Includes */
#include "Protocol.h"

/* Prototypes */
void cEncodeMessage(short *, Msg_t *);          //Encodes the start and end of a message to be sent
void cDecodeMessage(short *, Msg_t *);          //Decodes the given message into a structure


#endif
