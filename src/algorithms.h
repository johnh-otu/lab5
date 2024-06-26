#include "main.h"

#ifndef ALGORITHMS_HEADER_GUARD
#define ALGORITHMS_HEADER_GUARD

//SAFETY ALGORITHM
bool isSafeState(int* Available, int** Max, int **Allocation, int **Need);
//RESOURCE SAFE-TO-REQUEST ALGORITHM
bool safeToGrant(int* request, int customer_number, int* Available, int** Max, int** Allocation, int** Need);

#endif