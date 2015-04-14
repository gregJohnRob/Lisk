#ifndef COMS_H
#define COMS_H

/* Includes */
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "Encode.h"

/* Defines */
#define MAX_CLIENTS     2     //Sets maximum number of connections kept alive
#define DEFAULT_PORT    12345 //Sets default server port

/* Declarations */

//Defines a client in terms of Coms
typedef struct Client
{
  char id;    //ID number of Client
  char* IP;   //IP address
  int fd;     //Socket FD of Client
}Client_t;

/* Prototypes */
int GetNumClients(void);      //Returns the number of connected clients
void AcceptClients(int);      //Collects all details we need from clients
void CloseAllClients(void);   //CLoses all clients we know of

/* Message Send Functions */
void BroadcastMsg(unsigned char[]);
int SendMsg(Client_t, unsigned char[]);

#endif
