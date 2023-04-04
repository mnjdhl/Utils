#include <map>
using namespace std;


#define LINUX_SYS_CPU_DIRECTORY "/sys/devices/system/cpu"

typedef struct _cpu_info {
	int cpu_num;
	char cpu_name[255];
	//char *cpu_name;
} cpu_info_t;

class CPUAffi {

private:
	int cpu_size;
	map<int, cpu_info_t> cpu_list;
	bool debug_on;

	void get_cpu_list();
      
public:
	CPUAffi(bool); 

	int GetCPUCount();

	void set_cpu_affinity(int th_num); 

};


