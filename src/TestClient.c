#define CLIENT_BUILD        //Define this so we only include what we need from
                            //Server headers
/* *****************************************
 * LISK TEST CLIENT
 *
 *    A debugging client for LISK. You can't play
 *    the game through this. Or at least, not yet.
 *
 *   Author: Ewan McCartney
 * *****************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

//Better terminal output :)
#include <ncurses.h>


#include "Client.h"
#include "Protocol.h"
#include "Encode.h"

/* Prints out an error message to stderr then exits program */
void error(const char *msg)
{
    endwin();
    puts(msg);
    exit(1);
}

char Renderable[32][32];  //Renderable screen space [X][Y]

int main(int argc, char *argv[])
{
    int sockfd, portno, n;          //File descriptor, Server port and retrun value
    struct sockaddr_in serv_addr;   //Address of the Server
    struct hostent *server;         //Pointer to server description struct
    short buffer[128];              //Data buffer for send/recieve
    Msg_t msg;                      //Message buffer for sending/receiving
    short thisId = 0;               //Client ID

    //Checking CLI args for host and port
    if (argc != 2)
    {
       printf("%s", "Usage: ./client HostName:Port\n");
       exit(1);
    }

    /* Setup the window in curses mode! */
    initscr();
    printw("LISK Test Client\n");
    printw("Version: %s (%d)\n", VERSION, VERSION_NUM );

    printw("> Attempting connection to LISK Server. Please wait...\n");
    refresh();

    char *hostname;
    char *delim = ":";

	if (!strchr(argv[1], delim[0])) 
	{
		error("Usage: ./client HostName:Port");
	}

    // Token will point to hostname.
	
    hostname = strtok(argv[1], delim);
	
    portno = atoi(strtok(NULL, delim));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("ERROR opening socket\n");
    }

    //Get ther server name
    server = gethostbyname(hostname);
    if (server == NULL)
    {
        error("ERROR, no such host\n");
    }

    //Setup the address struct, we need to copy the server address into the struct
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    /* Then we connect to the server
     * FILE DESCRIPTOR - Of socket
     * ADDRESS - IP, Port of server
     * SIZE - of Address
     *
     * Returns -1 if fails
     */
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    {
        error("ERROR connecting");
    }

    //After initial connect - Read a message from the Server with our id number :)
    readMessage(sockfd, &buffer[0]);
    cDecodeMessage(&buffer[0], &msg);

    //Need to check if our server is currently full.
    if (msg.Code == STATUS_FULL)
    {
      clear();
      printw("Message> ERROR: Client Capacity full!\n");
      printw("\nPress any key to continue...");
      getch();
      endwin();
      return 0;
    }

    if (msg.Code == STATUS_OK && msg.DataSize == 3)
    {
        printw("> Client ID = %d\n", msg.Data[0]);
        printw("> LISK SERVER Version: %d\n", msg.Data[1]);

        if (msg.Data[2] == STATE_WAITING_PLAYERS)
        {
          printw(">> Waiting for game to start...\n");
        }
    }

    //Wait until we get a status update....
    while(msg.Code != STATUS_STATE)
    {
      readMessage(sockfd, &buffer[0]);
      cDecodeMessage(&buffer[0], &msg);
    }
    printw("READY TO START PLAYING!");
    getch();
    endwin();
    return 0;

    /* Get input from user and send to server */
    //printf("Please enter the message: ");
    //bzero(buffer,256);
    //fgets(buffer,255,stdin);
    //n = write(sockfd,buffer,strlen(buffer));
    //if (n < 0)
    //{
    //     error("ERROR writing to socket");
    //}

    /* Get reply from server then close the socket */
    bzero(buffer,256);
    int berst = 0;

    while(berst == 0)
    {
          n = read(sockfd,buffer,256);
          if (n < 0)
          {
            error("ERROR reading from socket");
          }

          Msg_t msg;
          int g;
          cDecodeMessage(&buffer[0], &msg);
          printf("Message Recieved!\nSTATUS: %d\nOP: %d\nDATA SIZE: %d\nData[0]: %d\nData[1]: %d\nData[2]: %d\n",
                                                        msg.Code,
                                                        msg.Op,
                                                        msg.DataSize,
                                                        msg.Data[0],
                                                        msg.Data[1],
                                                        msg.Data[2]);

         scanf("%d", &g);
         for(g = 0; g < 128; g++){ msg.Data[g] = 0; }


    }

    /* unsigned char Buffer[5];
    unsigned char *ptr_Buffer = Buffer;
    EncodeMessage(ptr_Buffer, CODE_SERV, SERV_GAME);
    Buffer[1] = (unsigned char)6;


    while(1)
    {
    //Sending initial request for Game Data
    n = write(sockfd,Buffer,5);
    if (n < 0)  {  error("ERROR writing to socket");  }

    sleep(2);
  }
  */

    printw("\n> Shutting down...");
    endwin();			/* End curses mode		  */
    close(sockfd);
    return 0;
}


/* Gets all the Server Data of the Map so we can populate it
 *
 */
void mGet(int sFd, int mId)
{
  short buffer[128];
  Msg_t msg;
  msg.Code = CODE_SERV;
  msg.Op = SERV_GAME;
  msg.DataSize = 0;

  cEncodeMessage(&buffer[0], &msg);

  //Sending initial request for Game Data
  int n = write(sFd,buffer,128);
  if (n < 0)  {  error("ERROR writing to socket");  }

  int berst = 0;

  while(berst == 0)
  {
        n = read(sFd,buffer,128);
        if (n < 0) { error("ERROR reading from socket");   }

        if (buffer[0] != 1) { printf("ERROR> Expected STATUS_OK + DATA_FLAG\n"); }
        else
        {
          if (buffer[4] != 0xEE) { printf("ERROR> Expected MSG_END\n");}
          else {
            berst = 2002; }
        }
  }
}


//Reads Message from Server
void readMessage(int fd, short *buffer)
{
  int n = 0;

  n = read(fd,buffer,128);
  if (n < 0)
  {
    error("ERROR reading from socket");
  }

  printw("Mesasge Recieved");
}
