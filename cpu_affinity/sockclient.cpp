#include <unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<fcntl.h>
#include <stdio.h>
#include <string.h>
#include<stdlib.h>

#define CONNECTION_PORT 3500

int main()
{
	struct sockaddr_in srv_addr;
	const char* message="This is a message from the client";
	char msg_buf[50];
	char rcv_buf[100];
	int status = 0;
	int sock_fd, cnt;

	/* Create an Ipv4 and TCP socket */
	sock_fd = socket(AF_INET,SOCK_STREAM,0);
	/* Check whether the socket is created successfully */
	if(sock_fd<0)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}  

	/* Initialize address structure for binding */
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(CONNECTION_PORT);
	/* Set address to any address available */
	srv_addr.sin_addr.s_addr =INADDR_ANY;
	srv_addr.sin_zero[8]='\0';

	/* Connect to the server */
	status=connect(sock_fd,(struct sockaddr*)&srv_addr, sizeof(srv_addr));
	if(status<0)
	{
		perror("Couldn't connect with the server");
		exit(EXIT_FAILURE);
	}    

	/* Send 10 messages to the server */
	cnt = 1;
	while (cnt <= 10) {
		sprintf(msg_buf, "%s:%d", message, cnt);
	    write(sock_fd, msg_buf, strlen(msg_buf));

	    /* Receive a message from the server */
	    read(sock_fd, rcv_buf, 100);
	    printf("Message from server: %s\n", rcv_buf);
	    usleep(10);
	    cnt++;
	}

	/* Terminate the socket connection */
	close(sock_fd);
	return 0;
}

