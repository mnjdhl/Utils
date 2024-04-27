build:
	 g++ CPUAffi.cpp socksrv_std_thread.cpp sock_common.cpp -o socksrv_std_thread -lpthread
	 g++ sockclient.cpp sock_common.cpp -o sockclient

debug:
	 g++ -g CPUAffi.cpp socksrv_std_thread.cpp sock_common.cpp -o socksrv_std_thread -lpthread
	 g++ -g sockclient.cpp sock_common.cpp -o sockclient

clean:
	rm socksrv_std_thread sockclient
