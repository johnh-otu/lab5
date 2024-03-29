#include "resources.h"
#include "main.h"
#include "algorithms.h"

// Acquire lock
void acquireLock(pthread_mutex_t *lock) {
    pthread_mutex_lock(lock);
}

// Release lock
void releaseLock(pthread_mutex_t *lock) {
    pthread_mutex_unlock(lock);
}

// Check if requested resources can be granted
int canGrantResources(int* Available, int** Need, int** Allocation, int customer_number, int* request) {
    int i;
    for (i = 0; i < NUM_RESOURCES; i++) {
        if (request[i] > Available[i] || request[i] > Need[customer_number][i]) {
            return 0;
        }
    }
    return 1;
}

// Grant requested resources
void grantResources(int* Available, int** Allocation, int** Need, int customer_number, int* request) {
    int i;
    for (i = 0; i < NUM_RESOURCES; i++) {
        Available[i] -= request[i];
        Allocation[customer_number][i] += request[i];
        Need[customer_number][i] -= request[i];
    }
}

// Release allocated resources
void releaseAllocatedResources(int* Available, int** Allocation, int** Need, int customer_number, int* release) {
    int i;
    for (i = 0; i < NUM_RESOURCES; i++) {
        Available[i] += release[i];
        Allocation[customer_number][i] -= release[i];
        Need[customer_number][i] += release[i];
    }
}

// Request resources
int requestResources(int* Available, int** Need, int **Allocation, int customer_number, int* request, pthread_mutex_t lock) {
    acquireLock(&lock);

    if (!canGrantResources(Available, Need, Allocation, customer_number, request)) {
        // Request fail
        releaseLock(&lock);
        return -1;
    }

    // Check if request can be granted without deadlock
    if (!checkSafety(Available, Need, Allocation, customer_number, request)) {
        // Request fail
        releaseLock(&lock);
        return -1;
    }

    // Grant requested resources
    grantResources(Available, Allocation, Need, customer_number, request);

    // Request success
    releaseLock(&lock);
    return 0;
}

// Release resources
int releaseResources(int* Available, int** Need, int **Allocation, int customer_number, int* release, pthread_mutex_t lock) {
    acquireLock(&lock);

    // Check if released resources exceed allocated resources
    int i;
    for (i = 0; i < NUM_RESOURCES; i++) {
        if (release[i] > Allocation[customer_number][i]) {
            // Release fail
            releaseLock(&lock);
            return -1;
        }
    }

    // Release allocated resources
    releaseAllocatedResources(Available, Allocation, Need, customer_number, release);

    // Release success
    releaseLock(&lock);
    return 0;
}
