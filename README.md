LAN RISK
--------
A C implementation of Risk over a LAN network.

*Work in Progress*



Building
--------
*build both Client & Server*

    ./make.sh <OPTION>

Options:
---
- -a = Build both Client and Server
- -c = Build only Client
- -s = Build only Server
- *See building.txt for more options*


Running
-------
*NOTE: Once compiled, execuitables are found in bin/*

----

*Server*

    ./LiskServer port

port = Server port number. This is *optional*. If left blank, runs on DEFAULT_PORT = 12345

Server broadcasts on 127.0.0.1 AND your computer's network IP (usually 192.168.X.Y)

*NOTE:* After server close, it will fail to open for a short period of time. This is because Linux keeps connections and sockets alive for a set period of time when an application closes for saftey reasons.


*Client*

    ./LiskClient localhost|Network IP:port

localhost if connecting via 127.0.0.1 or the IP address of the computer running the server

port = Port number of the server. *You must give this even if using DEFAULT_PORT*

*Example*

    ./client 192.168.0.2:12345
