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
  cond_t c_O2_ready;
  cond_t c_NO2_ready;
  cond_t c_O3_ready;
  int num_o_i;
  int est_num_o2;
  int i;


  if (argc != 8) { 
    Printf("Usage: "); Printf(argv[0]); Printf(" <handle_to_shared_memory_page> <handle_to_page_mapped_semaphore> <handle_to_stat_lock> <handle_to_O2_ready_semaphore> <handle_to_NO2_ready_semaphore> <handle_to_O3_ready_semaphore> <handle_to_input_number_of_N_atoms>\n"); 
    Exit();
  }     

  // Convert the command-line strings into integers for use as handles
  h_mem = dstrtol(argv[1], NULL, 10); // The "10" means base 10
  s_procs_completed = dstrtol(argv[2], NULL, 10);
  stat_lock = dstrtol(argv[3], NULL, 10);
  c_O2_ready = dstrtol(argv[4], NULL, 10);
  c_NO2_ready = dstrtol(argv[5], NULL, 10);
  c_O3_ready = dstrtol(argv[6], NULL, 10);
  num_o_i = dstrtol(argv[7], NULL, 10);


  if ((stat = (atmosphere_stat *)shmat(h_mem)) == NULL) {
    Printf("Could not map the virtual address to the memory in "); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
  }


  est_num_o2 = num_o_i / 2;
  for (i = 0; i < est_num_o2; i++) {
    lock_acquire(stat_lock);

    if (stat->num_o < 2) {
      cond_wait(c_O2_ready);
    }

    stat->num_o  -= 2;
    stat->num_o2 += 1;

    if (stat->num_n2 > 0 && stat->num_o2 > 1) cond_signal(c_NO2_ready);
    if (stat->num_o2 > 2)                     cond_signal(c_O3_ready);

    lock_release(stat_lock);
    Printf("[Reaction 2] O + O -> O2\n");
  }

  // Signal the semaphore to tell the original process that we're done
  Printf("[Reaction 2] Process with PID %d is complete.\n", Getpid());
  if(sem_signal(s_procs_completed) != SYNC_SUCCESS) {
    Printf("Bad semaphore s_procs_completed (%d) in ", s_procs_completed); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
  }
}