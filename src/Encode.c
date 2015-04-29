#include "Encode.h"

/* Encode.c
 *  Contains common message encode and decode functions
 * for both server and clients
 *
 * Author: Ewan McCartney
 */

 /* EncodeMessage
  *    Encodes a message into the provided buffer
  *
  * Data must be added to buffer before this is called, as this
  * puts data into buffer and finished it at MSG_END so client
  * knowns when the message has finished.
  *
  * NOTE Despite having 128Data slots, messages are only sent
  * of size 128, meaning you only have 124Data slots in total
  * 4 Reserved for Code, Op, DataSize and MSG_ENDs
  */
 void cEncodeMessage(short *buffer, Msg_t *msg)
 {
   int i = 0;

   buffer[0] = msg->Code;
   buffer[1] = msg->Op;
   buffer[2] = msg->DataSize;

   for(i = 0 ; i < msg->DataSize; i++)
   {
       buffer[3+i] = msg->Data[i];
   }
   for(i = msg->DataSize - 1; i < 128; i++){ msg->Data[i] = 0; }

   buffer[3 + msg->DataSize] = MSG_END;
 }

 /* DecodeMessage
  *  Decodes a message from recieved in Msg_t struct */
 void cDecodeMessage(short *inBuffer, Msg_t *msg)
 {
   int i = 0;

   msg->Code = inBuffer[0];
   msg->Op = inBuffer[1];
   msg->DataSize = inBuffer[2];

   for(i = 0 ; i < msg->DataSize; i++)
   {
       msg->Data[i]   = inBuffer[3+i];
   }
 }
