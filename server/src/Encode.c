#include "Encode.h"

/* Encode.c
 *
 * Some encoding utilites for communication functions
 * Author: Ewan McCartney
 */


/* EncodeMessage
 *    Encodes a message into the provided buffer
 *    NOTE: Any further data must be added before sending
 *
 *    Message:
 *    [0] = STATUS | DATA FLAG
 *    [4] = MSG_END
 */
void EncodeMessage(unsigned char *Buffer, Msg_t *Msg)
{
    unsigned char Head;        //0000 0000
    Head = Msg->Code << 4;        //STATUS 0000
    Head = Head | 0x0F;        //STATUS 1111
    *(Buffer) = Head & Msg->Op;   //STATUS DATA
    *(Buffer + 4) = MSG_END;
}

/* DecodeMessage
 *  Decodes a message from recieved in Msg_t struct
 *
 */
void DecodeMessage(unsigned char *InBuffer, Msg_t *OutBuffer)
{
    unsigned char PCode;
    unsigned char OpCode;

    //Perform char value halfing
    PCode = *(InBuffer);    //P_CODE OP_CODE
    PCode = PCode & 0xF0;   //P_Code 0000
    PCode = PCode >> 4;     //0000 P_CODE

    OpCode = *(InBuffer) & 0x0F;

    //Fill out structure
    OutBuffer->Code = PCode;
    OutBuffer->Op = OpCode;
    OutBuffer->Id = *(InBuffer + 1);
    OutBuffer->Data[0] = *(InBuffer + 2);
    OutBuffer->Data[1] = *(InBuffer + 3);

    if ((*InBuffer + 4) == MSG_END ) { OutBuffer->WasValid = 0; }
    else { OutBuffer->WasValid = -1; }
}
