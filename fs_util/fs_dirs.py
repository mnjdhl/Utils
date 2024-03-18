#Author: Manoj Dahal
#This code is compatible with Python3

import os
import sys


def get_dir_size(dpath):
    print("Getting the Size for " + dpath)
    tot_sz = 0
    for root, dirs, files, rootfd in os.fwalk(dpath):
        tot_sz += sum([os.stat(name, dir_fd=rootfd).st_size for name in dirs])
        tot_sz += sum([os.stat(name, dir_fd=rootfd).st_size for name in files])
    print("Size of the dir is " + str(tot_sz) + " bytes!")


dir_path = "."
if len(sys.argv) > 1:
    dir_path = sys.argv[1]

if os.access(dir_path, os.F_OK):
    get_dir_size(dir_path)
else:
    print("Error:Invalid Path!")
