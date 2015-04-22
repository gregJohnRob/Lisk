LAN RISK Test Client
--------

Client used for testing purposes

Building
---------

    gcc -o client TestClient2.c

**TODO** Add proper makefile


Running
-------

    ./client localhost|Network IP:port

localhost if connecting via 127.0.0.1 or the IP address of the computer running the server

port = Port number of the server. *You must give this even if using DEFAULT_PORT*

*EXAMPLE*:

    ./client 192.168.0.2:12345
