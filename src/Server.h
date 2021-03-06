#ifndef SERVER_H
#define SERVER_H

/* Includes */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>

#include "Coms.h"
#include "Game.h"

/* Declarations */
#define DEFAULT_PORT 12345      //Default listening port
#define MAX_CLIENTS  2          //Number of simulatenous clients

#define VERSION     "0.3(C)"    //LISK version as string
#define VERSION_NUM 10          //LISK version as number
/* Prototypes */
#endif
