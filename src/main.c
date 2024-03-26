#include "main.h"
#include "resources.h"

void initializeVectors();
void outputUsageMessage(char** argv);
bool toInteger(char* str, int* output);
void* thread_runner(void* args);

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int* Available;
int** Max;
int** Allocation;
int** Need;

int main(int argc, char** argv) 
{
	//check input format
	if (argc != NRESOURCES + 1) {
		outputUsageMessage(argv);
		return -1;
	}
	
	initializeVectors();
	
	//try setting available resources from input
	for (int i = 0; i < NRESOURCES; i++) {
		if (!toInteger(argv[i+1], &(Available[i])))
			return -1; //return if not integer
	}

	pthread_t threadids[NCUSTOMERS] = {};
	int* args; 
	srand(time(NULL)); //set random seed

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
	int* intargs = (int*)args;
	int customer_number = *intargs;
	int r;
	int* request = calloc(NRESOURCES, sizeof(int));
	int* release = calloc(NRESOURCES, sizeof(int));
	
	free(args);

	while(true) 
	{
		/*
		 * 1/5 chance -> request
		 * 1/5 chance -> release
		 * 3/5 chance -> do nothing
		*/
		r = (rand() + customer_number) % 5;
		switch (r) {
			case 0:
				printf("%d: requesting resources\n", customer_number);
				//create request
				for (int i = 0; i < NRESOURCES; i++) { request[i] = rand() % (Need[customer_number][i] + 1); }
				//make request
				requestResources(Available, Max, Allocation, Need, customer_number, request, lock);
				break;
			case 1:
				printf("%d: releasing resources\n", customer_number);
				//create release request
				for (int i = 0; i < NRESOURCES; i++) { release[i] = rand() % (Allocation[customer_number][i] + 1); }
				//release resources
				releaseResources(Available, Max, Allocation, Need, customer_number, release, lock);
				break;
			default:
				break;
		}
		sleep(1);
	}

	return NULL;
}

void initializeVectors()
{
	//allocate Memory to Each Vector/Matrix
	Available = (int *)malloc(NRESOURCES * sizeof(int));
    Max = (int **)malloc(NCUSTOMERS * sizeof(int *));
    Allocation = (int **)malloc(NCUSTOMERS * sizeof(int *));
    Need = (int **)malloc(NCUSTOMERS * sizeof(int *));

	//allocate Memory to Each Vector Within Matrices
    for (int i = 0; i < NCUSTOMERS; i++) {
        Max[i] = (int *)malloc(NRESOURCES * sizeof(int));
        Allocation[i] = (int *)malloc(NRESOURCES * sizeof(int));
        Need[i] = (int *)malloc(NRESOURCES * sizeof(int));
    }
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