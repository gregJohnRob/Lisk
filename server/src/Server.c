/* A LAN Risk Server
 * -----------------
 * Usage ./server PORT_NUMBER
 *
 * If no port number given, uses DEFAULT PORT
 *
 * Author: Ewan McCartney
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
Map_t *Map;

int main(int argc, char *argv[])
{
    //File Descriptors for the server socket and client socket, allowing use of read() and write()
     int newsockfd;
     char buffer[256];                        //Buffer for storing output messages
     int n;                                   //Return codes for socket functions

     printf("> Starting up...\nMSG> Version: %s\n", VERSION);

     //Checking to see if we have a port number given as CLI args
     if (argc == 2) {  PortNo = atoi(argv[1]); }
     else { printf("> No port given. Using default: %d\n",DEFAULT_PORT); }

     Map_t *Map = mCreate("TestMap.txt");

     if (Map == NULL) { printf("ERROR> %s\n", "Failed to load TestMap.txt"); exit(1); }
     else { printf("MSG> %s -> %s\n", "Loaded Map", Map->Name); }

     //Run Setup of server. Incase of error, we exit
     if (Serv_Setup() <0) { printf("NOTICE> Exiting...\n"); exit(1); }

    //Short Map testing section to support the loading of a Map to test
    #if MAP_DEBUG
        //printMap(Map);
        short V = mIsConnected(Map, 0,3);
        short VV = mIsConnected(Map, 1, 2);
        short VVV = mIsConnected(Map, 1, 6);
        printf("> Is 0 -> 3? %d\n", V);
        printf("> Is 1 -> 2? %d\n", VV);
        printf("> Is 1 -> 6? %d\n", VVV);
        exit(0);
    #endif



     //Once setup has been run, enter client connection stage.
     //EDIT: Perhaps put this in a loop?
     AcceptClients(Serverfd);

     //Once we're done, tell user we're exiting, cause this is only debug!
     printf("NOTICE> Shutting down...\n");
     CloseAllClients();

     //Free up the memory used by the Map to save memory leaks!
     mDestory(Map);

     close(Serverfd);
     exit(0);

     //UNREACHABLE CURRENTLY


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
