#include "resources.h"
#include "main.h"
#include "algorithms.h"

int canGrantResources(int* Available, int** Need, int** Allocation, int customer_number, int* request);
void grantResources(int* Available, int** Max, int** Allocation, int** Need, int customer_number, int* request);
void releaseAllocatedResources(int* Available, int** Max, int** Allocation, int** Need, int customer_number, int* release);
void outputCurrentState(int* Available, int** Max, int** Allocation, int** Need, int customer_number);

// Request resources
int requestResources(int* Available, int** Max, int **Allocation, int** Need, int customer_number, int* request) {

    if (!canGrantResources(Available, Need, Allocation, customer_number, request)) {
        // Request fail
        printf("\033[31m%d: RESOURCES NOT AVAILABLE!\n\033[0m", customer_number);
        outputCurrentState(Available, Max, Allocation, Need, customer_number);
        return -1;
    }

    // Check if request can be granted without deadlock
    if (!safeToGrant(request, customer_number, Available, Max, Allocation, Need)) {
        // Request fail
        printf("\033[31m%d: WILL NOT END IN SAFE STATE!\n\033[0m", customer_number);
        outputCurrentState(Available, Max, Allocation, Need, customer_number);
        return -1;
    }

    // Grant requested resources
    grantResources(Available, Max, Allocation, Need, customer_number, request);

    // Request success
    return 0;
}

// Release resources
int releaseResources(int* Available, int** Max, int **Allocation, int** Need, int customer_number, int* release) {

    // Check if released resources exceed allocated resources
    int i;
    for (i = 0; i < NRESOURCES; i++) {
        if (release[i] > Allocation[customer_number][i]) {
            // Release fail
            printf("\033[31m%d: NOT ENOUGH RESOURCES ALLOCATED!\n\033[0m", customer_number);
            outputCurrentState(Available, Max, Allocation, Need, customer_number);
            return -1;
        }
    }

    // Release allocated resources
    releaseAllocatedResources(Available, Max, Allocation, Need, customer_number, release);

    // Release success
    return 0;
}

//============HIDDEN================

// Check if requested resources can be granted
int canGrantResources(int* Available, int** Need, int** Allocation, int customer_number, int* request) {
    int i;
    for (i = 0; i < NRESOURCES; i++) {
        if (request[i] > Available[i] || request[i] > Need[customer_number][i]) {
            return 0;
        }
    }
    return 1;
}

// Grant requested resources
void grantResources(int* Available, int** Max, int** Allocation, int** Need, int customer_number, int* request) {
    int i;
    for (i = 0; i < NRESOURCES; i++) {
        Available[i] -= request[i];
        Allocation[customer_number][i] += request[i];
        Need[customer_number][i] -= request[i];
    }
    printf("\033[32m%d: RESOURCES GRANTED!\n\033[0m", customer_number);
    outputCurrentState(Available, Max, Allocation, Need, customer_number);
}

// Release allocated resources
void releaseAllocatedResources(int* Available, int** Max, int** Allocation, int** Need, int customer_number, int* release) {
    int i;
    for (i = 0; i < NRESOURCES; i++) {
        Available[i] += release[i];
        Allocation[customer_number][i] -= release[i];
        Need[customer_number][i] += release[i];
    }
    printf("\033[32m%d: RESOURCES RELEASED!\n\033[0m", customer_number);
    outputCurrentState(Available, Max, Allocation, Need, customer_number);
}

// Output current state
void outputCurrentState(int* Available, int** Max, int** Allocation, int** Need, int customer_number)
{
    //print contents of Available vector
    printf("Available: ");
    for (int i = 0; i < NRESOURCES; i++) {
        printf("%d ", Available[i]);
    }
    printf("\n");

    //print contents of Max matrix
    printf("Max:\n");
    for (int i = 0; i < NCUSTOMERS; i++) {
        printf("%d: ", i);
        for (int j = 0; j < NRESOURCES; j++) {
            printf("%d ", Max[i][j]);
        }
        printf("\n");
    }

    //print contents of Allocation matrix
    printf("Allocation:\n");
    for (int i = 0; i < NCUSTOMERS; i++) {
        printf("%d: ", i);
        for (int j = 0; j < NRESOURCES; j++) {
            printf("%d ", Allocation[i][j]);
        }
        printf("\n");
    }

    //print contents of Need matrix
    printf("Need:\n");
    for (int i = 0; i < NCUSTOMERS; i++) {
        printf("%d: ", i);
        for (int j = 0; j < NRESOURCES; j++) {
            printf("%d ", Need[i][j]);
        }
        printf("\n");
    }

    printf("\n");
}
