#include "Coms.h"

/* Coms.c
 *
 * Communication for LISK Server
 *
 * Author: Ewan McCartney
 */


/* EncodeMessage
 *    Encodes a message into the provided buffer
 *    NOTE: Any further data must be added before sending
 *
 *    Message:
 *    [0] = STATUS | DATA FLAG
 *    [4] = MSG_END
 *
 *    Data can use 1,2,3 slots.
 */
void cEncodeMessage(short *Buffer, Msg_t *Msg)
{
    short Head = 0;               //0000 0000 0000 0000
    short Tail = 0xFF00;          //1111 1111 0000 0000

    Head = Msg->Code << 8;        //STATUS STATUS 0000 0000
    Head = Head | 0x00FF;         //STATUS STATUS 1111 1111

    Tail = Tail | Msg->Op;        //1111 1111 OPCODE OPCODE

    *(Buffer) = Tail & Head;      //STATUS STATUS OPCODE OPCODE
    *(Buffer + 4) = MSG_END;

}

/* DecodeMessage
 *  Decodes a message from recieved in Msg_t struct */
void cDecodeMessage(short *InBuffer, Msg_t *OutBuffer)
{
    short PCode;
    short OpCode;

    //Perform char value halfing
    PCode = *(InBuffer);      //P_CODE P_CODE OP_CODE OP_CODE
    PCode = PCode & 0xFF00;   //P_Code P_CODE 0000 0000
    PCode = PCode >> 8;       //0000 0000 P_CODE P_CODE

    OpCode = *(InBuffer) & 0x00FF;

    //Fill out structure
    OutBuffer->Code = PCode;
    OutBuffer->Op = OpCode;
    OutBuffer->Data[0] = *(InBuffer + 1);
    OutBuffer->Data[1] = *(InBuffer + 2);
    OutBuffer->Data[2] = *(InBuffer + 3);
}


/* Send Client Capcity Message
 *    Sends a short repsonce to clients saying that the capcity of the server is MAX */
int cSendClientCapacityMsg(int fd)
{
  Msg_t msg;
  short buffer[5];

  msg.Code = STATUS_FULL;
  msg.Op = CODE_NODATA;
  cEncodeMessage(&buffer[0], &msg);

  return write(fd, buffer, sizeof(buffer));
}



/* Send Client ID Message
 *    Sends the client their Id, LISK version and
 *    current game status */
int cSendClientIdMsg(int fd)
{
  Msg_t msg;
  short buffer[5];

  msg.Code = STATUS_OK;
  msg.Op = CODE_DATA;
  buffer[1] = gGenId();
  buffer[2] = VERSION_NUM;
  buffer[3] = gState();
  cEncodeMessage(&buffer[0], &msg);

  return write(fd, buffer, sizeof(buffer));
}



/* Send Game State Changes Message
 *    Notifies all clients that the gate state has changed. */
void cSendGameStateChangeMsg(int* fds)
{
  Msg_t msg;
  short buffer[5];

  msg.Code = STATUS_STATE;
  msg.Op = CODE_DATA;
  buffer[1] = gState();
  cEncodeMessage(&buffer[0], &msg);

  cBroadcastMsg(&msg, fds);
}




/* Write Message
 *    Writes a 10byte message to given fd */
int cWriteMessage(short *buffer, int fd) { return write(fd, buffer, 5); }



/* Broadcast Message
 *    Sends a message to all the fds of clients current connected on LISK
 *    TODO Not send to server
 */
void cBroadcastMsg(Msg_t* msg, int *fds)
{
  short buffer[5];
  int i = 0;
  int sd = 0;
  int retval = 0;

  cEncodeMessage(&buffer[0], msg);

  for(i = 0; i < MAX_CLIENTS; i++)
  {
      sd = fds[i];
      if(sd  >  0)
      {
        retval = write(sd, buffer, sizeof(buffer));
        if (retval < 0)
        {
          printf("WARN> Error sending message to client on fd (%d)\n", sd);
        }
      }
  }
}
