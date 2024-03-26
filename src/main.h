#ifndef DISPATCHER_HEADER_GUARD
#define DISPATCHER_HEADER_GUARD

#include <pthread.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#define QUANTUM1 2
#define QUANTUM2 4
#define NPRINTERS 2
#define NSCANNERS 1
#define NMODEMS 1
#define NCDDRIVES 2
#define TOTALMEMORYMB 1024
#define MAXRTMEMORY 64

#define MAXPATHSIZE 100

#endif
