/*
 *
 *
 * CS 441/541: CPU Scheduler (Project 1)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


/******************************
 * Defines
 ******************************/
//none

/******************************
 * Structures
 ******************************/

 /*
  * Desc: A struct representing a single job that the cpu will run
  * Vars: processnum: the processing number of the job
  *       cpuburst: the amount of time the job requires to execute
  *       priority: the priority of the job
  *       wait: the amount of time the job had to wait (initially 0)
  *       turnaround: the total turnaround of the individual job (initially 0)
  */
struct job{
  int processnum;
  int cpuburst;
  int priority;
  int wait;
  int turnaround;
};

/******************************
 * Global Variables
 ******************************/
//the type of algorithm parsed from the command line
int alg_type;
//the value of the quantum (default is 0)
int quantum;
//the name of the file parsed from the commandline
char * file_name;
//the total amount of jobs that will be run
int total;
//an array of jobs that will represent the jobs the cpu must run
struct job ** jobs;

/******************************
 * Function declarations
 ******************************/
/*
 * Desc: Prints to the console the information gathered from the command line
 *       prompts
 *
 * Parameters: alg : the type of scheduling algorithm parsed
 *             q   : the value of the quantum that was parsed
 *             name: the name of the file that was parsed
 *
 * Returns: nothing, prints to the console and exits
 *
 */
void print_info( int alg, int q, char * name);

/*
 * Desc: loads the values read from a file into a struct
 *
 * Parameters: name: the name of the file the values will be read from
 *
 * Returns: an array of structs representing the jobs that will be scheduled
 *
 */
struct job ** load( char * name );

/*
 * Desc: Prints to the console the values of each job
 *
 * Parameters: jobs: the array of jobs that will be iterated through
 *
 * Returns: none - prints to console and exits
 *
 */
void process_info( struct job ** jobs );

/*
 * Desc: Runs the first come first serve algorithm and calculates the wait and
 *       turnaround time for each job
 *
 * Parameters: jobs: the array of jobs that will be iterated through
 *
 * Returns: none - prints to console and exits
 *
 */
void fcfs( struct job ** jobs );

/*
 * Desc: Runs the round-robin algorithm and calculates the wait and
 *       turnaround time for each job
 *
 * Parameters: jobs: the array of jobs that will be iterated through
 *
 * Returns: none - prints to console and exits
 *
 */
void rr( struct job ** jobs, int quantum );

/*
 * Desc: Creates a new array of jobs with the sorted values of a given
 *       array of jobs based on the length of the job
 *
 * Parameters: jobs: the array of jobs that will be sorted
 *
 * Returns: struct job ** - the sorted array of jobs
 *
 */
struct job ** sort_sjf( struct job ** jobs );

/*
 * Desc: Creates a new array of jobs with the sorted values of a given
 *       array of jobs based on the priority
 *
 * Parameters: jobs: the array of jobs that will be sorted
 *
 * Returns: struct job ** - the sorted array of jobs
 *
 */
struct job ** sort_priority( struct job ** jobs);

/*
 * Desc: Prints the jobs in order but keeps the values as though they were
 *       sorted
 *
 * Parameters: jobs: the array of jobs in original order
 *             sorted_jobs: an array of jobs sorted by cpuburst, or priority
 *
 * Returns: none - prints to console and exits
 */
void print_sort( struct job ** jobs, struct job ** sorted_jobs);

/*
 * Desc: Prints the average wait time and turnaround time to the console
 *
 *
 * Parameters: waittime: the total wait time
 *             turnaroundtime: the total turnaround time
 *             total : the total amount of jobs that were run
 *
 * Returns: none - prints to console and exits
 */
void printavgs( double waittime, double turnaroundtime, int total);

/*
 * Desc: sets the wait time and turnaround time values of each job in one struct
 *       to the wait time and turnaround time values in another struct,
 *       this method works with printing jobs in order with their sorted times
 *
 * Parameters: jobs1: the array of jobs that will be taking on the value
 *             jobs2: the array of jobs that jobs1 will get the values from
 *
 * Returns: none - this is all done dynamically
 */
void mergetimes( struct job ** jobs1, struct job ** jobs2 );

/*
 * Desc: sets one job equal to another job
 *
 * Parameters: job1: the job that will take on the value of job2
 *             job2: the values job1 will be taking on
 *
 * Returns: none - all of this is done dynamically
 */
void equaljobs( struct job * job1, struct job * job2 );


/*
 * Desc: Prints the results of the scheduler after printing "Running..."
 *
 * Parameters: jobs: the array of jobs thats results will be printed
 *
 * Returns: none - prints to console and exits
 */
void print_result( struct job ** jobs );
