//
// Created by alican on 2/26/19.
//
#include <stdio.h>
#include <mpi.h>

void main(int argc,char **argv){
    int size,rank;
    MPI_Status s;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if(rank==0){ //master
        int total = 0;
        int sz , i;
        sz = 0;
        FILE *fp ;
        fp = fopen(argv[argc-1],"r");
        while(!feof(fp)){


            fscanf(fp,"%d",&i);
            sz++;

        }
        sz--;
        rewind(fp);



        if(sz % size == 0){

            int st = sz/size;
            int arr[st];
            int arrtosend[st];
            int forCounter,forCounter2;
            for(forCounter = 0; forCounter < st; forCounter++){
                fscanf(fp,"%d",&arr[forCounter]);
            }
            for(forCounter=0;forCounter<size-1;forCounter++){
                for(forCounter2=0;forCounter2<st;forCounter2++){
                    fscanf(fp,"%d",&arrtosend[forCounter2]);
                }
                //send total size for checking purposes (#inputs < #processors)
                MPI_Send ((void *)&sz,1,MPI_INT,(forCounter+1),0xACE5,MPI_COMM_WORLD );
                //send array size
                MPI_Send ((void *)&st,1,MPI_INT,(forCounter+1),0xACE5,MPI_COMM_WORLD );
                //send actual array
                MPI_Send ((void *)&arrtosend,st,MPI_INT,(forCounter+1),0xACE5,MPI_COMM_WORLD );
            }
            for(forCounter=0;forCounter<st;forCounter++){
                total+=arr[forCounter];
            }

        }
        else{

            int rem,remc;
            rem = sz % size;
            remc = 0;
            int st = (sz - rem)/size;
            int st2 = st+1;
            int arr[st+1];
            remc++;
            int arrtosend1[st+1];
            int arrtosend2[st];
            int forCounter,forCounter2;
            for(forCounter=0;forCounter<st+1;forCounter++){
                fscanf(fp,"%d",&arr[forCounter]);
            }

            for(forCounter=0;forCounter<size-1;forCounter++){
                if(remc < rem) {
                    for (forCounter2 = 0;forCounter2<st+1;forCounter2++){
                        fscanf(fp,"%d",&arrtosend1[forCounter2]);
                    }
                    //send total size for checking purposes (#inputs < #processors)
                    MPI_Send ((void *)&sz,1,MPI_INT,(forCounter+1),0xACE5,MPI_COMM_WORLD );
                    //Send array size
                    MPI_Send ((void *)&st2,1,MPI_INT,(forCounter+1),0xACE5,MPI_COMM_WORLD);
                    //send actual array
                    MPI_Send((void *)&arrtosend1,(st+1),MPI_INT,(forCounter+1),0xACE5,MPI_COMM_WORLD );
                    remc++;
                }
                else{
                    for(forCounter2=0;forCounter2<st;forCounter2++){
                        fscanf(fp,"%d",&arrtosend2[forCounter2]);
                    }
                    //send total size for checking purposes (#inputs < #processors)
                    MPI_Send ((void *)&sz,1,MPI_INT,(forCounter+1),0xACE5,MPI_COMM_WORLD );
                    //send array size
                    MPI_Send ((void *)&st,1,MPI_INT,(forCounter+1),0xACE5,MPI_COMM_WORLD );
                    //send actual array
                    MPI_Send ((void *)&arrtosend2,st,MPI_INT,(forCounter+1),0xACE5,MPI_COMM_WORLD );

                }
            }
            for(forCounter=0;forCounter<st+1;forCounter++){
                total+=arr[forCounter];
            }


        }

        int arriving,forCounter;

        for(forCounter=0;forCounter<size-1;forCounter++){
            MPI_Recv((void*)&arriving,1,MPI_INT,(forCounter+1),0xACE5,MPI_COMM_WORLD,&s);
            total += arriving;
        }
        if(size > sz){
            //todo -> find a better way if possible
            total -= arriving*(size-sz);

        }
        //done its own part
        fclose(fp);
        fprintf(stderr,"%d\n",total);

    }
    else{
        int size,sz;
        MPI_Recv((void *)&sz,1,MPI_INT,0,0xACE5,MPI_COMM_WORLD, &s);

        MPI_Recv((void *)&size,1,MPI_INT,0,0xACE5,MPI_COMM_WORLD, &s);
        int myarr[size];
        MPI_Recv((void *)&myarr,size,MPI_INT,0,0xACE5,MPI_COMM_WORLD,&s);
        int mytotal;
        mytotal=0;
        int forCounter;
        if(sz <= rank){
            //donothing
        }
        else {
            for (forCounter = 0; forCounter < size; forCounter++) {
                //fprintf(stderr,"rank: %d adding %d size: %d\n",rank,myarr[forCounter],size);
                mytotal += myarr[forCounter];
            }
        }
        MPI_Send((void *)&mytotal,1,MPI_INT,0,0xACE5,MPI_COMM_WORLD);


    }

    MPI_Finalize();

}