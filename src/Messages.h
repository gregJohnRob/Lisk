#ifndef MESSAGES_H
#define MESSAGES_H

#include "Protocol.h"



/* Prototypes */
Msg_t mTroopMessage(short);               //Message with number of troops player can place
Msg_t mPlaceMessage(short *, short);      //Message with result of placing troops


Msg_t mFailMessage(void);                 //Returns a message of failure

#endif
