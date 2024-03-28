#include "algorithms.h"
#include "main.h"

bool isSafeState(int* Available, int** Max, int **Allocation, int **Need)
{
    return false;
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
