#include<stdio.h>
#include <dirent.h>
#include <errno.h>
#define LINUX_SYS_CPU_DIRECTORY "/sys/devices/system/cpu"

int main() {
   int cpu_count = 0;
   DIR *sys_cpu_dir = opendir(LINUX_SYS_CPU_DIRECTORY);
   if (sys_cpu_dir == NULL) {
       int err = errno;
       printf("Cannot open %s directory, error (%d).\n", LINUX_SYS_CPU_DIRECTORY, strerror(err));
       return -1;
   }
   const struct dirent *cpu_dir;
   while((cpu_dir = readdir(sys_cpu_dir)) != NULL) {
       if (fnmatch("cpu[0-9]*", cpu_dir->d_name, 0) != 0)
       {
          /* Skip the file which does not represent a CPU */
          continue;
       }
       cpu_count++;
   }
   printf("CPU count: %d\n", cpu_count);
   return 0;
}
