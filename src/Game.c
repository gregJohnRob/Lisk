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


/* Attack
 *    Performs an attacking phase between 2 nodes
 *  Checks if the attacker/defender are adjacent in the Map
 *  Works out No. dice/player
 *  Rolls and sends message back about who won/lost */
Msg_t gAttack(LiskMap_t *map, int attacker, int defender)
{
    Msg_t msg;
    short canAttack = mCanAttack(map, attacker, defender);

    //Just making sure we're connected
    if (canAttack == NOT_CONNECTED)
    {
      msg.Code = STATUS_FAIL;
      msg.Op = CODE_NODATA;
      msg.DataSize = 0;
    }
    else
    {
      Node_t aNode = map->Nodes[attacker];
      Node_t dNode = map->Nodes[defender];
      short aDice = 0;
      short dDice = 2;
      short aKills = 0;
      short dKills = 0;
      short wasConq = 0;
      unsigned int aRolls[3] = { 0, 0, 0};
      unsigned int dRolls[2] = { 0, 0};
      int i = 0;

      switch (aNode.Troops)
      {
        case 1: { aDice = 0; }
        case 2: { aDice = 1; }
        case 3: { aDice = 2; }
        default: { aDice = 3; }
      }

      //If only 1 troop, we can't attack can we?
      if (aDice == 0 )
      {
        msg.Code = STATUS_FAIL;
        msg.Op = CODE_NODATA;
        msg.DataSize = 0;
      }

      if (dNode.Troops == 1 ) { dDice = 1; }

      //Dice Rolls
      for( i = 0; i < aDice; i++) { aRolls[i] = uDiceRoll(); }
      for( i = 0; i < dDice; i++) { dRolls[i] = uDiceRoll(); }

      uSortHigh(aRolls, 3);
      uSortHigh(dRolls, 2);

      for(i = 0; i < 2; i++)
      {
        short a = aRolls[i];
        short d = dRolls[i];

        if (d == 0 && a == 0) { break; }
        else
        {
          if (a > d) { aKills++; }
          else { dKills++; }
        }
      }

      map->Nodes[attacker].Troops -= dKills;
      map->Nodes[defender].Troops -= aKills;

      //All of attackers troops die, and so defender gains it.
      if (map->Nodes[attacker].Troops < 0 && map->Nodes[defender].Troops >= 2)
      {
        map->Nodes[attacker].Troops = 1;
        map->Nodes[attacker].Owner = map->Nodes[defender].Owner;

        map->Nodes[defender].Troops--;
        wasConq = 100;
      }
      else if (map->Nodes[attacker].Troops < 0 && map->Nodes[defender].Troops < 2)
      {
          map->Nodes[attacker].Troops = 1;
          map->Nodes[attacker].Owner = NPC_ID;
          wasConq = 100;
      }
      else if (map->Nodes[defender].Troops < 0 && map->Nodes[attacker].Troops >= 2)
      {
        map->Nodes[defender].Troops = 1;
        map->Nodes[defender].Owner = map->Nodes[attacker].Owner;

        map->Nodes[attacker].Troops--;
        wasConq = 100;
      }
      else if (map->Nodes[defender].Troops < 0 && map->Nodes[attacker].Troops < 2)
      {
        map->Nodes[defender].Troops = 1;
        map->Nodes[defender].Owner = NPC_ID;
        wasConq = 100;
      }

      msg.Code = STATUS_OK;
      msg.Op = CODE_DATA;

      msg.Data[0] = wasConq;
      msg.Data[1] = aKills;
      msg.Data[2] = dKills;
      msg.Data[3] = aRolls[0];
      msg.Data[4] = aRolls[1];
      msg.Data[5] = aRolls[2];
      msg.Data[6] = dRolls[0];
      msg.Data[7] = dRolls[1];
      msg.DataSize = 8;
    }

    return msg;
}
