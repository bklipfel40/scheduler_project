/*
*
*
* CS 441/541: CPU Scheduler (Project 1)
*
* Author: Ben Klipfel
*
* Date: 2-14-2018
*/
#include "scheduler.h"
/*
*
* The start of the program, which simulates a cpu scheduler, calls functions
* based on command line arguments
*/
int main(int argc, char **argv) {
  int alg_type = 0;
  int quantum = 0;
  char * file_name = malloc( sizeof(char*) );
  int total = 0;
  struct job ** jobs;
  //counter ( server doesn't let you declare counters inside )
  int i;
  //Parse the command line prompts
  for( i = 0; i < argc; i++ ){
    //condition for if the commandline argument is "-s"
    if( strcmp( argv[i], "-s") == 0 ){
      i++;
      if( argv[i] ){
        alg_type = atoi(argv[i]);
      }
    }
    //condition for if the commandline argument is "-q"
    else if( strcmp( argv[i], "-q") == 0 ){
      i++;
      quantum = atoi(argv[i]);
    }
    //condition for if the commandline argument is a fileName
    else{
      file_name = argv[i];
    }
  }
  //make sure the proper -s value was entered
  if( alg_type > 4 || alg_type < 1 ){
    printf("Error: '-s' must have a value between 1-4\n");
    return 1;
  }
  //print the info received from parsing the commandLine
  print_info( alg_type, quantum, file_name);
  //declare an array of pointers to jobs
  jobs = load( file_name );
  //make sure file exists
  if( jobs == NULL ){
    printf("Error: file does not exist\n");
    return 1;
  }
  process_info(jobs);
  //run the appropriate algorithm
  if( alg_type == 1){
    fcfs(jobs);
  }
  else if( alg_type == 2){
    //create a new array with the jobs sorted in ascending order of cpuburst time
    struct job ** shortest_burst = calloc(total,sizeof(struct job));
    shortest_burst = sort_sjf(jobs);
    print_sort(jobs, shortest_burst);
  }
  else if( alg_type == 3){
    struct job ** highest_priority = calloc(total, sizeof(struct job));
    highest_priority = sort_priority(jobs);
    print_sort(jobs, highest_priority);
  }
  else if( alg_type == 4){
    //make sure the quantum has a positive value greater than 0
    if( quantum <= 0 ){
      printf("Error: quantum must be greater than 0\n");
      return 1;
    }
    rr(jobs, quantum);
  }
  return 0;
}

void print_info( int alg, int q, char * name) {
  char  *names[4] = {"FCFS","SJF","Priority","RR"};
  printf("Scheduler   :  %d %s\n", alg, names[alg-1]);
  printf("Quantum     :  %d\n", q);
  printf("Sch. File   : %s\n", name);
}

struct job ** load( char * name ){
  FILE * file = fopen( name, "r");
  //this will be used to store the value of the int we read from the file
  if(!file){
    return NULL;
  }
  int c = 0;
  //this will let me know how many total jobs are going to be queued up
  fscanf( file, "%d", &total);
  //initiate the array of jobs based off of the first integer we read
  struct job ** jobs = calloc( total, sizeof( struct job ) );
  int i;
  //check for the end of the file
  for( i = 0; i < total; i++ ){
    //need to allocate space for each job or else seg fault
    jobs[i] = calloc(1,sizeof(struct job));
    fscanf(file, "%d", &c);
    jobs[i]->processnum = c;
    fscanf(file, "%d", &c );
    jobs[i]->cpuburst = c;
    fscanf(file, "%d", &c );
    jobs[i]->priority = c;
    jobs[i]->wait = 0;
    jobs[i]->turnaround = 0;
  }
  //close the file
  fclose(file);
  return jobs;
}

void process_info( struct job ** jobs ){
  //at first glance it looks like there is 31 '-'s
  printf("-------------------------------\n");
  int i;
  //print the order of arrival
  printf("Arrival Order: %d", jobs[0]->processnum);
  for(i = 1; i < total; i++ ){
    printf(", %d", jobs[i]->processnum);
  }
  printf("\n");
  //display the information and properties
  printf("Process Information:\n");
  for( i = 0; i < total; i++ ){
    printf("%2d%5d%4d\n", jobs[i]->processnum, jobs[i]->cpuburst, jobs[i]->priority);
  }
  printf("-------------------------------\n");
}

void fcfs( struct job ** jobs ){
  double avg_turnaroundtime = 0;
  double avg_waittime = 0;
  int totalwaittime = 0;
  int totalturnaroundtime = 0;
  int i;
  //iterate through each job in the array
  for( i=0; i < total; i++ ){
    //calculate the total waittime and add to thea avg waittime
    jobs[i]->wait = totalwaittime;
    avg_waittime = avg_waittime + totalwaittime;
    jobs[i]->turnaround = totalturnaroundtime + jobs[i]->cpuburst;
    //increment the total waittime after the fact because we want initial waittime to be 0
    totalwaittime += jobs[i]->cpuburst;
    totalturnaroundtime = jobs[i]->turnaround;
    avg_turnaroundtime = avg_turnaroundtime + totalturnaroundtime;
  }
  print_result( jobs );
  //calculate/print the wait and turnaround averages
  printavgs( avg_waittime,avg_turnaroundtime, total);
}

void rr( struct job ** jobs , int quantum){
  double avg_turnaroundtime = 0;
  double avg_waittime = 0;
  int totalwaittime = 0;
  int totalturnaroundtime = 0;
  int i,k;
  //keep track of how many jobs still have to run
  int remaining_jobs = total;
  //create an array with the value of the required cpu time
  int cputimes[total];
  //create an array of int to keep track of how long we were actually at the job
  int time_at[total];
  for( i = 0; i < total; i++){
    cputimes[i] = jobs[i]->cpuburst;
    time_at[i] = 0;
  }
  //check to see if any jobs are remaining
  while( remaining_jobs > 0 ){
    //go through each job in order
    for( i = 0; i < total; i++ ){
      //if we come across a job with a higher burstime than the quantum
      if( cputimes[i] > quantum ){
        totalturnaroundtime += quantum;
        //update the time at
        time_at[i] += quantum;
        //the total time elapsed
        totalwaittime += quantum;
        jobs[i]->wait = totalwaittime - time_at[i];
        jobs[i]->turnaround = totalturnaroundtime;
        cputimes[i] = cputimes[i] - quantum;
      }
      //the job has less time remaining than the quantum, or the same amount
      else if( cputimes[i] <= quantum && cputimes[i] > 0){
        totalturnaroundtime += cputimes[i];
        time_at[i] += cputimes[i];
        totalwaittime += cputimes[i];
        jobs[i]->wait = totalwaittime - time_at[i];
        jobs[i]->turnaround = totalturnaroundtime;
        cputimes[i] = 0;
        remaining_jobs = remaining_jobs - 1;
      }
    }
  }//while remaining_jobs > 0
  for( k = 0; k < total; k++ ){
    avg_waittime += jobs[k]->wait;
    avg_turnaroundtime += jobs[k]->turnaround;
  }
  print_result( jobs );
  printavgs( avg_waittime, avg_turnaroundtime, total);
}

struct job ** sort_sjf( struct job ** jobs ){
  int i,j;
  //copy the array into a new array
  struct job ** sorted_jobs = malloc( sizeof(struct job) * total);
  for( i = 0; i < total; i++ ){
    sorted_jobs[i] = malloc(sizeof(struct job));
    sorted_jobs[i]->processnum = jobs[i]->processnum;
    sorted_jobs[i]->cpuburst = jobs[i]->cpuburst;
    sorted_jobs[i]->priority = jobs[i]->priority;
    sorted_jobs[i]->wait = 0;
    sorted_jobs[i]->turnaround = 0;
  }
  //declare a job to swap for our sorting algorithm
  struct job * swap = calloc(1,sizeof(struct job));
  //sort the copied array based on cpuburst (selection sort)
  //sort the copied array based on priority (bubble sort)
  for( i = 0; i < (total-1); i++){
    for( j = 0; j < total-i-1; j++ ){
      if( sorted_jobs[j+1]->cpuburst < sorted_jobs[j]->cpuburst){
        equaljobs(swap, sorted_jobs[j]);
        equaljobs(sorted_jobs[j],sorted_jobs[j+1]);
        equaljobs(sorted_jobs[j+1],swap);
      }
    }
  }
  free(swap);
  return sorted_jobs;
}

struct job ** sort_priority( struct job ** jobs ){
  int i,j;
  //copy the array into a new array
  struct job ** sorted_jobs = malloc( sizeof(struct job) * total);
  for( i = 0; i < total; i++ ){
    sorted_jobs[i] = malloc(sizeof(struct job));
    sorted_jobs[i]->processnum = jobs[i]->processnum;
    sorted_jobs[i]->cpuburst = jobs[i]->cpuburst;
    sorted_jobs[i]->priority = jobs[i]->priority;
    sorted_jobs[i]->wait = 0;
    sorted_jobs[i]->turnaround = 0;
  }
  //declare a job to swap for our sorting algorithm
  struct job * swap = calloc(1,sizeof(struct job));
  //sort the copied array based on priority (bubble sort)
  for( i = 0; i < (total-1); i++){
    for( j = 0; j < total-i-1; j++ ){
      if( sorted_jobs[j+1]->priority < sorted_jobs[j]->priority){
        equaljobs(swap, sorted_jobs[j]);
        equaljobs(sorted_jobs[j],sorted_jobs[j+1]);
        equaljobs(sorted_jobs[j+1],swap);
      }
    }
  }
  free(swap);
  return sorted_jobs;
}

void print_sort( struct job ** jobs, struct job ** sorted_jobs){
  double avg_turnaroundtime = 0;
  double avg_waittime = 0;
  int totalwaittime = 0;
  int totalturnaroundtime = 0;
  //counter
  int i;
  //calculate the wait times for the sorted jobs
  for( i=0; i < total; i++ ){
    sorted_jobs[i]->wait = totalwaittime;
    avg_waittime = avg_waittime + totalwaittime;
    sorted_jobs[i]->turnaround = totalturnaroundtime + sorted_jobs[i]->cpuburst;
    totalwaittime += sorted_jobs[i]->cpuburst;
    totalturnaroundtime = sorted_jobs[i]->turnaround;
    avg_turnaroundtime = avg_turnaroundtime + totalturnaroundtime;
  }
  //now we want to load the values if the id number is the same
  mergetimes( jobs, sorted_jobs);
  //print the result
  print_result( jobs );
  //print the final result
  printavgs( avg_waittime,avg_turnaroundtime,total);
}

void printavgs( double waittime, double turnaroundtime, int total){
  waittime = waittime/total;
  turnaroundtime = turnaroundtime/total;
  printf("##################################################\n");
  printf("# Avg. Waiting Time   : %4.2f\n", waittime);
  printf("# Avg. Turnaround Time: %4.2f\n", turnaroundtime);
  printf("##################################################\n");
}

void mergetimes( struct job ** jobs1, struct job ** jobs2 ){
  int i, j;
  for( i = 0; i < total; i++ ){
    for( j = 0; j < total; j++ ){
      if( jobs1[i]->processnum == jobs2[j]->processnum ){
        jobs1[i]->wait = jobs2[j]->wait;
        jobs1[i]->turnaround = jobs2[j]->turnaround;
      }
    }
  }
}

void equaljobs( struct job * job1, struct job * job2){
  job1->processnum = job2->processnum;
  job1->cpuburst = job2->cpuburst;
  job1->priority = job2->priority;
}

void print_result( struct job ** jobs ){
  int i;
  printf("Running...\n");
  //50 hashes?
  printf("##################################################\n");
  printf("#  #  CPU Pri   W   T\n");
  for( i = 0; i < total; i++){
    printf("# %2d %4d %3d %3d %3d\n", jobs[i]->processnum, jobs[i]->cpuburst,
    jobs[i]->priority, jobs[i]->wait, jobs[i]->turnaround);
  }
}
