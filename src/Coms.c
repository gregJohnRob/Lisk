#include "Encode.h"
#include "Coms.h"

/* Coms.c
 *
 * Communication for LISK Server
 *
 * Author: Ewan McCartney
 */

/* Send Client Capcity Message
 *    Sends a short repsonce to clients saying that the capcity of the server is MAX */
int cSendClientCapacityMsg(int fd)
{
  Msg_t msg;
  short buffer[128];

  msg.Code = STATUS_FULL;
  msg.Op = CODE_NODATA;
  msg.DataSize = 0;
  cEncodeMessage(&buffer[0], &msg);

  return write(fd, buffer, sizeof(buffer));
}



/* Send Client ID Message
 *    Sends the client their Id, LISK version and
 *    current game status */
int cSendClientIdMsg(int fd, int state)
{
  Msg_t msg;
  short buffer[128];

  msg.Code = STATUS_OK;
  msg.Op = CODE_DATA;
  msg.Data[0] = gGenId();
  msg.Data[1] = VERSION_NUM;
  msg.Data[2] = state;
  msg.DataSize = 3;
  cEncodeMessage(&buffer[0], &msg);

  return write(fd, buffer, sizeof(buffer));
}



/* Send Game State Changes Message
 *    Notifies all clients that the gate state has changed. */
void cSendGameStateChangeMsg(int state, int* fds)
{
  Msg_t msg;
  short buffer[128];

  msg.Code = STATUS_STATE;
  msg.Op = CODE_DATA;
  msg.Data[0] = state;
  msg.DataSize = 1;
  cEncodeMessage(&buffer[0], &msg);

  cBroadcastMsg(&buffer[0], fds);
}


/* Broadcast Message
 *    Sends a message to all the fds of clients current connected on LISK
 *    TODO Not send to server
 */
void cBroadcastMsg(short msg[], int *fds)
{
  short len = 0;
  int i = 0;
  int sd = 0;
  int retval = 0;

  for(i = 0; i < MAX_CLIENTS; i++)
  {
      sd = fds[i];
      if(sd  >  0)
      {
        retval = write(sd, msg, sizeof(len) * 128);
        if (retval < 0)
        {
          printf("WARN> Error sending message to client on fd (%d)\n", sd);
        }
      }
  }
}
