#include "Messages.h"
#include "Game.h"

/* Generates Message with Troop count
 *  Calcs how many troops the given player gets
 *  and returns message with this data */
Msg_t mTroopMessage(short id)
{
  Msg_t msg;
  short troops = gTroopsPerTurn(id);

  if (troops < 0) { msg = mFailMessage(); }
  else
  {
    msg.Code = STATUS_OK;
    msg.Op = CODE_DATA;
    msg.DataSize = 1;

    msg.Data[0] = troops;
  }

  return msg;
}

/* Conducts a placement of troops on the map */
Msg_t mPlaceMessage(short *data, short size)
{
  Msg_t msg;
  //If not enough PLAYER_ID | PLACE_ID | TROOP COUNT
  if (size != 3) { msg = mFailMessage(); }
  else
  {
      int id = data[0];
      int pid = data[1];
      int troops = data[2];
      int res = gPlaceTroops(id, pid, troops);

      if (res < 0) { msg = mFailMessage(); }
      else
      {
        msg.Code = STATUS_OK;
        msg.Op = CODE_NODATA;
        msg.DataSize = 0;
      }
  }
  return msg;
}



/* Returns a mesasge of failure */
Msg_t mFailMessage()
{
  Msg_t msg;

  msg.Code = STATUS_FAIL;
  msg.Op = CODE_NODATA;
  msg.DataSize = 0;

  return msg;
}
