#include "CPUAffi.h"
#include <stdio.h>
#include <unistd.h>
#include <sched.h>
#include <syscall.h>
#include <string.h>
#include <dirent.h>
#include <fnmatch.h>

#include <iostream>

using namespace std;


CPUAffi::CPUAffi(bool dbg_on) {
	_debug_on = dbg_on;
	_affi_set_cnt = 0;
	get_cpu_list();
}

void CPUAffi::get_cpu_list() {
	int cpu_count = 0, cpu_indx;
	const struct dirent *cpu_dir;
	DIR *sys_cpu_dir;
	map<int, cpu_info_t>::iterator it;
	cpu_info_t ci;

	sys_cpu_dir = opendir(_LINUX_SYS_CPU_PATH);

	if (sys_cpu_dir == NULL) {
		int err = errno;
		printf("Cannot open %s directory, error (%s).\n", _LINUX_SYS_CPU_PATH, strerror(err));
		return;
	}

	while((cpu_dir = readdir(sys_cpu_dir)) != NULL) {

		if (fnmatch("cpu[0-9]*", cpu_dir->d_name, 0) != 0) {
			 /* Let's skip the files which are not CPU names */
			 continue;
		}

		cpu_indx = atoi(strstr(cpu_dir->d_name, "cpu")+3);
		ci.cpu_num = cpu_indx;
		strcpy(ci.cpu_name,  cpu_dir->d_name);
		cpu_list[cpu_count] = ci; 
		cpu_count++;
	}

	_cpu_size = cpu_count;
	if (_debug_on) {
		cout<<"CPU List:"<<endl;
		cout<<"---------"<<endl;
		for (it = cpu_list.begin(); it != cpu_list.end(); ++it) {
			cout<<it->first<<"-->"<<it->second.cpu_num<<"---"<<it->second.cpu_name<<endl;
		}
		printf("CPU count: %d\n", cpu_count);
	}
}

int CPUAffi::get_cpu_count()
{
	
	return _cpu_size;	
}

void CPUAffi::set_cpu_affinity(int cpu_indx, int th_pid) {
	cpu_set_t mask;


	CPU_ZERO(&mask);
	CPU_SET(cpu_list[cpu_indx].cpu_num, &mask);

	int err = sched_setaffinity(th_pid, sizeof(cpu_set_t), &mask);
	if (err == 0) {
		_affi_set_cnt++;
		if (_debug_on) {
			cout <<"set_cpu_affinity():sched_setaffinity() succeeds on CPU " << cpu_indx<<" for the thread "<<th_pid<< endl;
		}
	} else {
		cout <<"set_cpu_affinity():sched_setaffinity() fails for thread "<< th_pid << ", "<< strerror(errno) << endl;
	}

}

void CPUAffi::set_cpu_affinity(int th_num) {

	int th_pid = syscall(SYS_gettid);

	int cpu_indx = th_num % _cpu_size;

	set_cpu_affinity(cpu_indx, th_pid);
}

void CPUAffi::set_cpu_affinity() {

	set_cpu_affinity(_affi_set_cnt);
}
