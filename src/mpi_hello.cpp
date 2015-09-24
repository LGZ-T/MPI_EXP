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
	send_or_recv = new int[size];
	for(int i = 0;i< size;++i)
	{
		send_or_recv[i] = rand(); 
	}

	return;
}
inline void mpi_test(int* send_or_recv, int size, int i)
{
	
}
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
   srand((unsigned)time(NULL));
   //MPI_Barrier(MPI_COMM_WORLD);


   if(my_rank == 0)
   {
      cout << (sizeof(int)*size) << endl;
   }
   for(int i = 0;i < parNum;++i)
   {
      if(my_rank == 0)
      {

         int* sending;
         int* recving = new int[size];
	 data_init(sending, size);


         MPI_Barrier(MPI_COMM_WORLD);
         //sleep(0.1);
         starttime = MPI_Wtime();
         MPI_Send(sending,size,MPI_INT,1,i,MPI_COMM_WORLD);
         endtime = MPI_Wtime();
         cout <<"Os\t"<< (endtime-starttime) <<"\ts"<< endl;

         delete[] sending;

         MPI_Barrier(MPI_COMM_WORLD);
	 starttime = MPI_Wtime();
	 MPI_Probe(1,i,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	 endtime = MPI_Wtime();
	 cout << "Os+(S-1)*G+L\t" << (endtime - starttime) << "\ts"<<endl;

	 starttime = MPI_Wtime();
         MPI_Recv(recving,size,MPI_INT,1,i,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	 endtime = MPI_Wtime();
	 cout << "Or\t"<< (endtime - starttime) << "\ts" <<endl;

         delete[] recving;
      }
      else
      {
         int* sending;
         int* recving = new int[size];
	 data_init(sending, size);

         MPI_Barrier(MPI_COMM_WORLD);
         starttime = MPI_Wtime();
         MPI_Recv(recving,size,MPI_INT,0,i,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
         endtime = MPI_Wtime();
	 cout << "Or+(S-1)G+L+Or\t"<<(endtime -starttime) << "\ts" <<endl;

         delete[] recving;

         MPI_Barrier(MPI_COMM_WORLD);
	 starttime = MPI_Wtime(); 
         MPI_Send(sending,size,MPI_INT,0,i,MPI_COMM_WORLD);
	 endtime = MPI_Wtime();

         delete[] sending;
      }
   }
   MPI_Finalize();
   return 0;
}
