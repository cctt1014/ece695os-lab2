#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"

#include "../include/buffer.h"

void main (int argc, char *argv[])
{
  int num_n_i = 0;               // Used to store number of N atoms
  int num_o_i = 0;
  int est_num_n2 = 0;
  int est_num_o2 = 0;
  int est_num_no2 = 0;
  int est_num_o3 = 0;
  int temperature_i = 0;
  int i;                          // Loop index variable
  atmosphere_stat *stat;           // Used to get address of shared memory page
  uint32 h_mem;                   // Used to hold handle to shared memory page
  sem_t s_procs_completed;        // Semaphore used to wait until all spawned processes have completed
  lock_t stat_lock;
  cond_t c_N2_ready;
  cond_t c_O2_ready;
  cond_t c_NO2_ready;
  cond_t c_O3_ready;
  char h_mem_str[10];             // Used as command-line argument to pass mem_handle to new processes
  char s_procs_completed_str[10]; // Used as command-line argument to pass page_mapped handle to new processes
  char stat_lock_str[10];
  char c_N2_ready_str[10];
  char c_O2_ready_str[10];
  char c_NO2_ready_str[10];
  char c_O3_ready_str[10];
  char num_n_i_str[10];
  char num_o_i_str[10];
  char est_num_no2_str[10];
  char est_num_o3_str[10];
  char temperature_i_str[10];


  if (argc != 4) {
    Printf("Usage: "); Printf(argv[0]); Printf(" <number of N atoms> <number of O atoms> <temperature>\n");
    Exit();
  }

  // Convert string from ascii command line argument to integer number
  num_n_i = dstrtol(argv[1], NULL, 10); // the "10" means base 10
  num_o_i = dstrtol(argv[2], NULL, 10); // the "10" means base 10
  temperature_i = dstrtol(argv[3], NULL, 10); // the "10" means base 10
  Printf("[MAKEPROCS] Simulate with %d N atoms and %d O atoms at %d degree\n", num_n_i, num_o_i, temperature_i);  

  // Allocate space for a shared memory page, which is exactly 64KB
  // Note that it doesn't matter how much memory we actually need: we 
  // always get 64KB
  if ((h_mem = shmget()) == 0) {
    Printf("ERROR: could not allocate shared memory page in "); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
  }

  // Map shared memory page into this process's memory space
  if ((stat = (atmosphere_stat *)shmat(h_mem)) == NULL) {
    Printf("Could not map the shared page to virtual address in "); Printf(argv[0]); Printf(", exiting..\n");
    Exit();
  }

  // Initialize stat counting number
  stat->num_n = 0;
  stat->num_o = 0;
  stat->num_n2 = 0;
  stat->num_o2 = 0;
  stat->num_no2 = 0;
  stat->num_o3 = 0;


  // Create semaphore to not exit this process until all other processes 
  // have signalled that they are complete.  To do this, we will initialize
  // the semaphore to (-1) * (number of signals), where "number of signals"
  // should be equal to the number of processes we're spawning - 1.  Once 
  // each of the processes has signaled, the semaphore should be back to
  // zero and the final sem_wait below will return.
  if ((s_procs_completed = sem_create(-(6-1))) == SYNC_FAIL) {
    Printf("Bad sem_create in "); Printf(argv[0]); Printf("\n");
    Exit();
  }

  // Create lock and semaphores for stat
  if ((stat_lock = lock_create()) == SYNC_FAIL) {
    Printf("Bad lock_create in "); Printf(argv[0]); Printf("\n");
    Exit();
  }
  if ((c_N2_ready = cond_create(stat_lock)) == SYNC_FAIL) {
    Printf("Bad cond_create in "); Printf(argv[0]); Printf("\n");
    Exit();
  }
  if ((c_O2_ready = cond_create(stat_lock)) == SYNC_FAIL) {
    Printf("Bad cond_create in "); Printf(argv[0]); Printf("\n");
    Exit();
  }
  if ((c_NO2_ready = cond_create(stat_lock)) == SYNC_FAIL) {
    Printf("Bad cond_create in "); Printf(argv[0]); Printf("\n");
    Exit();
  }
  if ((c_O3_ready = cond_create(stat_lock)) == SYNC_FAIL) {
    Printf("Bad cond_create in "); Printf(argv[0]); Printf("\n");
    Exit();
  }  


  // Determine pre-defined number of NO2 and O3
  est_num_n2 = num_n_i/2;
  est_num_o2 = num_o_i/2;

  while (((est_num_n2>0) && (est_num_o2>1) && (temperature_i<=120)) || ((est_num_o2 > 2) && (temperature_i>=60))) {
    if ((est_num_n2>0) && (est_num_o2>1) && (temperature_i<=120)) { // Reaction 3 -> NO2
      est_num_n2--;
      est_num_o2 -= 2;
      est_num_no2++;
    }
    if ((est_num_o2 > 2) && (temperature_i>=60)) { // Reaction 4 -> O3
      est_num_o2 -= 3;
      est_num_o3++;
    }
  }


  // Convert process arguments to strings
  ditoa(h_mem, h_mem_str);
  ditoa(s_procs_completed, s_procs_completed_str);
  ditoa(stat_lock, stat_lock_str);
  ditoa(c_N2_ready,  c_N2_ready_str);
  ditoa(c_O2_ready,  c_O2_ready_str);
  ditoa(c_NO2_ready,  c_NO2_ready_str);
  ditoa(c_O3_ready, c_O3_ready_str);
  ditoa(num_n_i, num_n_i_str);
  ditoa(num_o_i, num_o_i_str);
  ditoa(est_num_no2, est_num_no2_str);
  ditoa(est_num_o3, est_num_o3_str);
  ditoa(temperature_i, temperature_i_str);


  // Create 6 processes simulating the reactions
  process_create(PROD_N_OBJ_TO_RUN, h_mem_str, s_procs_completed_str, stat_lock_str, c_N2_ready_str, num_n_i_str, NULL);
  process_create(PROD_O_OBJ_TO_RUN, h_mem_str, s_procs_completed_str, stat_lock_str, c_O2_ready_str, num_o_i_str, NULL);

  process_create(REAC_1_OBJ_TO_RUN, h_mem_str, s_procs_completed_str, stat_lock_str, c_N2_ready_str, c_NO2_ready_str, num_n_i_str, NULL);
  process_create(REAC_2_OBJ_TO_RUN, h_mem_str, s_procs_completed_str, stat_lock_str, c_O2_ready_str, c_NO2_ready_str, c_O3_ready_str, num_o_i_str, NULL);
  process_create(REAC_3_OBJ_TO_RUN, h_mem_str, s_procs_completed_str, stat_lock_str, c_NO2_ready_str, est_num_no2_str, temperature_i_str, NULL);
  process_create(REAC_4_OBJ_TO_RUN, h_mem_str, s_procs_completed_str, stat_lock_str, c_O3_ready_str, est_num_o3_str, temperature_i_str, NULL);


  // And finally, wait until all spawned processes have finished.
  if (sem_wait(s_procs_completed) != SYNC_SUCCESS) {
    Printf("Bad semaphore s_procs_completed (%d) in ", s_procs_completed); Printf(argv[0]); Printf("\n");
    Exit();
  }
  Printf("[MAKEPROCS] All other processes completed, exiting main process.\n");

  Exit();


}
