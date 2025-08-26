#include <stdio.h>
#include <string.h>
#include <math.h>
#include "list.h"
#include "sparse_vector.h"
#include "PGE.h"
#include "SVR.h"
#include "linearcode.h"
#include "statistics.h"


void destroy_received_words(Bits** rec,unsigned int n,unsigned int Nrec){
    for(unsigned int k=0;k<Nrec;++k){
        destroy_vector(bits_to_vector(rec[k],n));
    }
    free(rec);
}

void destroy_bits(void* p){
    free((Bits*)p);
}

void parity_check(void* vector){
    Vector* vec=(Vector*)vector;
}

void free_dynamicaly_allocated_vector(void* p){
    free_vector(p);
    free(p);
}

int main (int argc,char **argv){
    srand(time(NULL));
    unsigned int n=12;
    unsigned int k=6;
    unsigned int p=0;
    unsigned int t=0;
    unsigned int ms=0;
    unsigned int ns=0;
    unsigned int weight=3;
    int circulant=2;
    unsigned int itLimit=100;
    double errorRate=0.000;
    
    if(argc>6){
        n=atoi(argv[1]);
        k=atoi(argv[2]);
        circulant=atoi(argv[3]);
        weight=atoi(argv[4]);
        errorRate=atof(argv[5]);
        itLimit=atoi(argv[6]);
    }
    


#ifdef COMPLEXITY
    //printf("complexity = %llu\n",get_complexity());
#endif




    return 0;
}

