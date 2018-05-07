#Sources:
#http://clalance.blogspot.com/2011/01/exiting-python-program.html
#http://www.pythonforbeginners.com/system/python-sys-argv
#https://wiki.python.org/moin/TcpCommunication
#https://docs.python.org/2/howto/sockets.html

import socket
import sys
import os

#Validates user input from start
#Input should be in the form "python chatserve.py <port>"
def validate():
    #check length
    if len(sys.argv) < 2:
        print("Please use the format 'python chatserve.py <port number>' ")
        os._exit(1)
        
    port_num = sys.argv[1]
    
    #Check if the arg is an int
    try:
        port_num_int = int(port_num)
    except ValueError, e:
        print("Please use the format 'python chatserve.py <port number>' ")
        print("Where the port number is an int greater than 4")
        os._exit(1)
        
    
#Sets up a socket
def StartUp():
    #Grab port number
    port_num = int(sys.argv[1])
    
    #Make a new INET STREAMing socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    #Bind it to the port
    sock.bind(('', port_num))
    
    #Listening time
    sock.listen(1)
    
    #Return the socket variable
    return sock

#Receives and prints incoming messages
def receive_message(conn):
    print("**getting a message!**")
    #Grab the message
    new_message = conn.recv(500)
    
    #print the message
    print(new_message)
    
    #If the client typed "quit", return true
    if "\quit" in new_message:
        return True
    else:
        return False
        
#Sends a message from the server        
def send_message(conn, server_name):
    print("**sending a message!**")
    
    #Nice string
    handle_string = server_name + ": "
    #print(handle_string)
    #get message from the user
    new_message = raw_input()
    message = handle_string + new_message
    print(message)
    #Send the message
    conn.send(message)
    
    if "\quit" in message:
        return True
    else:
        return False
        
#Call the function to validate user input
validate()

#Set up a socket
sock = StartUp()

#Keeps the program going until the user uses ctl c to quit
while 1:
    
    print("Hello naughty children, it's server time")
    print("Hanging around until a client contacts us....")
    
    #Ready to accept a connection request from a client
    conn, addr = sock.accept()
    
    while 1:
        #Set arbitrary server username
        server_name = "server"
        
        #Bool for if the connection should quit
        quit = False
        quit = receive_message(conn)
        
        #If the client wants to quit, exit the inner loop
        if quit == True:
            print("The client has quit")
            break
        
        #Send a message from the server
        quit = send_message(conn, server_name)
        
        #If the server wants to quit, exit the inner loop
        if quit == True:
            print("You have quit")
            break
    
    #When the inner loop exits, the connection is closed
    conn.close()
    
    