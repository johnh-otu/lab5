#include "main.h"

#ifndef RESOURCES_HEADER_GUARD
#define RESOURCES_HEADER_GUARD

// request resources - returns 0 on success, -1 on fail; use mutex lock to control access
int requestResources(int* Available, int** Max, int **Allocation, int **Need, int customer_number, int* request, pthread_mutex_t lock);
// release resources - returns 0 on success, -1 on fail; use mutex lock to control access
int releaseResources(int* Available, int** Max, int **Allocation, int **Need, int customer_number, int* release, pthread_mutex_t lock);

#endif