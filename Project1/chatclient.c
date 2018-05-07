//Sources:
//http://www.linuxhowtos.org/C_C++/socket.htm
//https://beej.us/guide/bgnet/html/multi/index.html

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


//Error helper function
void error(const char *msg) {
    perror(msg);
    exit(0);
}


//Connect to the server
int connectToServer(char *hostname, int port, struct hostent *server, struct sockaddr_in serv_addr, int sock){
    // Set up the socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    // Verify new socket
    if (sock < 0) {
        error("Socket not created");
    }

    // Grabs host information
    server = gethostbyname(hostname);

    // Checks for valid host
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    // Clear server address
    bzero((char *) &serv_addr, sizeof(serv_addr));

    // Set fields in server address
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
            (char *)&serv_addr.sin_addr.s_addr,
            server->h_length);

    // Converts the port number in host byte order to network byte order
    serv_addr.sin_port = htons(port);

    // Connects to server and checks for success
    if (connect(sock,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        error("ERROR connecting");
    }
    
    return sock;
}



//Sends a message to the server
int sendMessage(char *handle, int sock) {
    int action;
    char buffer[500];
    char message[500];
    
    //printf("Hello from the top of send message");
    
    //Print handle, wait for the user to type
    printf("Write a message: \n");
    printf("%s : ",handle);
    bzero(message,500);
    fgets(message,500,stdin);
    
    if(message == NULL || message == " "){
            //message = "Hi server!";
            printf("Message is empty");
    }
    
    //printf("Hello from line 72 of send message");


    //If the user wants to quit:
    //TODO: something betterf
    if (buffer[0] == '\\' &&
        buffer[1] == 'q' &&
        buffer[2] == 'u' &&
        buffer[3] == 'i' &&
        buffer[4] == 't') {
            // send back a bool to exit the loop
            //d = 0;
            

            // send "\quit" to the server
            action = write(sock,buffer,strlen(buffer));

            // check if the write was successful
            if (action < 0) {
                error("ERROR writing to socket");
            }

            // close the connection
            close(sock);
            
            //return a 1
            return 1;
    }
    else {

        // Add the handle
        strcat(message,handle);

        // Add the colon
        strcat(message,": ");
     
        // use write() to send the message
        action = write(sock,message,strlen(message));
        
        //printf("action: %d", action);
        
        //Verify that it worked, -1 indicates failure
        if (action < 0) {
            error("ERROR writing to socket");
        }
        else{
            //printf("\n Message sent, wait for reply");
            return 0;
        }
        
    }
    
}



//Gets messages from the server and prints them
//Returns a T/F int based on if the server quit or not
int receiveMessage(int sock) {
    int action;
    char message[500];
    // read from the server
    action = read(sock,message,500);

    //See if the read was successful
    //a -1 means a failure
    if (action < 0) {
        error("ERROR reading from socket");
    }

    //This is hideous
    // Check for quit message
    if (message[6] == '\\' &&
        message[7] == 'q' &&
        message[8] == 'u' &&
        message[9] == 'i' &&
        message[10] == 't') {
            
            // close the connection
            close(sock);
            
            // if so, end the while loop
            printf("The server has quit");
            return 1;

            
    }
    else {

        // write the message from the server
        printf("%s\n",message);
        return 0;
    }
    
    return 0;
}


// main program that talks back and forth the the server
int main(int argc, char *argv[]) {
    
    //Variable set up
    int sock; 
    int portNum;
    
    //Holds address of server
    struct sockaddr_in serv_addr;

    //Struct hostent defined in netdb.h
    struct hostent *server;

    //Validate command line input
    //Must include hostname, and port number
    if(argc != 3){
        fprintf(stderr,"Run as => %s hostname port\n", argv[0]);
        exit(0);
    }
    
    //Handle
    char handle[11];
    int valid = 0;
    while (valid == 0){
        printf("Enter your handle. 10 characters or fewer \n");
        scanf("%s", handle);
        
        //Exit the loop if the handle is valid
        if(handle != NULL || handle != ""){
            valid = 1;
        }
    }

    // Connect to server, return an open socket
    sock = connectToServer(argv[1],atoi(argv[2]), server, serv_addr, sock);

    // gets the user's message, sends it to the server,
    // Wait for a reply until client or server types '\quit'
    int loop = 0;
    printf("Chatting with %s \n", argv[1]);
    while (loop == 0) {
        
        if (loop == 0) {
            // send a message to the server
            loop = sendMessage(handle, sock);
            if(loop == 1){
                printf("You have quit");
            }
        }
        if (loop == 0) {
            // receive a message from the server
            loop = receiveMessage(sock);
            if(loop == 1){
                printf("The server quit");
            }
        }
    }
    return 0;
}