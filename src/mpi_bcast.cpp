#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <mpi.h>
#include <iostream>
#include <unistd.h>
#include <time.h>
#include <iomanip>
using namespace std;

void data_init(int * send_or_recv, int size)
{
   for(int i = 0;i< size;++i)
   {
      send_or_recv[i] = rand(); 
   }
}
int main(int argc ,char* argv[])
{
   if(argc != 3)
   {
      fprintf(stderr, "Usage: mpi_bcast ite_num num_elements");
      exit(1);
   }
   int comm_sz;
   int my_rank;
   MPI_Init(&argc,&argv);
   MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
   MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
   int parNum = atoi(argv[1]);
   int size = atoi(argv[2]);
   srand((unsigned)time(NULL));
   //MPI_Barrier(MPI_COMM_WORLD);

   double total_mpi_bcast_time = 0.0;
   for(int i = 0;i < parNum; ++i)
   {
      int* data = new int[size];
      data_init(data,size);

      MPI_Barrier(MPI_COMM_WORLD);
      total_mpi_bcast_time -= MPI_Wtime();
      MPI_Bcast(data, size,MPI_INT,0,MPI_COMM_WORLD);
      MPI_Barrier(MPI_COMM_WORLD);
      total_mpi_bcast_time += MPI_Wtime();

      delete[] data;
   }
   if(my_rank == 0)
   {
      cout << size * sizeof(int) << "\t" << fixed << setprecision(15) << total_mpi_bcast_time/parNum << "\n";  
   }
   MPI_Finalize();
   return 0;
}
