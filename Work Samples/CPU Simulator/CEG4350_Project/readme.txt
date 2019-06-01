CEG4350_Project
----------------------------------------------------------------------------
This project is a CPU scheduling simulator designed to simulate CPU 
scheduling of processes using either first come first serve (FCFS), round 
robin (RR), or multi-level feedback queue (MLFQ). The "processes" will be 
read in from a text file which simply indicates a process id (pid), arrival
time, and CPU burst time. The FCFS scheme will schedule them to run in FCFS,
with each process running to completion before yielding. The RR scheme will 
give each arrived process an 8 ms CPU timeslice (quantum) before preempting
that process and placing it at the rear of the queue. The MLFQ scheme will 
have newly arrived processes enter Queue 0, where they will have an 8 ms 
quantum in RR. If they do not complete, they will be demoted to Queue 1, 
where they will receive a 16 ms quantum in RR. If the process still does not
complete, it will be demoted to Queue 2, where it will be served FCFS. Any
time a process enters a higher priority queue, the lower priority process 
will be preempted and placed at the rear of its CURRENT queue (where it will
be given a fresh quantum if in RR).

Getting Started
----------------------------------------------------------------------------
This project was developed with Visual Studio. It will be necessary to have
Visual Studio on your machine.

Prerequisites

Microsoft Visual Studio. Download from https://visualstudio.microsoft.com/
For a step by step guide on how to install Visual studio, visit
https://docs.microsoft.com/en-us/visualstudio/install/install-visual-studio

Installing

After unzipping the project folder CEG4350_Project, it is only necessary to
open the project folder and select CEG4350_Project.sln. This will open the
Visual Studio IDE and all of the source code for the project in one place.
The program can be compiled by opening the Build menu and selecting Build
Solution.

Running
----------------------------------------------------------------------------
The program can be run from Visual Studio by opening the Debug menu and 
selecting Start Without Debugging. The user will then receive prompts to
input the name of the input file and the scheduling scheme. Alternatively, 
the program can be run from a command prompt. If using the command prompt,
make sure you are in the CEG4350_Project\Debug directory. Then enter
CEG4350_Project at the prompt to start the program with the input prompts
for the input file name and scheduling scheme. It is also possible to enter
the name of the input file and the scheduling scheme as command line
arguments. The format to run the program like this is as follows:
CEG4350_Project input_file [FCFS | RR | MLFQ]. If the format is not
followed, the normal input prompts will instead be used. Keep in mind that
it is necessary to place a copy of your input file in either the 
CEG4350_Project\CEG4350_Project directory (when the program is run from 
Visual Studio) or in the CEG4350_Project\Debug directory (when the input
file name is entered as a command line argument). A copy of a sample input
file is provided in each of these directories for testing.

Tests
----------------------------------------------------------------------------
A sample input file, which was used for testing, is provided. The name of
this input file is test_input.txt. Running the program with this input file
(and a selection of FCFS, RR, or MLFQ) will verify correct operation of the
program. The program will ignore any non-numeric text at the beggining of
the file, but after that it is assumed the file is formatted correctly.

Example:
PID Arrival Burst
1   0       10
2   1       12
(Only numeric data  of this format is permitted until the end of file is 
reached. Any amount of whitespace may be used. Only integers may be used, 
and only non-negative integers for arrival time and positive integers for
burst time will result in meaningful program execution. The processes need
not be ordered by arrival time, however. The program will do this).

The sample input file is designed to show the given processes will be 
scheduled correctly in any of the given scheduling schemes. This is trivial
for FCFS, and it is easy to verify for RR. For MLFQ, care was taken to
include the following test cases
1. A process enters Queue 0 and ends there.
2. A process enters Queue 0 and its quantum expires before ending.
3. A process is demoted to Queue 1 and ends there.
4. A process is demoted to Queue 1, and its quantum expires before ending.
5. A process is demoted to Queue 1, and it is preempted by a higher queue.
6. A process is demoted to Queue 2 and ends there.
7. A process is demoted to Queue 2, and it is preempted by a higher queue.
8. A process enters Queue 0 after all other processes had already ended.

With the test_input file, it can be observed that all of these test cases 
work correctly. Furthermore, it can be shown that the averages for the 
response time, waiting time, and turnaround times all compute the same as
with hand calculations. Finally, by scrambling the order of the processes,
it can be shown that the program will reorder the processes by arrival time.

Contributing
----------------------------------------------------------------------------
The ReadyQueue.h class in this project is a simple linked-queue 
implementation with methods for enqueue and dequeue. This implementation
was adapted from the textbook for CS 3100: Data Structures & Algorithm
Analysis in C++ by Clifford A. Shaffer. The remaining source code in the
project is original to myself.

Author
----------------------------------------------------------------------------
Daniel Nedrow
See Contributing section above.