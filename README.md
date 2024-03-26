# SOFE3950U Operating Systems - Lab 4: The HOST Dispatcher

## How to install / run
1. Download code as zip file on a linux system (wsl works fine), then extract/unzip
2. Open the new directory in a linux shell (bash, zsh, etc.)
3. Run the "make" command in the directory
4. Run HOST.exe once it's been built

## Code Organization

```bash
.
├── HOST.exe [git ignored]
├── Makefile
├── README.md
├── dispatcher.exe [git ignored]
├── jobs.txt
└── src
    ├── data_structures
    │   ├── heap.c
    │   ├── heap.h
    │   ├── queue.c
    │   └── queue.h
    ├── dispatcher
    │   ├── colours.h
    │   ├── dispatcher.c
    │   └── dispatcher.h
    ├── loader
    │   ├── loader.c
    │   └── loader.h
    ├── main.c
    ├── main.h
    └── schedulers
        ├── LTscheduler.c
        ├── LTscheduler.h
        ├── MTscheduler.c
        ├── MTscheduler.h
        ├── STscheduler.c
        └── STscheduler.h
```

| File              | Description                                                                              |
|-------------------|------------------------------------------------------------------------------------------|
| main.c/main.h     | The main function and libraries that will be used by the whole program                   |
| jobs.txt          | Example text file holding all jobs to be executed; see "Dispatch List"                                    |
| loader.c/loader.h | Loads jobs from jobs.txt and puts them in dispatch queue on arrival time                 |
| heap.c/heap.h     | Adds functionality for heaps; used in loader.c                                           |
| queue.c/queue.h   | Adds functionality for queues; used to represent waiting queues in the dispatcher system |
| LTscheduler.c/LTscheduler.h | Takes jobs from dispatch queue and sorts them into the real-time queue or user job queue based on priority |
| MTscheduler.c/MTscheduler.h | Takes jobs from user job queue and places them in priority queues based on priority |
| dispatcher.c/dispatcher.h | Simulates a program running on the CPU |
| STscheduler.c/STscheduler.h | Checks for jobs in real-time queue and priority queues, then if enough resources are available, runs the process via dispatcher.exe, and swaps processes based on their scheduling algorithms |

## Lab Manual
The Hypothetical Operating System Testbed (HOST) is a multiprogramming system with a
four-level priority process dispatcher operating within the constraints of finite available
resources.

### Four-Level Priority Dispatcher
The dispatcher operates at four priority levels:
1. Real-Time processes that must be run immediately on a First Come First Served
(FCFS) basis, pre-empting any other processes running with lower priority. These
processes are run till completion.
2. Normal user processes are run on a three-level feedback dispatcher. The basic
timing quantum of the dispatcher is 1 second. This is also the value for the time
quantum of the feedback scheduler.

![image](https://github.com/johnh-otu/OSLab4/assets/94157812/7e47b09b-1487-446a-8d29-5012fb4202f7)
*Figure 1. Three Level Feedback Dispatcher*

The dispatcher needs to maintain two submission queues - Real-Time and User priority - fed
from the job dispatch list. The dispatch list is examined at every dispatcher tick and jobs
that "have arrived" are transferred to the appropriate submission queue. The submission
queues are then examined; any Real-Time jobs are run to completion, pre-empting any other
jobs currently running.

The Real-Time priority job queue must be empty before the lower priority feedback
dispatcher is reactivated. Any User priority jobs in the User job queue that can run within
available resources (memory and i/o devices) are transferred to the appropriate priority
queue. Normal operation of a feedback queue will accept all jobs at the highest priority level
and degrade the priority after each completed time quantum. However, this dispatcher has
the ability to accept jobs at a lower priority, inserting them in the appropriate queue. This
enables the dispatcher to emulate a simple Round Robin dispatcher if all jobs are accepted
at the lowest priority.

![image](https://github.com/johnh-otu/OSLab4/assets/94157812/51d98282-9541-4023-a2a8-d40b03f2ff4b)
*Figure 2. Round Robin Dispatcher*

When all “ready” higher priority jobs have been completed, the feedback dispatcher
resumes by starting or resuming the process at the head of the highest priority non-empty
queue. At the next tick the current job is suspended (or terminated and its resources
released) if there are any other jobs "ready" of an equal or higher priority.
The logic flow should be as shown below (and as discussed in the exercises):

![image](https://github.com/johnh-otu/OSLab4/assets/94157812/245bb765-4a43-490c-9670-532dfcc51200)
_Figure 3. Dispatcher Logic Flow_

### Resource Constraints
The HOST has the following resources:
- 2 Printers
- 1 Scanner
- 1 Modem
- 2 CD Drives
- 1024 Mbyte Memory available for processes
Low priority processes can use any or all of these resources, but the HOST dispatcher is
notified of which resources the process will use when the process is submitted. The
dispatcher ensures that each requested resource is solely available to that process throughout
its lifetime in the "ready-to-run" dispatch queues: from the initial transfer from the job
queue to the Priority 1-3 queues through to process completion, including intervening idle
time quanta.

Real-Time processes will not need any i/o resources (Printer / Scanner / Modem / CD), but
will obviously require memory allocation - this memory requirement will always be 64
Mbytes or less for Real-Time jobs.

### Memory Allocation
- Memory allocation must be as a contiguous block of memory for each process that
remains assigned to the process for the lifetime of that process.
- Enough contiguous spare memory must be left so that the Real-Time processes are
not blocked from execution - 64 Mbytes for a running Real-Time job leaving 960
Mbytes to be shared amongst “active” User jobs.
- The HOST hardware MMU cannot support virtual memory so no swapping of
memory to disk is possible. Neither is it a paged system.
  - Within these constraints, any suitable varible partition memory allocation
scheme (First Fit, Next Fit, Best Fit, Worst Fit, Buddy, etc) may be used.

### Processes
The life-cycle of a process on HOST is:
1. The process is submitted to the dispatcher via the dispatch list, which includes the
process's attributes: arrival time, priority, processor time required (in seconds), memory
block size, and other requested resources.
```
<arrival time>, <priority>, <processor time>, <Mbytes>, <#printers>, <#scanners>, <#modems>, <#CDs>
```
2. A process is "ready-to-run" when it has "arrived" and all required resources are
available.
3. When a process is terminated, its resources are freed for use by other processes.

### Criteria for dispatcher:
1) A real-time process must be run as soon as it arrives.
2) Any real-time process is in dispatch list and submitted for execution should be schedule
based on a First Come First Served.
3) When a process is started to be executed, the dispatcher will display the job parameters
(Process ID, priority, processor time remaining (in seconds), memory location and
block size, and resources requested).
4) If a User process does not completely executed in its priority level, the user process
should be lowered (if possible) and it is re-queued on the appropriate priority queue as
shown in Figures 1 & 3 above.
5) If a User process is suspended by a higher user process, then its priority level should be
lowered (if possible) and it is re-queued on the appropriate priority queue as shown in
Figures 1 & 3 above.
6) If the provided process in dispatch list are at the lowest priority level, the dispatcher
mimic a simple Round Robin dispatcher.
7) If there is no real-time process pending in the dispatch list, the highest priority pending
process is started or restarted.
8) When a process is terminated, its resources are freed for use by other processes.
9) Spare memory must be reserved to ensure that Real-Time processes are not hindered
from executing. Each Real-Time process requires 64 Mbytes to run, leaving 960 Mbytes
available to be used by any ‘active’ User process.
10) If a process requests a resource, dispatcher ensures that the resource is exclusively
available to that process for its entire lifetime.
11) Real-Time processes will not need any i/o resources (Printer / Scanner / Modem / CD)

### Dispatch List
The Dispatch List is a text/csv file, which is the list of processes that the dispatcher will
process. The dispatch list file contains information about the processes, which is to be input
into the dispatcher.
Each line of the list describes one process with the following data as a "comma-space"
delimited list:
```
<arrival time>, <priority>, <processor time>, <Mbytes>, <#printers>, <#scanners>,
<#modems>, <#CDs>
```
Thus,
```
12, 0, 1, 64, 0, 0, 0, 0
12, 1, 2, 128, 1, 0, 0, 1
13, 3, 6, 128, 1, 0, 1, 2
```
would indicate:

1st Job: Arrival at time 12, priority 0 (Real-Time), requiring 1 second of CPU. time and 64
Mbytes memory - no i/o resources required.

2nd Job: Arrival at time 12, priority 1 (high priority User job), requiring 2 seconds of CPU
time, 128 Mbytes of memory, 1 printer and 1 CD drive.

3rd Job: Arrival at time 13, priority 3 (lowest priority User job), requiring 6 seconds of
CPU, 128 Mbytes of memory, 1 printer, 1 modem, and 2 CD drives.

The dispatch list file can be of any length (containing up to 10, 20, 30, ..., 1000 process). It
will be terminated with an end-of-line followed by an end-of-file marker.
Dispatcher input lists to test the operation of the individual features of the dispatcher are
described in the exercises. It should be noted that these lists will almost certainly form the
basis of tests that will be applied to your dispatcher during marking. Operation as described
in the exercises will be expected.
Obviously, your submitted dispatcher will be tested with more complex combinations as
well!

### Project Requirements
1. Please answer the following items in a document named “formal design”:
  - Describe and discuss what memory allocation algorithms you could have
used and justify your final design choice.
  - Describe and discuss the structures used by the dispatcher for queuing,
dispatching and allocating memory and other resources.
  - Describe and justify the overall structure of your program, describing the
various modules and major functions (descriptions of the function 'interfaces'
are expected).
  - Discuss why such a multilevel dispatching scheme would be
used, comparing it with schemes used by "real" operating systems. Outline
shortcomings in such a scheme, suggesting possible improvements. Include
the memory and resource allocation schemes in your discussions.
  - The design document should NOT include any source code.
The formal design document is expected to have in-depth discussions, descriptions
and arguments. The design document is to be submitted separately as a physical
paper document.
2. Implementation of dispatcher by the C language.
  - The dispatcher should prompt the user for the address file of the ‘Dispatcher
list’ then print the sequence of processes as each one is executed, adhering to
the ‘Criteria for dispatcher’. This allows the user to observe how the
processes in the ‘Dispatcher list’ are executed and how their priorities are
influenced.
  - The dispatcher should adhere to the 'Criteria for dispatcher' to ensure the
following operations:
    - Operation of FCFS for real-time process high priority scheduler.
    - Operation of User Feedback scheduler.
    - Operation of User Feedback scheduler in Round Robin mode.
    - Mixed scheduler operation.
    - Resource allocation.
    - Memory management.
    - Combination of all requirements
  - The source code MUST be commented and appropriately structured to allow
your peers to understand and easily maintain the code.
3. The submission should be a zip file consisting of source code file(s), and a
“formal design” document.
  - No executable program should be included. The marker will be automatically
rebuilding your program from the source code provided. If the submitted
code does not compile it can not be marked!
  - Please write the full name of all group members in both source code file(s)
and the formal design document.

### Deliverables
1. The “formal design” document as outlined in ‘Project Requirements - section 1’
above.
2. Source code file(s) of dispatcher as outlined in ‘Project Requirements - section 2’.

### Marking Criteria
The project will be marked out of 100.
‘40’ marks will be allocated for the submitted documentation, which will be judged on
depth of discussion, readability, maintainability, completeness, and demonstration of an
understanding of the problems and your solution:
- Description, discussion and justification of choice of memory allocation algorithms -
10 marks
- Description and discussion of the structures used by the dispatcher - 5 marks
- Description and justification of the program structure and individual modules –
10 marks
- Discussion of dispatching scheme (including memory and resource allocation),
shortcomings, and possible improvements - 15 marks.

The balance of the marks (60) will be based on the operation and functionality of your
dispatcher and how well it performs against different dispatcher lists:
- Operation of FCFS for real-time process high priority scheduler - 10 marks
- Operation of User Feedback scheduler - 10 marks
- Operation of User Feedback scheduler in Round Robin mode - 10 marks
- Mixed scheduler operation - 10 marks
- Resource allocation - 5 marks
- Memory management - 5 marks
- Combination of all requirements - 10 marks
Part marks will be awarded for incomplete projects or programs only providing a subset of
the above requirements. Where only part of the project has been attempted a statement to
that effect should be included in the submission.
