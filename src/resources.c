#include "resources.h"
#include "main.h"

// request resources - returns 0 on success, -1 on fail; use mutex lock to control access
int requestResources(int* Available, int** Max, int **Allocation, int **Need, int customer_number, int* request, pthread_mutex_t lock)
{
    return -1;
}
// release resources - returns 0 on success, -1 on fail; use mutex lock to control access
int releaseResources(int* Available, int** Max, int **Allocation, int **Need, int customer_number, int* release, pthread_mutex_t lock)
{
    return -1;
}
