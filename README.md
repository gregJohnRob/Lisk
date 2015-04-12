LAN RISK
--------
A C implementation of Risk over a LAN network.

*Work in Progress*

Status
------
The server will currently only accept 2 test clients and then will force itself shut. When a client connects it send out "stop" which, if using the provided client will cause to close after some delay.


Building
--------
*Server*

    gcc -o server Server.c Coms/Coms.c

**TODO** Add proper makefile


*Client*

    gcc -o client TestClient2.c

**TODO** Add proper makefile


Running
-------
*Server*

    ./server port

port = Server port number. This is *optional*. If left blank, runs on DEFAULT_PORT = 12345

Server broadcasts on 127.0.0.1 AND your computer's network IP (usually 192.168.X.Y)

*NOTE:* After server close, it will fail to open for a short period of time. This is because Linux keeps connections and sockets alive for a set period of time when an application closes for saftey reasons.


*Client*

   ./client localhost|Network IP port

localhost if connecting via 127.0.0.1 or the IP address of the computer running the server

port = Port number of the server. *You must give this even if using DEFAULT_PORT*
