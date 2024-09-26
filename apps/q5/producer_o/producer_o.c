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
  int total_num_o;
  int curr_num_n = 0;
  int curr_num_o = 0;
  int i;

  if (argc != 6) { 
    Printf("Usage: "); Printf(argv[0]); Printf(" <handle_to_shared_memory_page> <handle_to_page_mapped_semaphore> <handle_to_stat_lock> <handle_to_O2_ready_cond_var> <handle_to_input_number_of_O_atoms>\n"); 
    Exit();
  }   

  // Convert the command-line strings into integers for use as handles
  h_mem = dstrtol(argv[1], NULL, 10); // The "10" means base 10
  s_procs_completed = dstrtol(argv[2], NULL, 10);
  stat_lock = dstrtol(argv[3], NULL, 10);
  c_O2_ready = dstrtol(argv[4], NULL, 10);
  total_num_o = dstrtol(argv[4], NULL, 10);


  if ((stat = (atmosphere_stat *)shmat(h_mem)) == NULL) {
    Printf("Could not map the virtual address to the memory in "); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
  }

  for (i = 0; i < total_num_o; i++) {
    lock_acquire(stat_lock);

    stat->num_o += 1;

    curr_num_n = stat->num_n;
    curr_num_o = stat->num_o;

    if (curr_num_o > 1) cond_signal(c_O2_ready);

    lock_release(stat_lock);
    Printf("[Producer O] %d N and %d O\n", curr_num_n, curr_num_o);
  }

  // Signal the semaphore to tell the original process that we're done
  Printf("[Producer O] Process with PID %d is complete.\n", Getpid());
  if(sem_signal(s_procs_completed) != SYNC_SUCCESS) {
    Printf("Bad semaphore s_procs_completed (%d) in ", s_procs_completed); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
  }

}