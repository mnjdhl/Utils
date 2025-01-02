#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <errno.h>
#include <sched.h>
#include <syscall.h>
#include <dirent.h>
#include <fnmatch.h>

#include <iostream>
#include <queue>
#include <map>
#include <vector>
#include <thread>
#include <mutex>
#include <future>
#include "CPUAffi.h"
#include "sock_common.h"

using namespace std;

#define THREAD_POOL_SZ 3
#define LINUX_SYS_CPU_DIRECTORY "/sys/devices/system/cpu"

/* epoll fd */
int epfd;

/* Client socket fd queues, one queue per thread */
int _thread_pool_sz;
queue<int> *cfd_que;
int *queindx;
vector<std::thread> threads;
std::mutex que_mtx;

CPUAffi cpu_affi(false);
uint16_t conn_port = 0;

void add_socket(int cfd) {
	static struct epoll_event ev;

	/* Check if the server is accepting the signals from the client */
	    if (cfd < 0) { 
		perror("Couldn't establish connection with client");
		exit(EXIT_FAILURE);
	    }
	    ev.data.fd = cfd;
	    ev.events = EPOLLIN;
	    int err = epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);
	    if (err == -1) {
		cout << "add_socket():epoll_ctl() failed during socket add with error - " << strerror(errno) << endl;
	    }

}

void remove_socket(int sfd) {
	static struct epoll_event ev;

	    ev.data.fd = sfd;
	    ev.events = EPOLLIN;
	    /* int err = epoll_ctl(epfd, EPOLL_CTL_DEL, sfd, NULL); */
	    int err = epoll_ctl(epfd, EPOLL_CTL_DEL, sfd, &ev);
	    if (err == -1 && errno != EBADF) {
		cout << "remove_socket():epoll_ctl() failed during socket delete with error - " << strerror(errno) << endl;
	    } else {
		    close(sfd);	
	    }

}

bool is_closed(int sfd) {

	char buf[100];
	int rsz;

	rsz = recv(sfd, buf, sizeof(buf), MSG_PEEK | MSG_DONTWAIT);
	if (rsz == 0)
		return true;

	if (rsz == -1 && errno == EBADF)
		return true;

	return false;

}

int handle_client_request(int cfd) {
/* Buffer to store message */
    char msg_buf[512];
    char snd_buf[512];
    const char* message = "Hello Client!! This is a message from your server!";
    int rsz, ssz;

	/* Receive data sent by the client */

    rsz = recv(cfd, msg_buf, sizeof(msg_buf), 0);

    if (rsz <= 0)
    {
        //remove_socket(cfd);
        return -1;
        //break;
    }
    /* Set the last index of the character array as a null character */
    msg_buf[rsz] = '\0';
    printf("Message (%d bytes) from client %d: %s \n", rsz, cfd, msg_buf);

	/* Send data to the client */
    sprintf(snd_buf, "%d::%s", cfd, message);
	ssz = send(cfd, snd_buf, strlen(snd_buf), 0);

    if (ssz <= 0)
        return -1;

    return 0;

}

void request_handler_thread(int qindx) {

	int fd;
	int th_pid = syscall(SYS_gettid);
    std::future<int> fut;


	//cpu_affi.set_cpu_affinity(qindx);
	cpu_affi.set_cpu_affinity();

	cout << "Started Request Handler Thread "<< th_pid << " with Queue Index = " << qindx <<endl;

	while(true) {

		if (!cfd_que[qindx].empty()) {
            que_mtx.lock();
			fd = cfd_que[qindx].front();
			cfd_que[qindx].pop();
            que_mtx.unlock();
			if (is_closed(fd))
			{
				//cout << "request_handler_thread(" <<qindx <<"):removing socket = " << fd << endl;
				remove_socket(fd);
				continue;
			}

			//cout << "request_handler_thread(" <<qindx <<"):Handling Request for fd = " << fd << endl;
            /* Returns the future object 'fut' which can be reffered later */
			fut = std::async(std::launch::async, handle_client_request, fd);
		} else {
			usleep(100);
		}

	}
}


void poller_thread() {
	struct epoll_event events[5];
	sigset_t sigmask;
	int qi;

	cout << "Started Poller Thread..." <<endl;
	 while(true){
		int nfds = epoll_pwait(epfd, events, 5, 10000, &sigmask);

		for(int i=0;i<nfds;i++) {
			//cout <<"Pushing to queue" << i%3 <<endl;
			qi = events[i].data.fd % _thread_pool_sz;
            que_mtx.lock();
			cfd_que[qi].push(events[i].data.fd);
            que_mtx.unlock();
		}
	  }

}

void do_start_socksrv() {
	int srv_sfd;
	int cli_sfd;
	int addr_len;
	int opt_val = 1;
	struct sockaddr_in srv_addr;
	struct sockaddr_in cli_addr;

	/* Creating the socket with IPv4 domain and TCP protocol */
	srv_sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (srv_sfd < 0)
	{
		perror("Socket creation failed");
		/* EXIT_FAILURE is a macro used to indicate unsuccessful execution of the program */
		exit(EXIT_FAILURE);
	}     

	/* Set options for the socket */
	int status = setsockopt(srv_sfd, SOL_SOCKET,SO_REUSEADDR , &opt_val,sizeof(opt_val));
	if (status < 0){
		perror("Couldn't set options");
		exit(EXIT_FAILURE);
	}
	/* Initializing structure elements for address */
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(conn_port);
	srv_addr.sin_addr.s_addr = INADDR_ANY;
	srv_addr.sin_zero[8] = '\0';

	/* bind the socket with the values address and port from the sockaddr_in structure */
	status = bind(srv_sfd, (struct sockaddr*)&srv_addr, sizeof(struct sockaddr));
	if (status < 0) {
		perror("Couldn't bind socket");
		exit(EXIT_FAILURE);
	}

	/* Let's loop indefinitely, waiting for client requests */
	while (true) {

	    /* Listen on specified port with a maximum of 4 requests */
	    status = listen(srv_sfd, 4);
	    
	    if (status<0) {
		perror("Couldn't listen for connections");
		exit(EXIT_FAILURE);
	    }
	    addr_len = sizeof(cli_addr);
	    

	    /* Accept connection signals from the client */
	    cli_sfd = accept(srv_sfd, (struct sockaddr*)&cli_addr, (socklen_t*)&addr_len);
	    add_socket(cli_sfd);
	}

	close(srv_sfd);


}

void do_start_threads() {
	/* Create epoll fd, poller thread and client request handler threads */
	epfd = epoll_create(10);
	/* Create the poller thread which listens for new connection requests */
    threads[0] = std::thread(poller_thread);
	/* Create the handler threads which handle requests from existing connections */
	for(int i = 1;i<(_thread_pool_sz + 1);i++) {
        threads[i] = std::thread(request_handler_thread, i-1);
	}
}

void init() {

	_thread_pool_sz = cpu_affi.get_cpu_count();
	cfd_que = new queue<int>[_thread_pool_sz];
	//threads = new pthread_t[_thread_pool_sz + 1];
    threads.resize(_thread_pool_sz + 1);
}


int main(int argc, char *argv[])
{

    if (argc > 1)
        conn_port = atoi(argv[1]);

    if (conn_port == 0)
        conn_port = CONNECTION_PORT;

	do_sig_ignore();

	init();

	do_start_threads();

	do_start_socksrv();

	for(int i = 0;i<(_thread_pool_sz + 1);i++) {
        threads[i].join();
    }

    threads.clear();

	return 0;
}

