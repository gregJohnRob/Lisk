/* A LAN Risk Server
 * -----------------
 * Usage ./server PORT_NUMBER
 *
 * If no port number given, uses DEFAULT PORT
 *
 * Version: 0.1 (Pre-Alpha)
 * AUthor: BigE
 */
#include "Server.h"

/* Small function to print out errors over stderr.
 * Program terminates once message printed */
void error(const char *msg)
{
    perror(msg);
    exit(1);
}
/* Globals */
int Serverfd = 0;                           //Server File Descriptor
int PortNo = DEFAULT_PORT;                  //Port Number of Server


int main(int argc, char *argv[])
{
    //File Descriptors for the server socket and client socket, allowing use of read() and write()
     int newsockfd;
     socklen_t clilen;                         //Stores the size of client's address
     char buffer[256];                        //Buffer for storing output messages
     struct sockaddr_in cli_addr;  //Structures for holding IP addresses (serv for the server, cli for client)
     int n;                                   //Return codes for socket functions

     printf("> Starting up...\nMSG> Version: %s\n", VERSION);

     //Checking to see if we have a port number given as CLI args
     if (argc == 2) {  PortNo = atoi(argv[1]); }
     else { printf("> No port given. Using default: %d\n",DEFAULT_PORT); }

     //Run Setup of server. Incase of error, we exit
     if (Serv_Setup() <0) { printf("NOTICE> Exiting...\n"); exit(1); }

     //Once setup has been run, enter client connection stage.
     //EDIT: Perhaps put this in a loop?
     AcceptClients(Serverfd);

     //Once we're done, tell user we're exiting, cause this is only debug!
     printf("NOTICE> Shutting down...\n");

     BroadcastMsg("stop");
     CloseAllClients();

     close(Serverfd);
     exit(0);

     //UNREACHABLE CURRENTLY

     //Accept gets a connection from the client, saving a new file descriptor, checking for errors once more.
     clilen = sizeof(cli_addr);
     newsockfd = accept(Serverfd, (struct sockaddr *) &cli_addr, &clilen);
     if (newsockfd < 0)
     {
       error("ERROR on accept");
     }

     //Reads in data from the client that's connected
     bzero(buffer,256);
     n = read(newsockfd,buffer,255);
     if (n < 0)
     {
        error("ERROR reading from socket");
     }

     //Prints out what it got, and sends a confirmation to the client.
     printf("Here is the message: %s\n",buffer);
     n = write(newsockfd,"I got your message",18);
     if (n < 0)
     {
       error("ERROR writing to socket");
     }

     //Closes both server and client sockets, IN CORRECT ORDER
     close(newsockfd);
     close(Serverfd);
     return 0;
}

/* Setup
 *   Creates Server socket and performs bind() and listen()
 *   Allows server to startup
 *
 * Returns -1 if fails
 */
int Serv_Setup()
{
    struct sockaddr_in serv_addr;

    Serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if (Serverfd < 0)
    {
        printf("ERROR> Couldn't start network coms.\n");
        return -1;
    }

    //Sets everything in the serv_addrs to 0
    bzero((char *) &serv_addr, sizeof(serv_addr));

    //Setup serv_addr struct with right values
    serv_addr.sin_family = AF_INET;          //Should always be AF_INET (I assume AF_UNIX for pipe)
    serv_addr.sin_addr.s_addr = INADDR_ANY;  //Sets the address of the Server. Should always be the IP of machine INADDR_ANY = IP of machine
    serv_addr.sin_port = htons(PortNo);      //Sets port of server. Requires to ne in Network byte order, so use Htons

    /* Binds the Socket to the address.
     * Bind(FILE DES, ADDRESS, SIZE)
     * FILE DES is the value returned from socket()
     * ADDRESS is our addr struct, but requires to be cast to sockaddr for function
     * SIZE of the address of socket
     *
     * Returns -1 if failed. Perhaps the socket is already in use.
     */
    if (bind(Serverfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("ERROR> Unable to associate server with network socket.\n");
       return -1;
    }

    //Listens for connections. accept maximum of 5 connections in queue while processing responce. Standard size for systems
    listen(Serverfd,5);
    printf("MSG> I'm broadcasting on: 127.0.0.1 and your computer's IP address.\n");
    return 0;
}
