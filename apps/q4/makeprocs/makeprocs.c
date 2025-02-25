#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"

#include "../include/buffer.h"

void main (int argc, char *argv[])
{
  int numprocs = 0;               // Used to store number of processes to create
  int i;                          // Loop index variable
  circular_buffer *buf;           // Used to get address of shared memory page
  uint32 h_mem;                   // Used to hold handle to shared memory page
  sem_t s_procs_completed;        // Semaphore used to wait until all spawned processes have completed
  lock_t buf_lock;
  cond_t c_fullslots;
  cond_t c_emptyslots;
  char h_mem_str[10];             // Used as command-line argument to pass mem_handle to new processes
  char s_procs_completed_str[10]; // Used as command-line argument to pass page_mapped handle to new processes
  char buf_lock_str[10];
  char c_fullslots_str[10];
  char c_emptyslots_str[10];

  if (argc != 2) {
    Printf("Usage: "); Printf(argv[0]); Printf(" <number of processes to create>\n");
    Exit();
  }

  // Convert string from ascii command line argument to integer number
  numprocs = dstrtol(argv[1], NULL, 10); // the "10" means base 10
  Printf("Creating %d pairs of producer and consumer\n", numprocs);  

  // Allocate space for a shared memory page, which is exactly 64KB
  // Note that it doesn't matter how much memory we actually need: we 
  // always get 64KB
  if ((h_mem = shmget()) == 0) {
    Printf("ERROR: could not allocate shared memory page in "); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
  }

  // Map shared memory page into this process's memory space
  if ((buf = (circular_buffer *)shmat(h_mem)) == NULL) {
    Printf("Could not map the shared page to virtual address in "); Printf(argv[0]); Printf(", exiting..\n");
    Exit();
  }

  // Initialize circular buffer pointers
  buf->head = 0;
  buf->tail = 0;

  // Create semaphore to not exit this process until all other processes 
  // have signalled that they are complete.  To do this, we will initialize
  // the semaphore to (-1) * (number of signals), where "number of signals"
  // should be equal to the number of processes we're spawning - 1.  Once 
  // each of the processes has signaled, the semaphore should be back to
  // zero and the final sem_wait below will return.
  if ((s_procs_completed = sem_create(-(numprocs*2-1))) == SYNC_FAIL) {
    Printf("Bad sem_create in "); Printf(argv[0]); Printf("\n");
    Exit();
  }

  // Create lock and semaphores for buffer
  if ((buf_lock = lock_create()) == SYNC_FAIL) {
    Printf("Bad lock_create in "); Printf(argv[0]); Printf("\n");
    Exit();
  }
  if ((c_fullslots = cond_create(buf_lock)) == SYNC_FAIL) {
    Printf("Bad cond_create in "); Printf(argv[0]); Printf("\n");
    Exit();
  }
  if ((c_emptyslots = cond_create(buf_lock)) == SYNC_FAIL) {
    Printf("Bad cond_create in "); Printf(argv[0]); Printf("\n");
    Exit();
  }  


  // Convert process arguments to strings
  ditoa(h_mem, h_mem_str);
  ditoa(s_procs_completed, s_procs_completed_str);
  ditoa(buf_lock, buf_lock_str);
  ditoa(c_fullslots,  c_fullslots_str);
  ditoa(c_emptyslots, c_emptyslots_str);


  // Create producers and consumers
  for(i=0; i<numprocs; i++) {
    process_create(PROD_OBJ_TO_RUN, h_mem_str, s_procs_completed_str, buf_lock_str, c_fullslots_str, c_emptyslots_str, NULL);
    Printf("Producer %d created\n", i);
    process_create(CONS_OBJ_TO_RUN, h_mem_str, s_procs_completed_str, buf_lock_str, c_fullslots_str, c_emptyslots_str, NULL);
    Printf("Consumer %d created\n", i);
  }

  // And finally, wait until all spawned processes have finished.
  if (sem_wait(s_procs_completed) != SYNC_SUCCESS) {
    Printf("Bad semaphore s_procs_completed (%d) in ", s_procs_completed); Printf(argv[0]); Printf("\n");
    Exit();
  }
  Printf("All other processes completed, exiting main process.\n");

  Exit();


}