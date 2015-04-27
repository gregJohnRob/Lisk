#include "Coms.h"
#include "Protocol.h"

/* Coms.c
 *
 * Communication for LISK Server
 *
 * Author: Ewan McCartney
 */

int NumClients = 0;       //Current Number of clients connected
Client_t Clients[MAX_CLIENTS]; //File Descriptor list for clients


/* Getter for NumClients */
int GetNumClients() { return NumClients; }

/* AcceptClients
 *  Sits and waits for connections to the Server
 *  Once we have MAX_CLIENTS connected, we return
 */
void AcceptClients(int Serverfd)
{
  struct sockaddr_in Cli_addr;
  int NewClientfd = 0;
  Client_t NewClient;
  Msg_t Welcome;
  socklen_t Clilen;
  unsigned char Buffer[5];

  printf("> Waiting for clients... (%d / %d)\n", NumClients, MAX_CLIENTS);

  while(NumClients < MAX_CLIENTS)
  {
    //Accept gets a connection from the client, saving a new file descriptor, checking for errors once more.
    Clilen = sizeof(Cli_addr);
    NewClientfd = accept(Serverfd, (struct sockaddr *) &Cli_addr, &Clilen);
    if (NewClientfd < 0) { printf("ERROR> Unable to accept client\n"); }
    else
    {
      //Fills out new Client_t struct with the data we require
      struct sockaddr_in addr_in = (struct sockaddr_in)Cli_addr;
      char *IP = inet_ntoa(addr_in.sin_addr);
      NewClient.id = NumClients;
      NewClient.fd = NewClientfd;
      NewClient.IP = IP;

      Clients[NumClients] = NewClient;
      NumClients++;

      printf("> New Client (%d): %s\n", NumClients, IP);

      Welcome.Code = STATUS_OK;
      Welcome.Op = CODE_DATA;

      EncodeMessage(&Buffer[0], &Welcome);
      Buffer[1] = (unsigned char)NewClient.id;
      SendMsg(NewClient, Buffer);


        RecieveMsgs(NewClient.fd);

      printf("> Waiting for clients... (%d / %d)\n", NumClients, MAX_CLIENTS);
    }
  }

}

/* CloseAllClients
 *  Closes all Client connections the server current knows of
 */
void CloseAllClients()
{
  int i = 0;
  for(i = 0; i <= MAX_CLIENTS; i++)
  {
    close(Clients[i].fd);
  }
}

/* BroadcastMsg
 *  Sends given message to all clients
 *  NOTE: When boradcasting over localhost, server broadcasts to itself.
 */
void BroadcastMsg(unsigned char Msg[])
{
  int i = 0;
  for(i = 0; i <= MAX_CLIENTS; i++)
  {
    SendMsg(Clients[i], Msg);
  }
}


/* SendMsg
 *  Send message to given client
 */
int SendMsg(Client_t Client, unsigned char Msg[])
{
  int n = 0;

  n = write(Client.fd,Msg,5);
  if (n < 0)
  {
    printf("ERROR> Couldn't send messge to Client (%d)\n", Client.id);
  }
  return n;
}


void RecieveMsgs(int fd)
{
  struct timeval tv;
  fd_set rfds;
  int retval = 0;
  tv.tv_sec = 2;
  tv.tv_usec = 0;
  int i = 0;

  for(i = 0; i < NumClients; i++)
  {
    FD_SET(Clients[i].fd, &rfds);
  }

  while (1)
  {
        retval = select(NumClients+ 1, &rfds, NULL, NULL, &tv);

        if (retval == -1)
        { perror("select()"); }
        else if (retval)
        {
          for(i = 0; i < NumClients; i++)
          {
            if(FD_ISSET(Clients[i].fd, &rfds))
            {
              printf("Message on Client(%d): %s", Clients[i].id, Clients[i].IP);
              sleep(5);
            }
          }
        }
        else
        { printf("> No data within 2 second.\n"); }
  }


  unsigned char Buffer[5];
  Msg_t Msg;
  int n = read(fd,Buffer,5);
  if (n < 0) { printf("%s\n", "ERROR> reading from socket");  }
  DecodeMessage(&Buffer[0], &Msg);
  printf("Message Got: (Valid? %d) ID - %d  STATUS - %d OP - %d\n", Msg.WasValid, Msg.Id, Msg.Code, Msg.Op);
}
