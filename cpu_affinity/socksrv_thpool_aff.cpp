#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/epoll.h>
#include <errno.h>
#include <sched.h>
#include <syscall.h>
#include <dirent.h>
#include <fnmatch.h>
#include <signal.h>

#include <iostream>
#include <queue>
#include <map>

using namespace std;

#define CONNECTION_PORT 3500
#define THREAD_POOL_SZ 3
#define LINUX_SYS_CPU_DIRECTORY "/sys/devices/system/cpu"

/*Global variables */
pthread_attr_t attr;

/* epoll fd */
int epfd;

/* Client socket fd queues, one queue per thread */
queue<int> cfd_que[THREAD_POOL_SZ];
int queindx[THREAD_POOL_SZ];
map<string, int> cpu_list;
int cpu_size;

int GetCPUCount()
{
	cpu_set_t cs;
	
	sched_getaffinity(0, sizeof(cs), &cs);
	
	return CPU_COUNT_S(sizeof(cs), &cs);
}

void get_cpu_list() {
   int cpu_count = 0, cpu_indx;
   const struct dirent *cpu_dir;
   DIR *sys_cpu_dir;
   map<string, int>::iterator it;
   
   sys_cpu_dir = opendir(LINUX_SYS_CPU_DIRECTORY);

   if (sys_cpu_dir == NULL) {
       int err = errno;
       printf("Cannot open %s directory, error (%s).\n", LINUX_SYS_CPU_DIRECTORY, strerror(err));
       return;
   }

   while((cpu_dir = readdir(sys_cpu_dir)) != NULL) {
       if (fnmatch("cpu[0-9]*", cpu_dir->d_name, 0) != 0)
       {
          /* Skip the file which does not represent a CPU */
          continue;
       }

	cpu_indx = atoi(strstr(cpu_dir->d_name, "cpu")+3);
       cpu_list[cpu_dir->d_name] = cpu_indx;
       //cout<<cpu_dir->d_name<<"-"<<cpu_indx<<endl;
       cpu_count++;
   }
   cpu_size = cpu_count;
   cout<<"CPU List:"<<endl;
   cout<<"---------"<<endl;
   for (it = cpu_list.begin(); it != cpu_list.end(); ++it) {
	cout<<it->first<<" --> "<<it->second<<endl;
   }
   printf("CPU count: %d\n", cpu_count);
}

void set_cpu_affinity(int th_num) {
	cpu_set_t mask;
	int th_pid = syscall(SYS_gettid);

	int cpunum = th_num % cpu_size;

	CPU_ZERO(&mask);
	CPU_SET(cpunum, &mask);

	int err = sched_setaffinity(th_pid, sizeof(cpu_set_t), &mask);
	if (err == 0) {
		cout <<"set_cpu_affinity():sched_setaffinity() succeeds on CPU " << cpunum<<" for the thread "<<th_pid<< endl;
	} else {
		cout <<"set_cpu_affinity():sched_setaffinity() fails, " << strerror(errno) << endl;
	}

}

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
	    //int err = epoll_ctl(epfd, EPOLL_CTL_DEL, sfd, NULL);
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

void handle_client_request(int cfd) {
/* Buffer to store message */
    char msg_buf[512];
    char* message = "Hello Client!! This is a message from your server!";
    int rsz;

	/* Receive data sent by the client */
    	
    	while (true) {
	    //bytes_read = read(cfd, msg_buf, sizeof(msg_buf));
		rsz = recv(cfd, msg_buf, sizeof(msg_buf), MSG_DONTWAIT);

	    if (rsz <= 0)
	    {
		    //remove_socket(cfd);
		    //return;
		    break;
	    }
	/* Set the last index of the character array as a null character */
	    msg_buf[rsz] = '\0';
	    printf("Message (%d bytes) from client: %s \n", rsz, msg_buf);
	}

	usleep(10);
	/* Send data to the client */
	send(cfd, message, strlen(message), 0);

}

void do_sig_mask() {

	struct sigaction sa;

	memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = SIG_IGN;
	sigaction(SIGPIPE, &sa, NULL);
}

void *request_handler_thread(void *param) {

	int fd;
	int qindx = *(int *)param;
	int th_pid = syscall(SYS_gettid);

	//do_sig_mask();

	set_cpu_affinity(qindx);

	cout << "Started Request Handler Thread "<< th_pid << " with Queue Index = " << qindx <<endl;

	while(true) {

		if (!cfd_que[qindx].empty()) {
			fd = cfd_que[qindx].front();
			cfd_que[qindx].pop();
			if (is_closed(fd))
			{
				//cout << "request_handler_thread(" <<qindx <<"):removing socket = " << fd << endl;
				remove_socket(fd);
				continue;
			}

			//cout << "request_handler_thread(" <<qindx <<"):Handling Request for fd = " << fd << endl;
			handle_client_request(fd);
		} else {
			usleep(100);
		}

	}
}


void *poller_thread(void *) {
	struct epoll_event events[5];
	sigset_t sigmask;
	int qi;

	cout << "Started Poller Thread..." <<endl;
	 while(true){
		int nfds = epoll_pwait(epfd, events, 5, 10000, &sigmask);

		for(int i=0;i<nfds;i++) {
			//cout <<"Pushing to queue" << i%3 <<endl;
			qi = events[i].data.fd % THREAD_POOL_SZ;
			cfd_que[qi].push(events[i].data.fd);
		}
	  }

}

int main()
{

	pthread_t threads[THREAD_POOL_SZ + 1];
	int srv_sfd;
	int cli_sfd;
	int addr_len;
	int opt_val = 1;
	struct sockaddr_in srv_addr;
	struct sockaddr_in cli_addr;

	cout <<"CPU Count = " << GetCPUCount <<endl;
	get_cpu_list();

	/* Creating the socket with IPv4 domain and TCP protocol */
	srv_sfd = socket(AF_INET, SOCK_STREAM, 0);
	/* Check if the socket is created successfully */
	if (srv_sfd<0)
	{
		perror("Socket creation failed");
		/* EXIT_FAILURE is a macro used to indicate unsuccessful execution of the program */
		exit(EXIT_FAILURE);
	}     

	/* Set options for the socket */
	int status=setsockopt(srv_sfd, SOL_SOCKET,SO_REUSEADDR , &opt_val,sizeof(opt_val));
	/* Check if options are set successfully */
	if (status<0){
		perror("Couldn't set options");
		exit(EXIT_FAILURE);
	}
	/* Initializing structure elements for address */
	srv_addr.sin_family = AF_INET;
	/* Convert port to network byte order using htons */
	srv_addr.sin_port = htons(CONNECTION_PORT);
	/* Any address available */
	srv_addr.sin_addr.s_addr = INADDR_ANY;
	/* Assigning null character to the last index of the character array */
	srv_addr.sin_zero[8]='\0';
	/* bind the socket with the values address and port from the sockaddr_in structure */
	status=bind(srv_sfd, (struct sockaddr*)&srv_addr, sizeof(struct sockaddr));
	/* Check if the binding was successful */
	if (status<0) {
		perror("Couldn't bind socket");
		exit(EXIT_FAILURE);
	}

	do_sig_mask();

	/* Create epoll fd, poller thread and client request handler threads */
	pthread_attr_init(&attr);
	epfd = epoll_create(10);
	pthread_create(&threads[0], &attr, poller_thread, NULL);
	for(int i=1;i<(THREAD_POOL_SZ + 1);i++) {
		queindx[i-1] = i-1;
		pthread_create(&threads[i], &attr, request_handler_thread, (void *)&queindx[i-1]);
	}
	//pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	while (true) {

	    /* Listen on specified port with a maximum of 4 requests */
	    status = listen(srv_sfd, 4);
	    
	    /* Check if the socket is listening successfully */
	    if (status<0) {
		perror("Couldn't listen for connections");
		exit(EXIT_FAILURE);
	    }
	    addr_len = sizeof(cli_addr);
	    
	    //pthread_create(&threads[0], &attr, handle_client_request, (void *)&cli_sfd);
	    //handle_client_request(cli_sfd);
	    //pthread_join(threads[0], NULL);

	    /* Accept connection signals from the client */
	    cli_sfd = accept(srv_sfd, (struct sockaddr*)&cli_addr, (socklen_t*)&addr_len);
	    add_socket(cli_sfd);
	}

	pthread_attr_destroy(&attr);
	close(srv_sfd);
	return 0;
}

