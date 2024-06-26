#include "main.h"

#ifndef RESOURCES_HEADER_GUARD
#define RESOURCES_HEADER_GUARD

// request resources - returns 0 on success, -1 on fail; use mutex lock to control access
int requestResources(int* Available, int** Max, int **Allocation, int **Need, int customer_number, int* request);
// release resources - returns 0 on success, -1 on fail; use mutex lock to control access
int releaseResources(int* Available, int** Max, int **Allocation, int **Need, int customer_number, int* release);
// output current state of vectors and matrices
void outputCurrentState(int* Available, int** Max, int** Allocation, int** Need, int customer_number);

#endif