#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

/* Prints out an error message to stderr then exits program */
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;          //File descriptor, Server port and retrun value
    struct sockaddr_in serv_addr;   //Address of the Server
    struct hostent *server;         //Pointer to server description struct

    //Checking CLI args for host and port
    char buffer[256];
    if (argc < 3)
    {
       error("usage: client hostname port\n");
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("ERROR opening socket");
    }

    //Get ther server name
    server = gethostbyname(argv[1]);
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
          n = read(sockfd,buffer,4);
          if (n < 0)
          {
            error("ERROR reading from socket");
          }
          printf("%s\n",buffer);
          if (strcmp(buffer, "stop") == 0) { berst = 2002; }
          bzero(buffer,256);
          sleep(2);
    }

    printf("> Recieved signal to close\n\n");
    close(sockfd);
    return 0;
}
