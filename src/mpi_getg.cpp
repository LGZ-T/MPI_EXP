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
void clean_cache(int flag, int size)
{
   for(int i = 0;i < 10;i++)
   {
      int * sending = new int[size];
      int* recving = new int[size];
      data_init(sending, size);

      if(flag == 0)
      {
         MPI_Barrier(MPI_COMM_WORLD);
         MPI_Send(sending,size,MPI_INT,1,i,MPI_COMM_WORLD);

         delete[] sending;

         MPI_Barrier(MPI_COMM_WORLD);
         MPI_Recv(recving,size,MPI_INT,1,i,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

         delete[] recving;
      }
      else if(flag == 1)
      {
         MPI_Barrier(MPI_COMM_WORLD);
         MPI_Recv(recving,size,MPI_INT,0,i,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

         delete[] recving;

         MPI_Barrier(MPI_COMM_WORLD);
         MPI_Send(sending,size,MPI_INT,0,i,MPI_COMM_WORLD);

         delete[] sending;

      }

   }
}
int main(int argc ,char* argv[])
{
   int comm_sz;
   int my_rank;
   MPI_Init(&argc,&argv);
   MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
   MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
   int parNum = atoi(argv[1]);
   int size = atoi(argv[2]);
   srand((unsigned)time(NULL));
   //MPI_Barrier(MPI_COMM_WORLD);


   double starttime[parNum];
   double endtime[parNum];
   double result[parNum];
   if(my_rank == 0)
   {
      cout << (sizeof(int)*size) << endl;
   }
   clean_cache(my_rank, size);
   int* sending = new int[size];
   int* recving = new int[size];
   data_init(sending,size);
   if(my_rank == 0)
   {
      MPI_Barrier(MPI_COMM_WORLD);

      for(int i = 0; i < parNum; ++i )
      {
         starttime[i] = MPI_Wtime();
         MPI_Send(sending, size, MPI_INT, 1, i,MPI_COMM_WORLD);
         endtime[i] = MPI_Wtime();
      }

      for(int i = 0; i < parNum; ++i)
      {
         result[i] = endtime[i] - starttime[i];
         cout <<i << "\t"<<std::fixed <<setprecision(15)<<result[i]<< "\n";
      }
      cout << endl;
   }
   else if(my_rank == 1)
   {
      MPI_Barrier(MPI_COMM_WORLD);
      for(int i = 0;i < parNum; ++i)
      {
         MPI_Recv(recving, size, MPI_INT, 0, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
      }
   }
   MPI_Finalize();
   return 0;
}
