#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"

#include "../include/buffer.h"


void main (int argc, char *argv[])
{
  atmosphere_stat *stat;
  uint32 h_mem;
  sem_t s_procs_completed; // Semaphore to signal the original process that we're done
  lock_t stat_lock;
  cond_t c_NO2_ready;
  int est_num_no2;
  int temperature;
  int i;


  if (argc != 7) { 
    Printf("Usage: "); Printf(argv[0]); Printf(" %d arguments received, 6 input arguments expected\n", argc-1); 
    Exit();
  }     

  // Convert the command-line strings into integers for use as handles
  h_mem = dstrtol(argv[1], NULL, 10); // The "10" means base 10
  s_procs_completed = dstrtol(argv[2], NULL, 10);
  stat_lock = dstrtol(argv[3], NULL, 10);
  c_NO2_ready = dstrtol(argv[4], NULL, 10);
  est_num_no2 = dstrtol(argv[5], NULL, 10);
  temperature = dstrtol(argv[6], NULL, 10);


  if ((stat = (atmosphere_stat *)shmat(h_mem)) == NULL) {
    Printf("Could not map the virtual address to the memory in "); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
  }


  for (i = 0; i < est_num_no2; i++) {
    if (temperature > 120) break;

    lock_acquire(stat_lock);

    while (stat->num_n2 < 1 || stat->num_o2 < 2) {
      cond_wait(c_NO2_ready);
    }

    stat->num_n2  -= 1;
    stat->num_o2  -= 2;
    stat->num_no2 += 2;

    Printf("[Reaction 3] N2 + 2O2 -> 2NO2\n");
    
    lock_release(stat_lock);
    
  }

  // Signal the semaphore to tell the original process that we're done
  Printf("[Reaction 3] Process with PID %d is complete.\n", Getpid());
  if(sem_signal(s_procs_completed) != SYNC_SUCCESS) {
    Printf("Bad semaphore s_procs_completed (%d) in ", s_procs_completed); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
  }
}