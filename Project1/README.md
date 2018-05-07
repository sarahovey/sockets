About:
This project is a very simple chat server and client. 

Run chatserve.py, THEN run chatclient.c
The client will send an initial message to the server,
then the server can respond. The hosts take turns messaging
each other.
As of 5/6, 6pm, both hosts still contain some
print statements to say what's happening at any given time.
These will be removed from the final version.

To quit, type "\quit" from either host

To run chatserve.py:
python chatserve.py <port number>
ie python chatserve.py 2000

To compile chatclient.c:
gcc chatclient.c -o chatclient

To run chatclient.c:
./chatclient hostname <port number>
./chatclient localhost 2000

Sources:
http://www.linuxhowtos.org/C_C++/socket.htm
https://beej.us/guide/bgnet/html/multi/index.html
http://clalance.blogspot.com/2011/01/exiting-python-program.html
http://www.pythonforbeginners.com/system/python-sys-argv
https://wiki.python.org/moin/TcpCommunication
https://docs.python.org/2/howto/sockets.html
