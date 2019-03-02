//
// Created by alican on 2/26/19.
//
#include <stdio.h>
#include <mpi.h>

void main(int argc,char **argv){
    int size,rank;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    int globalsum;
    globalsum = 0;

    if(rank==0){//size

        int localsum,i;
        localsum = 0;
        int sz;
        sz = 0;
        FILE *fp;
        fp  = fopen(argv[argc-1],"r");
        while(!feof(fp)){


            fscanf(fp,"%d",&i);
            sz++;

        }
        sz--;
        rewind(fp);
        int arrtobcast[sz];
        int forCounter,forCounter2;
        for(forCounter=0;forCounter<sz;forCounter++){
            fscanf(fp,"%d",&arrtobcast[forCounter]);
        }
        //send array size
        MPI_Bcast((void *)&sz,1,MPI_INT, 0, MPI_COMM_WORLD);
        //send array
        MPI_Bcast((void *)&arrtobcast,sz,MPI_INT,0,MPI_COMM_WORLD);

        if(sz%size == 0){
            int x;
            x = sz/size;
            for(forCounter = 0; forCounter<x; forCounter++){
                localsum += arrtobcast[forCounter];
            }


        }
        else{
            int x,y;
            y = sz%size;
            x = (sz - y) / size;

            if(x == 0){//more cores than input numbers

                localsum += arrtobcast[0];



            }
            else{
                x++;
                for(forCounter = 0; forCounter<x; forCounter++){
                    localsum += arrtobcast[forCounter];
                }


            }
        }

        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Allreduce(&localsum,&globalsum,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
        //here

    }
    else{//not root
        int localsum;
        localsum = 0;
        int sz;
        MPI_Bcast((void *)&sz,1,MPI_INT, 0, MPI_COMM_WORLD);
        int arrtobcast[sz];
        MPI_Bcast((void *)&arrtobcast,sz,MPI_INT,0,MPI_COMM_WORLD);

        if(rank>=sz){//there are more processors than input numbers and this one should not do anything

        }
        else{
            int forCounter;

            if(sz%size == 0){

                int x;
                x = sz/size;
                for(forCounter = (x*rank); forCounter<(x*(rank+1)); forCounter++){
                    localsum += arrtobcast[forCounter];

                }


            }
            else{
                int x,y;
                y = sz%size;
                x = (sz - y) / size;
                if(x == 0){//more cores than input numbers


                    localsum += arrtobcast[rank];

                }
                else{
                   if(rank < y){

                       for(forCounter = (x+1)*rank; forCounter<((x+1)*(rank+1));forCounter++){
                           localsum+=arrtobcast[forCounter];
                       }

                   }
                   else{
                       for(forCounter = ((x*rank)+y); forCounter< ((x * (rank +1) )+y);forCounter++){
                           localsum+=arrtobcast[forCounter];
                       }
                   }

                }

            }



        }

        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Allreduce(&localsum,&globalsum,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);

    }


    if(rank == 0){
        fprintf(stderr,"%d\n",globalsum);
    }
    MPI_Finalize();

}