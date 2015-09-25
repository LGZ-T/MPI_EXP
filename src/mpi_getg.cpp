#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <mpi.h>
#include <iostream>
#include <unistd.h>
#include <time.h>
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

      starttime[0] = MPI_Wtime();
      MPI_Send(sending, size, MPI_INT, 1, 0, MPI_COMM_WORLD);
      endtime[0] = MPI_Wtime();
      starttime[1] = MPI_Wtime();
      MPI_Send(sending, size, MPI_INT, 1, 1, MPI_COMM_WORLD);
      endtime[1] = MPI_Wtime();
      starttime[2] = MPI_Wtime();
      MPI_Send(sending, size, MPI_INT, 1, 2, MPI_COMM_WORLD);
      endtime[2] = MPI_Wtime();
      starttime[3] = MPI_Wtime();
      MPI_Send(sending, size, MPI_INT, 1, 3, MPI_COMM_WORLD);
      endtime[3] = MPI_Wtime();
      starttime[4] = MPI_Wtime();
      MPI_Send(sending, size, MPI_INT, 1, 4, MPI_COMM_WORLD);
      endtime[4] = MPI_Wtime();
      starttime[5] = MPI_Wtime();
      MPI_Send(sending, size, MPI_INT, 1, 5, MPI_COMM_WORLD);
      endtime[5] = MPI_Wtime();
      starttime[6] = MPI_Wtime();
      MPI_Send(sending, size, MPI_INT, 1, 6, MPI_COMM_WORLD);
      endtime[6] = MPI_Wtime();
      starttime[7] = MPI_Wtime();
      MPI_Send(sending, size, MPI_INT, 1, 7, MPI_COMM_WORLD);
      endtime[7] = MPI_Wtime();
      starttime[8] = MPI_Wtime();
      MPI_Send(sending, size, MPI_INT, 1, 8, MPI_COMM_WORLD);
      endtime[8] = MPI_Wtime();
      starttime[9] = MPI_Wtime();
      MPI_Send(sending, size, MPI_INT, 1, 9, MPI_COMM_WORLD);
      endtime[9] = MPI_Wtime();

      for(int i =0; i < parNum; ++i)
      {
         result[i] = endtime[i]-starttime[i];
      }
      for(int i = 0; i < parNum; ++i)
      {
         cout << starttime[i]<< "\t";
      }
      cout << endl;
      for(int i = 0; i < parNum; ++i)
      {
         cout << endtime[i]<< "\t";
      }
      cout << endl;
      for(int i = 0; i < parNum; ++i)
      {
         cout << result[i]<< "\t";
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
