#include <atomic>
#include <map>
using namespace std;


#define _LINUX_SYS_CPU_PATH "/sys/devices/system/cpu"

typedef struct _cpu_info {
	int cpu_num;
	char cpu_name[255];
	//char *cpu_name;
} cpu_info_t;

class CPUAffi {

private:
	int _cpu_size;
	map<int, cpu_info_t> cpu_list;
	atomic_uint32_t _affi_set_cnt;
	bool _debug_on;

	void get_cpu_list();
	void set_cpu_affinity(int cpu_indx, int th_pid); 
      
public:
	CPUAffi(bool); 

	int get_cpu_count();

	void set_cpu_affinity(int th_num); 
	void set_cpu_affinity(); 

};


