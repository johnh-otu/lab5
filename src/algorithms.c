#include "algorithms.h"
#include "main.h"
#include <stdbool.h>

bool isSafeState(int* Available, int** Max, int **Allocation, int **Need)
{
    // Initializing arrays to keep track of state
    int work[NRESOURCES];
    bool finish[NCUSTOMERS];

    // Initialize work and finish passing arrays
    for (int i = 0; i < NRESOURCES; ++i) {
        work[i] = Available[i];
    }
    for (int i = 0; i < NCUSTOMERS; ++i) {
        finish[i] = false;
    }

    // loop to initialize the main safe state sequence
    int count = 0; // Counter for finished processes
    while (count < NCUSTOMERS) {
        bool found = false;
        for (int i = 0; i < NCUSTOMERS; ++i) {
            if (!finish[i]) {  // If process is not finished
                bool can_allocate = true;
                for (int j = 0; j < NRESOURCES; ++j) {
                    if (Need[i][j] > work[j]) {
                        can_allocate = false;
                        break;
                    }
                }
                // Allocating resources to this process
                if (can_allocate) {
                    // Allocate resources
                    for (int j = 0; j < NRESOURCES; ++j) {
                        work[j] += Allocation[i][j];
                    }
                    finish[i] = true; // The process has not finished
                    found = true; // Updating flag
                    count++; // Increment counter for the finished processes
                }
            }
        }
        if (!found) {
            // Break the loop if no processes at all are to be allocated
            break;
        }
    }

    // When all processes have succesfully finished, the system is now in safe state
    for (int i = 0; i < NCUSTOMERS; ++i) {
        if (!finish[i]) {
            return false;
        }
    }
    return true;
}


bool safeToGrant(int* request, int* Available, int** Allocation, int** Need) {
    // Allocate memory for temporary matrices for resource allocation
    int** tempAllocation = (int**)malloc(NCUSTOMERS * sizeof(int*));
    int** tempNeed = (int**)malloc(NCUSTOMERS * sizeof(int*));
    int* tempAvailable = (int*)malloc(NRESOURCES * sizeof(int));

    // Initialize temporary matrices
    for (int i = 0; i < NCUSTOMERS; i++) {
        tempAllocation[i] = (int*)malloc(NRESOURCES * sizeof(int));
        tempNeed[i] = (int*)malloc(NRESOURCES * sizeof(int));
        for (int j = 0; j < NRESOURCES; j++) {
            tempAllocation[i][j] = Allocation[i][j];
            tempNeed[i][j] = Need[i][j];
        }
    }
    for (int i = 0; i < NRESOURCES; i++) {
        tempAvailable[i] = Available[i] - request[i];
    }

    // Update temporary matrices based on the request
    for (int i = 0; i < NRESOURCES; i++) {
        tempAllocation[customer_number][i] += request[i];
        tempNeed[customer_number][i] -= request[i];
    }

    // Check if the state is safe
    bool isSafe = isSafeState(tempAvailable, Max, tempAllocation, tempNeed);

    // Free memory allocated
    for (int i = 0; i < NCUSTOMERS; i++) {
        free(tempAllocation[i]);
        free(tempNeed[i]);
    }
    free(tempAllocation);
    free(tempNeed);
    free(tempAvailable);

    return isSafe;
}
