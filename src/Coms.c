#include "Coms.h"
#include "Encode.h"
#include "Server.h"
#include "Messages.h"

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


/* Send Attack Message
 *    Sends the resulting message from gAttack method */
int csendAttackMsg(int fd, Msg_t* msg)
{
    short buffer[128];
    cEncodeMessage(&buffer[0], msg);

    return write(fd, buffer, sizeof(buffer));
}


/* Given a message from the client
 * decodes what should be done
 * passing it off to specific function to handle
 * that code's operation
 *
 * msg = Message read
 * C = Fd of client
 */
void cProcessMessage(Msg_t *msg, int c)
{
    switch(msg->Code)
    {
      case CODE_SERV:  { cPServ(msg, c);  break; }
      case CODE_GAME:  { cPGame(msg, c);  break; }
      case CODE_ADMIN: { cPAdmin(msg, c); break;}
      default:
      {
        cPUnknown(msg, c);
        break;
      }
    }
}


/* Handles server message code */
void cPServ(Msg_t *msg, int c)
{
  switch (msg->Op)
  {
    case SERV_GAMELIST: { break; }        //TODO Get Gamelist
    case SERV_PLAYERINFO: { break; }      //TODO Get Playerinfo
    case SERV_GAMEINFO: { break; }        //TODO Get Game Info (Players, rounds, times)
    case SERV_GAME: { break; }            //TODO Get Game (Map ID / Placements)
    case SERV_JOIN: { break; }            //TODO Player joins Game
    case SERV_LEAVE: { break; }           //TODO Lave Game
    default:
    {
      cPUnknown(msg, c);
      break;
    }
  }
}


/* Handles Game message code */
void cPGame(Msg_t *msg, int c)
{
  Msg_t rmsg;
  short buffer[128];

  switch (msg->Op)
  {
    case GAME_START: { break; }         //TODO Turn Start
    case GAME_END: { break; }           //TODO Turn End
    case GAME_P1: { break; }            //TODO Phase 1 -> Cards - ENTER PHASE

    case GAME_P2:   //Sends out the number of troops player can place
      {
        rmsg = mTroopMessage(msg->Data[0]);
        cEncodeMessage(&buffer[0], &rmsg);
        int val =  write(c, buffer, sizeof(buffer));
        if (val < 0 ) { printf("Error sending MSG_TROOPS to client (%d)", c); }
        break;
      }


    case GAME_P3: { break; }            //TODO Phase 3 -> Attack  - ENTER PHASE
    case GAME_P4: { break; }            //TODO Phase 4 -> Fortify - ENTER PHASE

    case GAME_PLACE:  //Places troops and returns a success or fail message
      {
        rmsg = mPlaceMessage(msg->Data, msg->DataSize);
        cEncodeMessage(&buffer[0], &rmsg);
        int val =  write(c, buffer, sizeof(buffer));
        if (val < 0 ) { printf("Error sending MSG_TROOPS to client (%d)", c); }
        break;
      }

    case GAME_ATTACK: { break; }        //TODO Perform attack
    case GAME_FORTIFY: { break; }       //TODO Fortify place
    case GAME_TRADE: { break; }         //TODO Trade in cards
    case GAME_INFO: { break; }          //TODO Return game info
    case GAME_TIME: { break; }          //TODO return target time for move/turn
    default:
    {
      cPUnknown(msg, c);
      break;
    }
  }
}

/* Handles Admin message code
 * Nothing actually is defined
 * for admin but is here when
 * required */
void cPAdmin(Msg_t *msg, int c)
{
  puts("Admin reques recieved. How did this get here? ");
  cPUnknown(msg, c);
}


/* Handles an unknown message code */
void cPUnknown(Msg_t *msg, int c)
{
  Msg_t rmsg;
  short buffer[128];
  rmsg.Code = STATUS_UNKNOWN;
  rmsg.Op = CODE_NODATA;
  rmsg.DataSize = 0;
  cEncodeMessage(&buffer[0], &rmsg);

  int val =  write(c, buffer, sizeof(buffer));
  if (val < 0 ) { printf("Error sending MSG_UNKNOWN to client (%d)", c); }
}
