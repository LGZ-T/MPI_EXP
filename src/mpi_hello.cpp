#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <mpi.h>
#include <iostream>
#include <unistd.h>
#include <iomanip>
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
   for(int i = 0;i < 5;i++)
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
   clean_cache(my_rank, size);
   for(int i = 0;i < parNum;++i)
   {
      if(my_rank == 0)
      {

         int* sending = new int[size];
         int* recving = new int[size];
         data_init(sending, size);


         clean_cache(my_rank, size);

         MPI_Barrier(MPI_COMM_WORLD);
         //sleep(0.1);
         starttime = MPI_Wtime();
         MPI_Send(sending,size,MPI_INT,1,i,MPI_COMM_WORLD);
         endtime = MPI_Wtime();
         cout << fixed << setprecision(15);
         cout <<"t1_Os\t"<< (endtime-starttime) <<"\ts\n";

         delete[] sending;

         clean_cache(my_rank, size);

         double recvtime;
         MPI_Barrier(MPI_COMM_WORLD);
         MPI_Recv(&recvtime,1,MPI_DOUBLE,1,i,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
         cout << "t1_Os+(S-1)G+L+Or\t"<< recvtime<< "\ts\n";

         clean_cache(my_rank, size);

         double endtime_Or;
         MPI_Barrier(MPI_COMM_WORLD);
         starttime = MPI_Wtime();
         MPI_Probe(1,i,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
         endtime = MPI_Wtime();

         MPI_Recv(recving,size,MPI_INT,1,i,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
         endtime_Or = MPI_Wtime();
         cout << "t2_Os+(S-1)*G+L\t" << (endtime - starttime) << "\ts\n";
         cout << "t2_Or?\t"<< (endtime_Or - endtime) << "\ts\n";
         MPI_Recv(&recvtime,1,MPI_DOUBLE,1,i,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
         cout << "t2_Os\t" << recvtime << "\ts\n";


         delete[] recving;

         recving = new int[size];
         data_init(recving, size);

         clean_cache(my_rank, size);

         MPI_Barrier(MPI_COMM_WORLD);
         MPI_Probe(1,i,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
         sleep(0.1);
         starttime = MPI_Wtime();
         MPI_Recv(recving,size,MPI_INT,1,i,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
         endtime = MPI_Wtime();
         cout << "t3_Or\t"<< (endtime - starttime) << "\ts\n\n";
         delete[] recving;
      }
      else
      {
         int* sending = new int[size];
         int* recving = new int[size];
         data_init(sending, size);

         clean_cache(my_rank, size);

         MPI_Barrier(MPI_COMM_WORLD);
         starttime = MPI_Wtime();
         MPI_Recv(recving,size,MPI_INT,0,i,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
         endtime = MPI_Wtime();

         clean_cache(my_rank, size);

         double recvtime = endtime - starttime;
         MPI_Barrier(MPI_COMM_WORLD);
         MPI_Send(&recvtime,1,MPI_DOUBLE,0,i,MPI_COMM_WORLD);
         //cout << "Or+(S-1)G+L+Or\t"<<(endtime -starttime) << "\ts\n";

         delete[] recving;

         clean_cache(my_rank, size);

         MPI_Barrier(MPI_COMM_WORLD);
         starttime = MPI_Wtime(); 
         MPI_Send(sending,size,MPI_INT,0,i,MPI_COMM_WORLD);
         endtime = MPI_Wtime();
         recvtime = endtime - starttime;
         MPI_Send(&recvtime,1,MPI_DOUBLE,0,i,MPI_COMM_WORLD);

         delete[] sending;
         sending  = new int[size];
         data_init(sending, size);

         clean_cache(my_rank, size);

         MPI_Barrier(MPI_COMM_WORLD);
         MPI_Send(sending,size,MPI_INT,0,i,MPI_COMM_WORLD);

         delete[] sending;

      }
   }
   MPI_Finalize();
   return 0;
}
