#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"

#include "../include/buffer.h"


void main (int argc, char *argv[])
{
  circular_buffer *cb;
  uint32 h_mem;
  sem_t s_procs_completed; // Semaphore to signal the original process that we're done
  lock_t buf_lock;
  cond_t c_fullslots;
  cond_t c_emptyslots;

  int out_curr_idx = 0;
  char out_char;

  if (argc != 6) { 
    Printf("Usage: "); Printf(argv[0]); Printf(" <handle_to_shared_memory_page> <handle_to_page_mapped_semaphore> <handle_to_buffer_lock> <handle_to_buffer_fullslot_cond_var> <handle_to_buffer_emptyslot_cond_var>\n"); 
    Exit();
  }     

  // Convert the command-line strings into integers for use as handles
  h_mem = dstrtol(argv[1], NULL, 10); // The "10" means base 10
  s_procs_completed = dstrtol(argv[2], NULL, 10);
  buf_lock = dstrtol(argv[3], NULL, 10);
  c_fullslots = dstrtol(argv[4], NULL, 10);
  c_emptyslots = dstrtol(argv[5], NULL, 10);

  if ((cb = (circular_buffer *)shmat(h_mem)) == NULL) {
    Printf("Could not map the virtual address to the memory in "); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
  }

  while (out_curr_idx < ITEM_SIZE) {
    lock_acquire(buf_lock);


    if (cb->head == cb->tail) { // empty
      Printf("Consumer %d waits for items\n", Getpid());;
      cond_wait(c_fullslots);
    }

    out_char = cb->char_buf[cb->tail++];
    cond_signal(c_emptyslots);
    lock_release(buf_lock);
    
    Printf("Consumer %d removed: %c\n", Getpid(), out_char);
    out_curr_idx++;
  }

  // Signal the semaphore to tell the original process that we're done
  Printf("Consumer %d is complete.\n", Getpid());
  if(sem_signal(s_procs_completed) != SYNC_SUCCESS) {
    Printf("Bad semaphore s_procs_completed (%d) in ", s_procs_completed); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
  }
}