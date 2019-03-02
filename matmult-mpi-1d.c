//
// Created by alican on 2/27/19.
//

#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <stdlib.h>

int main(int argc,char **argv){

    int size,rank;
    MPI_Status s;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    if(rank==0){//master

        int noOfp = size;


        FILE *fp1,*fp2,*fp3;
        fp1=fopen(argv[argc-3],"r");
        fp2=fopen(argv[argc-2],"r");

        fp3=fopen(argv[argc-1],"w+");

        int size1,size2;

        fscanf(fp1,"%d",&size1);


        fscanf(fp2,"%d",&size2);
        if(size1 != size2){


            return -1;


        }


        int forCounter1,forCounter2,forCounter3;
        int arr1[size1][size1],arr2[size1][size1],arr3[size1][size1];
        for(forCounter1 = 0; forCounter1<size1;forCounter1++){
            for (forCounter2 = 0; forCounter2 < size1; forCounter2++) {
                fscanf(fp1,"%d",&arr1[forCounter1][forCounter2]);

            }
        }

        for(forCounter1 = 0; forCounter1<size1;forCounter1++){
            for (forCounter2 = 0; forCounter2 < size1; forCounter2++) {
                fscanf(fp2,"%d",&arr2[forCounter1][forCounter2]);

            }
        }


        double pDPerLine;

        pDPerLine = sqrt(noOfp);

        int pPL;
        pPL = (int)pDPerLine;

        int lPP;

        lPP = size1/pPL;

        int myarr1[lPP][size1],myarr2[size1][lPP],leArrayFinale[size1][size1];
        int arrtosend[size1];



        //initialize its own array

        for (forCounter1 = 0; forCounter1 < lPP ; forCounter1++) {
            for (forCounter2 = 0; forCounter2 < size1 ; forCounter2++) {
                myarr1[forCounter1][forCounter2] = arr1[forCounter1][forCounter2];

            }
        }


        for (forCounter1 = 0; forCounter1 < size1 ; forCounter1++) {
            for (forCounter2 = 0; forCounter2 < lPP ; forCounter2++) {
                myarr2[forCounter1][forCounter2] = arr2[forCounter1][forCounter2];

            }
        }



        //send (lPP) 1d arrays then create 2D array on the workers
        int unnecesaryCounter1,unnecesaryCounter2;
        unnecesaryCounter1=1;
        unnecesaryCounter2=0;
        for(forCounter1 = 0;forCounter1<size-1;forCounter1++){//perWorker
            //send # of lines
            MPI_Send ((void *)&lPP,1,MPI_INT,(forCounter1+1),0xACE5,MPI_COMM_WORLD );
            //send total size
            MPI_Send ((void *)&size1,1,MPI_INT,(forCounter1+1),0xACE5,MPI_COMM_WORLD );


            for(forCounter2=0;forCounter2<lPP;forCounter2++){
                for(forCounter3=0;forCounter3<size1;forCounter3++){
                    arrtosend[forCounter3] = arr1[forCounter2 + (unnecesaryCounter2*lPP)][forCounter3];
                }
                MPI_Send ((void *)&arrtosend,size1,MPI_INT,(forCounter1+1),0xACE5,MPI_COMM_WORLD );

            }
            if(unnecesaryCounter1 == pPL-1){
                unnecesaryCounter1 =0;
                unnecesaryCounter2++;
            }
            else
                unnecesaryCounter1++;
        }
        int arrtosend2[lPP];
        unnecesaryCounter1=1;
        unnecesaryCounter2=0;
        //same for second matrix
        for(forCounter1 = 0;forCounter1<size-1;forCounter1++){//perWorker
            //send # of lines not required since its already known
            //MPI_Send ((void *)&lPP,1,MPI_INT,(forCounter1+1),0xACE5,MPI_COMM_WORLD );

            for(forCounter2=0;forCounter2<size1;forCounter2++){
                for(forCounter3=0;forCounter3<lPP;forCounter3++){
                    arrtosend2[forCounter3] = arr2[forCounter2][forCounter3 + (unnecesaryCounter1*lPP)];
                    //fprintf(stderr,"arr2[x] = %d, arr2[y] = %d \n",forCounter2 + (unnecesaryCounter2*lPP),forCounter3 + (unnecesaryCounter1*lPP));

                    //fprintf(stderr,"sending %d. array to child %d [%d] = %d \n",forCounter2,forCounter1,forCounter3,arr2[forCounter2 + (unnecesaryCounter2*lPP)][forCounter3 + (unnecesaryCounter1*lPP)]);

                }
                MPI_Send ((void *)&arrtosend2,lPP,MPI_INT,(forCounter1+1),0xACE5,MPI_COMM_WORLD );

            }
            if(unnecesaryCounter1 == pPL-1){
                unnecesaryCounter1 =0;
                unnecesaryCounter2++;
            }
            else
                unnecesaryCounter1++;
        }
        int sum;
        sum = 0;
        //calculate its own
        unnecesaryCounter1 = 0;
        unnecesaryCounter2 = 0;
        for (forCounter1 = 0;forCounter1  < lPP ; forCounter1++) {
            for (forCounter2 = 0; forCounter2 < lPP; forCounter2++) {
                for (forCounter3 = 0; forCounter3 < size1; forCounter3++) {
                    sum += myarr1[forCounter1][forCounter3] * myarr2[forCounter3][forCounter2];

                }
                //its okay to directly write to final array since master starts at 0,0 and goes to lPPxlPP
                leArrayFinale[forCounter1][forCounter2] = sum;
                sum = 0;
            }
        }
        int incomingarr[lPP];
        unnecesaryCounter1 = 1;
        unnecesaryCounter2 = 0;

        //get resulting array from workers
        for(forCounter1 = 0; forCounter1<size-1;forCounter1++){//for each child
            for(forCounter2=0; forCounter2<lPP;forCounter2++){//it will recieve lPP number of arrays
                MPI_Recv((void *)&incomingarr,lPP,MPI_INT,(forCounter1+1),0xACE5,MPI_COMM_WORLD,&s);
                for(forCounter3 = 0; forCounter3<lPP;forCounter3++){//write incoming arrays to final array
                    leArrayFinale[forCounter2+(unnecesaryCounter2*lPP)][forCounter3+(unnecesaryCounter1*lPP)] = incomingarr[forCounter3];
                    //arrtosend[forCounter3] = arr1[forCounter2 + (unnecesaryCounter2*lPP)][forCounter3 + (unnecesaryCounter1*lPP)];

                }
            }

            if(unnecesaryCounter1 == pPL-1){
                unnecesaryCounter1 =0;
                unnecesaryCounter2++;
            }
            else
                unnecesaryCounter1++;
            //please work
        }

        //at this point array must be completely done *hopefully* then we will write to file
        for(forCounter1 = 0; forCounter1<size1;forCounter1++){
            for (forCounter2 = 0; forCounter2 <size1 ; forCounter2++) {
                fprintf(fp3,"%d ",leArrayFinale[forCounter1][forCounter2]);
            }
            fprintf(fp3,"\n");
        }

    }
    else{

        int lPP,sum,size1;
        //get size
        MPI_Recv((void *)&lPP,1,MPI_INT,0,0xACE5,MPI_COMM_WORLD, &s);
        MPI_Recv((void *)&size1,1,MPI_INT,0,0xACE5,MPI_COMM_WORLD, &s);

        int myarr1[lPP][size1],myarr2[size1][lPP],finalarr[lPP][lPP];
        int recarr1[size1],recarr2[lPP];
        int forCounter1,forCounter2,forCounter3;

        //initialize arrays
        for(forCounter1=0;forCounter1<lPP;forCounter1++) {
            MPI_Recv((void *)&recarr1, size1, MPI_INT, 0, 0xACE5, MPI_COMM_WORLD, &s);
            for (forCounter2 = 0; forCounter2 < size1; forCounter2++) {
                myarr1[forCounter1][forCounter2] = recarr1[forCounter2];
            }
        }
        for(forCounter1=0;forCounter1<size1;forCounter1++) {
            MPI_Recv((void *) &recarr2, lPP, MPI_INT, 0, 0xACE5, MPI_COMM_WORLD, &s);
            for (forCounter2 = 0; forCounter2 < lPP; forCounter2++) {
                myarr2[forCounter1][forCounter2] = recarr2[forCounter2];
            }
        }
        //calculate
        sum = 0;
        for (forCounter1 = 0;forCounter1  < lPP ; forCounter1++) {
            for (forCounter2 = 0; forCounter2 < lPP; forCounter2++) {
                for (forCounter3 = 0; forCounter3 < size1; forCounter3++) {
                    sum += myarr1[forCounter1][forCounter3] * myarr2[forCounter3][forCounter2];

                }
                finalarr[forCounter1][forCounter2] = sum;
                sum = 0;
            }
        }
        //send back result array
        for(forCounter1 = 0;forCounter1<lPP;forCounter1++){
            MPI_Send((void *)&finalarr[forCounter1],lPP,MPI_INT,0,0xACE5,MPI_COMM_WORLD );
        }

    }

    MPI_Finalize();

    return 0;
}