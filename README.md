# CS441/541 Project 2

## Author(s):

Ben Klipfel


## Date:

2-11-2018


## Description:

This program simulates a CPU scheduling simulator. It will read from a file information for jobs and then run
them based on command line prompts. This program will also calculate the wait and turnaround times of each job depending on the type of algorithm it runs.


## How to build the software

Typing 'make' into the command prompt will compile the software. This software is programmed in the c language.


## How to use the software

Running ./scheduler followed by 3 command line parameters:
  -s : the type of scheduler to simulate
       1 : FCFS (first come first serve)
       2 : SFJ (shortest job first )
       3 : Priority
       4 : RR (round-robin)

  -q : the value of the quantum, this is completely optional if you are not running
       a round-robin scheduler, and will be defaulted to 0 if not specified

  any other input : will be parsed as the name of the file containing the data for the number of jobs, and the properties
                    of each job

Example: The following will simulate a round-robin scheduler with a quantum of 2 reading in from the file text.txt

          ./scheduler -s 4 -q 2 text.txt


## How the software was tested

Since running 'make check' without a mostly complete program would result in absolute anarchy and thousands of lines of errors,
I mostly tested my software by running one individual test from the given-examples file, starting at the lowest level and then
working my way up, until every output seemed (at least at first glance, I didn't actually compare all 8000 lines) to work.

After each level of each type of scheduler worked, I ran 'make check' which tested 42 cases, and could let me know if the
formatting was correct.

To check certain command line requirements ( such as making sure -s had a value 1-4 ) I had to manually type an invalid
command prompt myself. Since there is no required error message and it is not tested in any given test or make, it can be
easy to miss something.

## Test Suite

I created 5 test files, located in the /tests directory of this project. Each test was used to double check something I wasn't
too sure of and needed more than one small scale file to check. The main goal of these was to double check  a specific value
(i.e. making sure the processnums were sorted correctly in sfj). The files are the following:

sjffirst.txt : This file had 5 jobs, and 3 of them have the same priority. I wanted the output to be easy to interpret so I made
               the processnums 1-5. This input file double checks that if multiple jobs have the same cpuburst time. The
               "tiebreaker" goes to the job that was seen first.

easyRR.txt  : This file may seem kind of ridiculous, since it contains 5 jobs all with a cpuburst of 5. However, that makes it
              easy for someone to trace their round-robin algorithm to make sure that everything looks ok (at least in the early
              stages).

fcfsMed.txt : This test file was used when I thought I had a good function for FCFS but I wasn't ready to test it in a large file
              with 50 or more, so I used this file with only 20.

priorTst.txt : This test file is a simple file to test the priority algorithm, it has only 5 jobs with processnums 1-5 and
               priorities that match the processnum so it is easy to read

easycalc.txt : This is a random test file that was easy for me to figure out on paper for each algorithm, another example of
               creating something small scale in addition to the given tests

## Known bugs and problem areas

I encountered tons of little bugs throughout the course of this program (syntax, etc.), but the following are the most notable:

Problem: Segmentation fault: 11 when trying to load the values read in from the file into an array of jobs
Solution: instead of allocating space for a pointer to struct, I just allocated space for a struct

Problem: Infinite loop in my rr() function that breaks my entire program
Solution: the value of total was not being preserved when called, and therefore not all of the jobs were being visited, so
          I passed total as a parameter instead

Problem: Segmentation fault if I had any sort of typo in the fileName
Solution: While this is suppose to cease the program anyway, it is cleaner for me to send an error if the file does not exist
          instead of letting the program seg fault

Problem: When sorting by priority or job-length, jobs with the same value are not always prioritized by the first one that
         entered the scheduler (on the larger test cases only)
Solution: I switched to bubble sort from selection sort

Problem: Round robin algorithm has incorrect time variables for each job
Solution: Created an array to remember the current time left for each job
