/*
 * Example of how to use PAPI counters
 *
 * Tyler Stocksdale
 * 10/12/2017
 */

#include "mpi.h"
#include "papi.h"

int main(int argc, char** argv)
{
  int rank;        //local MPI rank
  int nprocs;      //number of MPI ranks

  // PAPI counters we want to use
  int events[2] = {PAPI_RES_STL, PAPI_STL_ICY}, ret;

  // Counter values will be stored here
  long_long values[2];

  // Check if required number of counters are present or not
  if (PAPI_num_counters() < 2) {
    fprintf(stderr, "No hardware counters here, or PAPI not supported.\n");
    exit(1);
  }

  // Init MPI tasks
  MPI_Init(&argc, &argv);

  // Start PAPI counters
  if ((ret = PAPI_start_counters(events, 2)) != PAPI_OK) {
    fprintf(stderr, "PAPI failed to start counters: %s\n", PAPI_strerror(ret));
    exit(1);
  }

  // *****************  DO STUFF  **************************
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Barrier(MPI_COMM_WORLD);
  // *******************************************************

  // Read counter data
  if ((ret = PAPI_read_counters(values, 2)) != PAPI_OK) {
    fprintf(stderr, "PAPI failed to read counters: %s\n", PAPI_strerror(ret));
    exit(1);
  }

  // Print counter data : "Process rank" "Resurce stall cyles" "No-issue stall cycles"
  printf("%d %lld %lld\n", rank, values[0], values[1]);

  // Stop counters
  if ((ret = PAPI_stop_counters(values, 2)) != PAPI_OK) {
    fprintf(stderr, "PAPI failed to stop counters: %s\n", PAPI_strerror(ret));
    exit(1);
  }

  // Finalize MPI tasks
  MPI_Finalize();

  return 0;
}
