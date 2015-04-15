#ifndef ENCODE_H
#define ENCODE_H

#include <stdio.h>

#include "Protocol.h"

/* Declarations */


/* Prototypes */
void EncodeMessage(unsigned char *, unsigned char, unsigned char);      //Encodes the start and end of a message to be sent
void DecodeMessage(unsigned char *, Msg_t *);                           //Decodes the given message into a structure

#endif
