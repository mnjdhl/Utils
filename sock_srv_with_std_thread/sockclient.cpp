#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sock_common.h"

#define CONNECTION_PORT 3500

int main()
{
	struct sockaddr_in srv_addr;
	const char* message="This is a message from the client";
	char msg_buf[512];
	char rcv_buf[512];
	int status = 0, rsz;
	int sock_fd, cnt;

	do_sig_ignore();

	/* Create an Ipv4 and TCP socket */
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	/* Check whether the socket is created successfully */
	if(sock_fd < 0)
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
	if(status < 0)
	{
		perror("Couldn't connect with the server");
		exit(EXIT_FAILURE);
	}    

    printf("\nConnected to Server...");
	/* Send 10 messages to the server */
	cnt = 1;

	while (cnt <= 10) {
		sprintf(msg_buf, "%s:%d", message, cnt);
	    write(sock_fd, msg_buf, strlen(msg_buf));

	    /* Receive a message from the server */
	    rsz = read(sock_fd, rcv_buf, 512);
        if (rsz > 0) {
            rcv_buf[rsz] = '\0';
	        printf("\nMessage from server: %s\n", rcv_buf);
	        //usleep(10);
        }
	    cnt++;
	}

    printf("\nDone\n");
	/* Terminate the socket connection */
	close(sock_fd);
	return 0;
}

