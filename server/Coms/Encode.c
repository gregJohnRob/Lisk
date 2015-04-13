#include "Protocol.h"
#include "Encode.h"

/* EncodeMessage
 *    Encodes a message into the provided buffer
 *    NOTE: Any further data must be added before sending
 *
 *    Message:
 *    [0] = STATUS | DATA FLAG
 *    [4] = MSG_END
 */
void EncodeMessage(unsigned char *Buffer, unsigned char Status, unsigned char Data)
{
    unsigned char Head;                 //0000 0000
    Head = Status << 4;        //STATUS 0000
    Head = Head | 0x0F;        //STATUS 1111
    *(Buffer) = Head & Data;   //STATUS DATA
    *(Buffer + 4) = MSG_END;
}
