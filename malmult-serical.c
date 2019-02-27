//
// Created by alican on 2/26/19.
//

#include <stdio.h>

int main(int argc, char **argv){

    FILE *fp1,*fp2,*fp3;
    fp1=fopen(argv[1],"r");
    fp2=fopen(argv[2],"r");
    fp3=fopen(argv[3],"w+");

    int size1,size2;

    fscanf(fp1,"%d",&size1);
    fscanf(fp2,"%d",&size2);
    if(size1 != size2)
        return -1;


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

    int sum;
    sum = 0;
    for (forCounter1 = 0;forCounter1  < size1 ; forCounter1++) {
        for (forCounter2 = 0; forCounter2 < size1; forCounter2++) {
            for (forCounter3 = 0; forCounter3 < size1; forCounter3++) {
                sum += arr1[forCounter1][forCounter3] * arr2[forCounter3][forCounter2];

            }
            arr3[forCounter1][forCounter2] = sum;
            sum = 0;
        }
    }

    for(forCounter1 = 0; forCounter1<size1;forCounter1++){
        for (forCounter2 = 0; forCounter2 <size1 ; forCounter2++) {
            fprintf(fp3,"%d ",arr3[forCounter1][forCounter2]);
        }
        fprintf(fp3,"\n");
    }


}