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
  sem_t s_fullslots;
  sem_t s_emptyslots;

  char out_content[ITEM_SIZE] = BUF_ITEM;
  int out_curr_idx = 0;

  if (argc != 6) { 
    Printf("Usage: "); Printf(argv[0]); Printf(" <handle_to_shared_memory_page> <handle_to_page_mapped_semaphore> <handle_to_buffer_lock> <handle_to_buffer_fullslot_semaphore> <handle_to_buffer_emptyslot_semaphore>\n"); 
    Exit();
  }   

  // Convert the command-line strings into integers for use as handles
  h_mem = dstrtol(argv[1], NULL, 10); // The "10" means base 10
  s_procs_completed = dstrtol(argv[2], NULL, 10);
  buf_lock = dstrtol(argv[3], NULL, 10);
  s_fullslots = dstrtol(argv[4], NULL, 10);
  s_emptyslots = dstrtol(argv[5], NULL, 10);

  if ((cb = (circular_buffer *)shmat(h_mem)) == NULL) {
    Printf("Could not map the virtual address to the memory in "); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
  }

  while (out_curr_idx < ITEM_SIZE) {
    sem_wait(s_emptyslots);

    lock_acquire(buf_lock);
    cb->char_buf[cb->head++] = out_content[out_curr_idx];
    Printf("Producer %d inserted: %c\n", Getpid(), out_content[out_curr_idx++]);
    lock_release(buf_lock);

    sem_signal(s_fullslots);
  }

  // Signal the semaphore to tell the original process that we're done
  Printf("Producer %d is complete.\n", Getpid());
  if(sem_signal(s_procs_completed) != SYNC_SUCCESS) {
    Printf("Bad semaphore s_procs_completed (%d) in ", s_procs_completed); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
  }

}