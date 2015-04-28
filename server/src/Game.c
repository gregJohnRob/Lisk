#include "Game.h"

static LiskMap_t* Map;                  //Game Map object
static short idNum = 0;             //Used to assign Ids to clients
short state = 0;                     //Internal game state
static short hasGotPlayers = 0;     //Flag incas


/* GenID
 *  Increments id niumber and gives it to clients */
short gGenId(void) { int i = idNum; idNum++; return i; }

/* Set State
 *  Sets the state of the game and notifies everyone.
  * Here we also check for if the waiting players
  * have been set already incase of problems. */
void gSetState(Game_t *game, int nState, int *fds) {

  //Check to make sure we don't override the game state
  if (nState == STATE_INGAME && hasGotPlayers > 0) { return; }
  else if (nState == STATE_INGAME && hasGotPlayers <=0) { hasGotPlayers = 100; }

  game->State = nState;

  printf("***Updated Game State. State = %d***\n", game->State);

  cSendGameStateChangeMsg(game->State, fds);
}



/* Troops Per Turn
 *    Returns the number of troops the given player
 *    will recieve at the start of the turn */
short gTroopsPerTurn(LiskMap_t *map, Player_t *player)
{
  int i = 0;
  short troops = 0;
  for(i = 0; i < Map->NodeCount; i++)
  {
      if (Map->Nodes[i].Owner == player->Id) { troops++; }
  }

  return troops / 3;
}


/* Total Troops
 *    Returns the total troops that player has. */
int gTotalTroops(LiskMap_t *map, Player_t *player)
{
  int i = 0;
  int troops = 0;
  for(i = 0; i < Map->NodeCount; i++)
  {
      if (Map->Nodes[i].Owner == player->Id) { troops += Map->Nodes[i].Troops; }
  }

  return troops;
}
