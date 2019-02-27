//
// Created by alican on 2/26/19.
//
#include <stdio.h>

int main(int argc, char **argv) {
    FILE *fp;
    fp = fopen(argv[1], "r");
    int i;
    int sum = 0;
    while(!feof(fp)){
        fscanf(fp,"%d",&i);
        sum += i;
    }

    fprintf(stderr,"%d\n",sum);
    fclose(fp);
    return 0;
}