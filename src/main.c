#include "main.h"
#include "resources.h"

int initializeVectors(char** argv);
void outputUsageMessage(char** argv);
bool toInteger(char* str, int* output);
void* thread_runner(void* args);

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int* Available;
int** Max;
int** Allocation;
int** Need;

bool AllZeros_FLAG;

int main(int argc, char** argv) 
{
	//check input format
	if (argc != NRESOURCES + 1) {
		outputUsageMessage(argv);
		return -1;
	}
	
	srand(time(NULL)); //set random seed

	//try initializing vectors/matrices
	if (initializeVectors(argv) < 0) { 
		return -1;
	}

	pthread_t threadids[NCUSTOMERS] = {};
	int* args; 

	//start threads
	for (int i = 0; i < NCUSTOMERS; i++) {
		args = malloc(sizeof(int));
		*args = i;
		pthread_create(&(threadids[i]), NULL, thread_runner, (void*)args);
	}

	//join threads
	for (int i = 0; i < NCUSTOMERS; i++) {
		pthread_join(threadids[i], NULL);
	}
	return 0;
}

void* thread_runner(void* args)
{
	//set up variables
	int* intargs = (int*)args;
	int customer_number = *intargs;
	int r;
	int* request = calloc(NRESOURCES, sizeof(int));
	int* release = calloc(NRESOURCES, sizeof(int));
	
	free(args);

	//loop continuously
	while(true) 
	{
		r = (rand() + customer_number) % 4;
		/*
		 * 1/4 chance -> request
		 * 1/4 chance -> release
		 * 2/4 chance -> do nothing
		*/
		
		switch (r) {
			case 0:

				//acquire lock
				pthread_mutex_lock(&lock);

				//create request
				AllZeros_FLAG = true;
				for (int i = 0; i < NRESOURCES; i++) { 
					request[i] = rand() % (Need[customer_number][i] + 1); //random % Need + 1 ----> [0,Need]
					if (request[i] != 0)
						AllZeros_FLAG = false;
				}

				//remove empty requests
				if (AllZeros_FLAG) {
					pthread_mutex_unlock(&lock);
					break;
				}
				
				//print request details
				printf("%d: requesting resources: ", customer_number);
				for (int i = 0; i < NRESOURCES; i++) {  
					printf("%d ", request[i]);
				}
				printf("\n");

				//send request to banker
				requestResources(Available, Max, Allocation, Need, customer_number, request);

				//release lock
				pthread_mutex_unlock(&lock);
				break;

			case 1:

				//acquire lock
				pthread_mutex_lock(&lock);

				//create release request
				AllZeros_FLAG = true;
				for (int i = 0; i < NRESOURCES; i++) { 
					release[i] = rand() % (Allocation[customer_number][i] + 1); //random % Allocation + 1 ----> [0,Allocation]
					if (release[i] != 0)
						AllZeros_FLAG = false;
				}

				//remove empty requests
				if (AllZeros_FLAG) { 
					pthread_mutex_unlock(&lock);
					break;
				}

				//print release request details
				printf("%d: releasing resources: ", customer_number);
				for (int i = 0; i < NRESOURCES; i++) {  
					printf("%d ", release[i]);
				}
				printf("\n");

				//ask banker to release resources
				releaseResources(Available, Max, Allocation, Need, customer_number, release);

				//release lock
				pthread_mutex_unlock(&lock);
				break;

			default:
				break; //no action
		}

		sleep(1);
	}

	return NULL;
}

int initializeVectors(char** argv)
{
	//allocate Memory to Each Vector/Matrix
	Available = (int *)malloc(NRESOURCES * sizeof(int));
    Max = (int **)malloc(NCUSTOMERS * sizeof(int *));
    Allocation = (int **)malloc(NCUSTOMERS * sizeof(int *));
    Need = (int **)malloc(NCUSTOMERS * sizeof(int *));

	//try setting available resources from input
	for (int i = 0; i < NRESOURCES; i++) {
		if (!toInteger(argv[i+1], &(Available[i])))
			return -1; //return if not integer
	}

	//allocate Memory to Each Vector Within Matrices
    for (int i = 0; i < NCUSTOMERS; i++) {
        Max[i] = (int *)malloc(NRESOURCES * sizeof(int));
        Allocation[i] = (int *)malloc(NRESOURCES * sizeof(int));
        Need[i] = (int *)malloc(NRESOURCES * sizeof(int));

		//set matrix values for current row "i"
		for (int j = 0; j < NRESOURCES; j++)
		{
			if (Available[j] > 0) {
				Max[i][j] = rand() % Available[j];
			}
			else {
				Max[i][j] = 0;
			}
			Need[i][j] = Max[i][j];
			Allocation[i][j] = 0;
		}
		
    }

	return 0;
}

void outputUsageMessage(char** argv)
{
	char* output = calloc(512, sizeof(char));
	output[0] = '\0';
	
	strcat(output, "\033[31mUsage: ");
	strcat(output, argv[0]);

	char* stri = calloc(10, sizeof(char));

	for(int i = 0; i < NRESOURCES; i++)
	{
		snprintf( stri, 10, "%d", i+1 ); //convert i+1 to string
		strcat(output, " <max resource #");
		strcat(output, stri);
		strcat(output, ">");
	}

	free(stri);
	strcat(output, "\n\033[0m\0");
	printf(output);
	free(output);
}

bool toInteger(char* str, int* output) 
{
	int len = strlen(str);
    for(int i = 0; i < len; i++) {
        //check if the character is not a digit
        if(!isdigit(str[i]))
		{
			printf("\033[31mError: Invalid integer \"%s\"\n\033[0m", str);
            return false;
		}
    }

	//convert to integer and return
	*output = atoi(str);
    return true;
}