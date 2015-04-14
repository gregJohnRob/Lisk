LAN Risk Server
------------

Building
----------

    make

**NOTE** Make options here map directory to make script commands in root


Running
-------

    ./server port

port = Server port number. This is *optional*. If left blank, runs on DEFAULT_PORT = 12345

Server broadcasts on 127.0.0.1 AND your computer's network IP (usually 192.168.X.Y)
