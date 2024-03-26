#include "main.h"

#ifndef ALGORITHMS_HEADER_GUARD
#define ALGORITHMS_HEADER_GUARD

//SAFETY ALGORITHM
bool isSafeState(int* Available, int** Max, int **Allocation, int **Need, pthread_mutex_t lock);
//RESOURCE SAFE-TO-REQUEST ALGORITHM
bool safeToGrant(int* request, int* Available, int **Allocation, int **Need, pthread_mutex_t lock);

#endif