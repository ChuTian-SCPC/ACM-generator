#ifndef _SGPCET_COMMON_H_
#define _SGPCET_COMMON_H_

#include"testlib.h"
#include <sstream>
#include <unordered_map>
#include <thread>
#include <chrono>
#include <sys/stat.h>
#include <queue>
#include <stack>
#include <unordered_set>

#ifdef ON_WINDOWS
#include <direct.h>
#include <windows.h>
#define mkdir(dir, mode) _mkdir(dir)
#define stat _stat 
#else
#include <unistd.h>
#include <limits.h>
#include <dirent.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <csignal>
#endif

#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

#endif // !_SGPCET_COMMON_H_
