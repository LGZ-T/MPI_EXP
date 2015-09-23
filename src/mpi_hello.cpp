#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <mpi.h>
#include <iostream>
#include <unistd.h>
using namespace std;

int main(int argc ,char* argv[])
{
   int comm_sz;
   int my_rank;
   MPI_Init(&argc,&argv);
   MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
   MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
   double starttime, endtime;
   starttime = MPI_Wtime();
   endtime = MPI_Wtime();
   double wTime = endtime - starttime;
   int parNum = atoi(argv[1]);
   int size = atoi(argv[2]);
   //MPI_Barrier(MPI_COMM_WORLD);


   if(my_rank == 0)
   {
      cout << (sizeof(int)*size) << endl;
   }
   for(int i = 0;i < parNum;++i)
   {
      if(my_rank == 0)
      {
         int* sending = new int[size];
         int* recving = new int[size];
         for(int j = 0; j < size;++j)
         {
            sending[j] = i*j+j+my_rank;
            recving[j] = i*j+j+my_rank;
         }
         double starttime,endtime;
         MPI_Barrier(MPI_COMM_WORLD);
         //sleep(0.1);
         starttime = MPI_Wtime();
         MPI_Send(sending,size,MPI_INT,1,i,MPI_COMM_WORLD);
         endtime = MPI_Wtime();
         cout <<i<<"\t"<< (endtime-starttime - wTime) << endl;
         delete[] sending;
         MPI_Barrier(MPI_COMM_WORLD);
         MPI_Recv(recving,size,MPI_INT,1,i,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
         delete[] recving;
      }
      else
      {
         int* sending = new int[size];
         int* recving = new int[size];
         MPI_Barrier(MPI_COMM_WORLD);
         starttime = MPI_Wtime();
         MPI_Recv(recving,size,MPI_INT,0,i,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
         endtime = MPI_Wtime();
         for(int j = 0; j < size;++j)
         {
            sending[j] = i*j+j+my_rank;
            recving[j] = i*j+j+my_rank;
         }
         delete[] recving;
         MPI_Barrier(MPI_COMM_WORLD);
         MPI_Send(sending,size,MPI_INT,0,i,MPI_COMM_WORLD);
         delete[] sending;
      }
   }
   MPI_Finalize();
   return 0;
}
