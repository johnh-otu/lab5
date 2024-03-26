#include "dispatcher.h"
#include "colours.h"

char* colours[] = {RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN};
#define N_COLOUR 6
#define NORMAL "\033[0m"
char* current_colour;

static int signal_SIGINT = FALSE;
static int signal_SIGTSTP = FALSE;
static int signal_SIGCONT = FALSE;

static void sighandler(int);
void dispatcherPrint(pid_t pid, int job_id, char* string);
void dispatcherPrintTick(pid_t pid, int job_id, int tick);

/********************************************************************/
//         Based on the sigtrap.c file from labs 7 and 8            //
/********************************************************************/

int main(int argc, char* argv[]) 
{
	pid_t pid = getpid();
	int i, cycle, rc, job_id;
	long clktck = sysconf(_SC_CLK_TCK);
	struct tms t;
	clock_t starttick, stoptick;
	sigset_t mask;

	current_colour = colours[pid % N_COLOUR]; //select colour

	if (argc > 3 || (argc == 2 && !isdigit((int)argv[1][0])) || (argc == 3 && !isdigit((int)argv[1][0]) && !isdigit((int)argv[2][0]) ))
	{
		printf("usage: %s [seconds] [job_id]\n", argv[0]);
		fflush(stdout);
		exit(1);
	}

	//connect signals to handler
	signal(SIGINT, sighandler);
	signal(SIGTSTP, sighandler);
	
	rc = setpriority(PRIO_PROCESS, 0, 20); //lower process priority
	cycle = argc < 2 ? 1 : atoi(argv[1]); //get tick count
	job_id = argc < 3 ? pid : atoi(argv[2]); //get job_id

	dispatcherPrint(pid, job_id, "PROCESS START");

	for (i = 0; i < cycle;) {
		
		if (signal_SIGCONT) { //if continuing from previous suspension
			signal_SIGCONT = FALSE;
			dispatcherPrint(pid, job_id, "SIGCONT");
		}

		//tick
		starttick = times (&t);
		rc = sleep(1);
		stoptick = times (&t);

		if (rc == 0 || (stoptick-starttick) > clktck/2) //print tick
			dispatcherPrintTick(pid, job_id, ++i);
		
		if (signal_SIGINT) { //if interrupt signal received
			dispatcherPrint(pid, job_id, "SIGINT");
			exit(0);
		}

		if (signal_SIGTSTP) { //if suspend signal received 
			signal_SIGTSTP = FALSE;
			dispatcherPrint(pid, job_id, "SIGTSTP");
			sigemptyset (&mask); //unblock if necessary
			sigaddset (&mask, SIGTSTP);
			sigprocmask (SIG_UNBLOCK, &mask, NULL);
			signal(SIGTSTP, SIG_DFL); //reset trap to default
			raise(SIGTSTP); //suspend
			signal(SIGTSTP, sighandler); //reset trap to handler
			signal_SIGCONT = TRUE; //set continue flag on continue
	
		}
		fflush(stdout);

	}
	exit(0);
}

/********************************************************************/

void dispatcherPrint(pid_t pid, int job_id, char* str)
{
	if ((int)pid == job_id)
		printf("%s%7d; %s%s\n", current_colour, (int)pid, str, WHITE);
	else
		printf("%s%7d) %7d; %s%s\n", current_colour, job_id, (int)pid, str, WHITE);
	
	fflush(stdout);
}
void dispatcherPrintTick(pid_t pid, int job_id, int tick)
{
	if ((int)pid == job_id)
		printf("%s%7d; tick %d%s\n", current_colour, (int)pid, tick, WHITE);
	else
		printf("%s%7d) %7d; %d%s\n", current_colour, job_id, (int)pid, tick, WHITE);
}

/********************************************************************/

static void sighandler(int sig)
{
	switch (sig) {
		case SIGINT:
			signal_SIGINT = TRUE;
			break;
		case SIGTSTP:
			signal_SIGTSTP = TRUE;
			break;
	}
}


