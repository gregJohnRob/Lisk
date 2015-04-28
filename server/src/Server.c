/* LISK SERVER
 *    Mutliple concurrent LAN Risk server. Please see README for
 *    more details on building.
 *
 *    Based on Silver Moons Linux server with select()
 *    -> http://www.binarytides.com/multiple-socket-connections-fdset-select-linux/
 *
 *     Usage ./LiskServer PORT_NUMBER
 *     If no port number given, uses DEFAULT PORT
 *
 *     Author: Ewan McCartney
 */
#include "Server.h"


int main(int argc , char *argv[])
{
    int opt = 1;                                //Used for setting up multiple addresses
    int PortNo = DEFAULT_PORT;                  //Server Port Number
    int master_socket = 0;                      //Fd of Server socket
    int addrlen = 0;                            //Length of IP address
    int new_socket = 0 ;                        //Store of fd when a new client connects
    int client_socket[MAX_CLIENTS];             //Store of all connected clients
    int activity = 0;                           //Return val of select()
    int i = 0;                                  //Counter
    int valread = 0;                            //Return val of read()
    int sd = 0;                                 //Used when reading set to listen on
    int max_sd = 0;                             //Highest fd
    int NumClients = 0;                         //Number of connected clients
    struct sockaddr_in address;                 //Server address
    char buffer[1025];                          //Data buffer of 1K
    fd_set readfds;                             //Set of socket descriptors

    Game_t game;                                //Game Config

    //Checking to see if we have a port number given as CLI args
    if (argc == 2) {  PortNo = atoi(argv[1]);  printf("> Port given. Using port: %d\n", PortNo); }
    else { printf("> No port given. Using default: %d\n",DEFAULT_PORT); }

    /* Setup Game Here */

    //Make sure to 0 the fd array so we don't send to random fds that doesn't exist
    for (i = 0; i < MAX_CLIENTS; i++) { client_socket[i] = 0; }
    gSetState(&game, STATE_INIT, &client_socket[0]);    //Put Game in Setup state
    // TODO Pick a Map
    // TODO Load Map from File
    // TODO Seed Random Number Generator
    // TODO Any other Setup?

    //create a master socket
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    //Setting master socket to allow multiple connections.
    //NOTE Will work without this but is apparently goo dhabit.
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    //Setup serv_addr struct with right values
    address.sin_family = AF_INET;          //Should always be AF_INET (I assume AF_UNIX for pipe)
    address.sin_addr.s_addr = INADDR_ANY;  //Sets the address of the Server. Should always be the IP of machine INADDR_ANY = IP of machine
    address.sin_port = htons(PortNo);      //Sets port of server. Requires to ne in Network byte order, so use Htons

    //bind the socket to localhost port : DEFAULT or given CLI arg
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Listener on port %d \n", PortNo);

    //Specify maximum of 5 pending connections for the master socket
    if (listen(master_socket, 5) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    //Accept the incoming connection
    addrlen = sizeof(address);
    puts("Waiting for connections ...");

    gSetState(&game, STATE_WAITING_PLAYERS, &client_socket[0]); //Put game in waiting state, while we find players

    while(1)
    {
      //Quick check incase we have everyone ready to play then start the game?!
      if (NumClients == MAX_CLIENTS)
      {
        puts("> All players have joined. Starting game...");
        gSetState(&game, STATE_INGAME, &client_socket[0]);

        //TODO Once connected populate the Map and start the Game
        //TODO Setup Game: Turn(s) etc
      }

        //Clearing set and adding server socket for reading
        FD_ZERO(&readfds);
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        //Adding all the child sockets to the set, making sure they are valid
        //And updating the highest for select()
        for ( i = 0 ; i < MAX_CLIENTS ; i++)
        {
            sd = client_socket[i];
            if(sd > 0) { FD_SET( sd , &readfds);}
            if(sd > max_sd) { max_sd = sd; }
        }

        //Waiting for activity. SInce no timeout we do this indefinately
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);

        if ((activity < 0) && (errno!=EINTR))  { printf("select error"); }

        //First - Check master socket. If something here we have a new connection
        if (FD_ISSET(master_socket, &readfds))
        {
          if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
          {
            perror("accept");
            exit(EXIT_FAILURE);
          }

          //Helpful print message
          printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

          NumClients++;

          //Make sure to refuse the connection if the game is in a playing state
          //or if the maximum number of clients have been reached.
          if( (NumClients > MAX_CLIENTS) || (game.State != STATE_WAITING_PLAYERS) )
          {
            NumClients--;
            puts("> Client capcity full. CLosing new connection");
            if( cSendClientCapacityMsg(new_socket) < 0 ) { perror("Failed to send FULL message"); }
            close (new_socket);
          }

          //Otherwise, we can add them to the game, assign and ID
          //and send off the welcome message to them.
          else
          {
            //Send connection the info we require.
            if( cSendClientIdMsg(new_socket, game.State) < 0 ) { perror("send"); }

            puts("Welcome message sent successfully");

            //Adding the new socket to the list of clients
            for (i = 0; i < MAX_CLIENTS; i++)
            {
              if( client_socket[i] == 0 )
              {
                client_socket[i] = new_socket;
                printf("Adding to list of sockets as %d\n" , i);
                break;
              }
            }
          }
        }

        //If not on master, we have something from the clients!
        for (i = 0; i < MAX_CLIENTS; i++)
        {
            sd = client_socket[i];

            if (FD_ISSET( sd , &readfds))
            {
                //Checking for closing making sure to read message
                if ((valread = read( sd , buffer, 1024)) == 0)
                {
                    //Somebody disconnected , get his details and print
                    getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
                    printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

                    //Close the socket and mark as 0 in list for reuse
                    close( sd );
                    client_socket[i] = 0;
                    NumClients--;
                }

                //Other than that, we can now do some processing on the message!

                //TODO Interpret message from Client
                //TODO Process responce
                //TODO Send back reply
                //TODO Add check at top of Loop if the game state has changed or something?
                else
                {
                    //set the string terminating NULL byte on the end of the data read
                    buffer[valread] = '\0';
                    send(sd , buffer , strlen(buffer) , 0 );
                }
            }
        }
    }

    return 0;
}
