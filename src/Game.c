#include "Game.h"

static LiskMap_t* Map;              //Game Map object
static short idNum = 0;             //Used to assign Ids to clients
static short hasGotPlayers = 0;     //Flag incase of overwriting play state


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

  printf("***Updated Game State. State=%d***\n", game->State);

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


/* Trade in Cards
 *   Trades in cards and updates the card bonus for next trade-in
 *   Card bonus is 2,4,6,8,10 -> 15 then +5 each time */
short gTradeCards(Game_t *game, Card_t *cards, int numCards)
{
    short troops = game->CardBonus;
    int i = 0;
    short colour = cards[0].Colour;

    for(i = 1; i < numCards; i++)
    {
       if (cards[i].Colour != colour && cards[i].Colour != CARD_BLACK ) { return 0; }
    }

    if (game->CardBonus < 15) { game->CardBonus += 2; }
    else { game->CardBonus += 5; }

    return troops;
}
