#include "main.h"
#include "algorithms.h"

void initializeVectors();
void outputUsageMessage(char** argv);
bool toInteger(char* str, int* output);
void* thread_runner();

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
	
	//try setting input as available resources
	if (!toInteger(argv[1], &(Available[0])) || !toInteger(argv[2], &(Available[1])) || !toInteger(argv[3], &(Available[2]))) {
		return -1;
	}

	return 0;
}

void* thread_runner()
{
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