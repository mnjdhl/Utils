// provides required data types
#include <sys/types.h>
// holds address family and socket functions
#include <sys/socket.h>
// has the sockaddr_in structure
#include <netinet/in.h>
// has functions for read and write operations
#include <fcntl.h>
// basic C header
#include <stdio.h>
// header to help with strings
#include <string.h>
// has macros such as EXIT_FAILURE
#include<stdlib.h>
#include <pthread.h>
#include <sys/epoll.h>

// port through which connection is to be made
#define CONNECTION_PORT 3500

pthread_attr_t attr;
int epfd;

void *handle_client_request(void *param) {
// buffer to store message
    char storage_buffer[80];
    char* message = "This is a message from the server";
    int client_socket = *(int *)param;

	// check if the server is accepting the signals from the client
	    if (client_socket<0) { 
		perror("Couldn't establish connection with client");
		exit(EXIT_FAILURE);
	    }
	// Receive data sent by the client
	    int bytes_read = read(client_socket, storage_buffer, 80);

	    if (bytes_read == 0)
	    {
		     static struct epoll_event ev;

		    ev.data.fd = client_socket;
		    ev.events = EPOLLIN;
	    	    epoll_ctl(epfd, EPOLL_CTL_DEL, ev.data.fd, &ev);
		    close(client_socket);	
		    return;
	    }
	// set the last index of the character array as a null character
	    storage_buffer[bytes_read] = '\0';
	    sleep(2);
	    printf("Message from client: %s \n",storage_buffer);
	// Send data to the client
	    send(client_socket, message, strlen(message), 0);
	// Close all the sockets created 

}

void *poller_thread(void *) {
	struct epoll_event events[5];
	pthread_t threads[5];
	sigset_t sigmask;

	 while(1){
		printf("\nRound again");
		int nfds = epoll_pwait(epfd, events, 5, 10000, &sigmask);

		for(int i=0;i<nfds;i++) {
	    		pthread_create(&threads[i], &attr, handle_client_request, (void *)&events[i].data.fd);
		}
	  }

}

int main()
{
	pthread_t threads[1];
// server socket 
    int socket_descriptor;
// socket created by the bind function
    int client_socket;
    int length_of_address;
// option value for respective option_name
    int option_value=1;
// server and client address structures
    struct sockaddr_in server_address;
    struct sockaddr_in connection_address;
// creating the socket with IPv4 domain and TCP protocol
    socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
// Check if the socket is created successfully
    if (socket_descriptor<0)
   {
        perror("Socket creation failed");
// EXIT_FAILURE is a macro used to indicate unsuccessful execution of the program
        exit(EXIT_FAILURE);
   }     
// set options for the socket
    int status=setsockopt(socket_descriptor, SOL_SOCKET,SO_REUSEADDR , &option_value,sizeof(option_value));
// check if options are set successfully
    if (status<0){
        perror("Couldn't set options");
        exit(EXIT_FAILURE);
    }
//  initializing structure elements for address
    server_address.sin_family = AF_INET;
// convert port to network byte order using htons
    server_address.sin_port = htons(CONNECTION_PORT);
// any address available
    server_address.sin_addr.s_addr = INADDR_ANY;
// Assigning null character to the last index of the character array
    server_address.sin_zero[8]='\0';
// bind the socket with the values address and port from the sockaddr_in structure
    status=bind(socket_descriptor, (struct sockaddr*)&server_address, sizeof(struct sockaddr));
// check if the binding was successful
    if (status<0) {
        perror("Couldn't bind socket");
        exit(EXIT_FAILURE);
    }

 pthread_attr_init(&attr);
  epfd = epoll_create(10);
  pthread_create(&threads[0], &attr, poller_thread, NULL);
  //pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    while (1) {
	// listen on specified port with a maximum of 4 requests
	    status = listen(socket_descriptor, 4);
	// check if the socket is listening successfully
	    if (status<0) {
		perror("Couldn't listen for connections");
		exit(EXIT_FAILURE);
	    }
	    length_of_address = sizeof(connection_address);
	// accept connection signals from the client
	    //pthread_create(&threads[0], &attr, handle_client_request, (void *)&client_socket);
	    //handle_client_request(client_socket);
	    //pthread_join(threads[0], NULL);
	    static struct epoll_event ev;

	    client_socket = accept(socket_descriptor, (struct sockaddr*)&connection_address, &length_of_address);
	    ev.data.fd = client_socket;
	    ev.events = EPOLLIN;
	    epoll_ctl(epfd, EPOLL_CTL_ADD, ev.data.fd, &ev);
    }

   pthread_attr_destroy(&attr);
   close(socket_descriptor);
    return 0;
}

