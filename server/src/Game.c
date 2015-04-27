#include "Game.h"

static Map_t* Map;                  //Game Map object
static short idNum = 0;             //Used to assign Ids to clients
static short state = 0;             //Internal game state
static short hasGotPlayers = 0;     //Flag incas


/* GenID
 *  Increments id niumber and gives it to clients */
short gGenId(void) { int i = idNum; idNum++; return i; }

/* Get State
 *  Returns the current state of game */
short gState(void) { return state; }

/* Set State
 *  Sets the state of the game and notifies everyone.
  * Here we also check for if the waiting players
  * have been set already incase of problems. */
void gSetState(int nState, int *fds) {

  //Check to make sure we don't override the game state
  if (nState == STATE_INGAME && hasGotPlayers > 0) { return; }
  else if (nState == STATE_INGAME && hasGotPlayers <=0) { hasGotPlayers = 100; }

  state = nState;

  cSendGameStateChangeMsg(fds);
}
