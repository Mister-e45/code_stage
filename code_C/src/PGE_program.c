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

int main(int argc,char** argv){
    srand(time(NULL));

    unsigned int t=0;
    unsigned int p=0;
    unsigned int paritySearchIterLimit=0;
    unsigned int targetDimension;//number of independent parity check vectors to find
    unsigned int numberVecsUse=0;
    unsigned int circulantBlockSize;
    unsigned int itLimit=0;// maximum number of 
    double errorRate=0.0;
    char* filename;
    char* foundCodeFilename;

    int numFoundVectors=0;

    if(argc==11){
        t=atoi(argv[1]);
        p=atoi(argv[2]);
        numberVecsUse=atoi(argv[3]);
        errorRate=atof(argv[4]);
        targetDimension=atoi(argv[5]);
        circulantBlockSize=atoi(argv[6]);
        paritySearchIterLimit=atoi(argv[7]);
        itLimit=atoi(argv[8]);
        filename=argv[9];
        foundCodeFilename=argv[10];

    }
    else if(argc==10){
        t=atoi(argv[1]);
        p=atoi(argv[2]);
        numberVecsUse=atoi(argv[3]);
        targetDimension=atoi(argv[4]);
        circulantBlockSize=atoi(argv[5]);
        paritySearchIterLimit=atoi(argv[6]);
        itLimit=atoi(argv[7]);
        filename=argv[8];
        foundCodeFilename=argv[9];
    }
    else if(argc==1){
        printf("./pge [t]\n [p]\n [number of received words to use in the algorithm]\n [received words' error rate]\n [number of independent vectors to find]\n [quasi cyclic block size]\n [parity vector search iteration limit]\n [maximum number of iterations]\n [received words file name]\n [output code filename]\n");
        printf("or\n");
        printf("./pge [t]\n [p]\n [number of received words to use in the algorithm]\n [number of independent vectors to find]\n [quasi cyclic block size]\n [parity vector search iteration limit]\n [maximum number of iterations]\n [received words file name]\n [output code filename]\n");        
        printf("note that the unused words in the received words file will be used to perform a statiscal test on found vectors in order to assess the likelyhod of them being parity vectors\n");
        return 0;
    }
    else{
        printf("%u arguments provided but 10 or 11 required (error rate is optional if it is provided in received words file)\n",argc-1);
        return 0;
    }

    Bits** rec;
    unsigned int n;
    unsigned int k;
    unsigned int nbWords;
    if(argc==10){
        rec=load_received_code_words_from_file(filename,&n,&k,&nbWords,&errorRate);
    }
    else if(argc==11){
        rec=load_received_code_words_from_file(filename,&n,&k,&nbWords,NULL);
    }


    RawMatrix reconstructed=QCLDPC_reconstruction_PGE(rec,nbWords,numberVecsUse,&errorRate,n,targetDimension,circulantBlockSize,t,p,paritySearchIterLimit,itLimit);
    if(reconstructed.nrows==0){
        printf("did not find any vector\n");
    }
    else{
        printf("found %u parity check vectors\n",reconstructed.nrows);
        numFoundVectors=reconstructed.nrows;
    }


    LinearCode reconstructedCode=create_LinearCode(reconstructed,0);
    
    
    orthogonalize(&reconstructedCode);

    
    save_linear_code_in_file(reconstructedCode,foundCodeFilename);
    destroy_LinearCode(reconstructedCode);
    
    

    destroy_rawmatrix(reconstructed);
    destroy_received_words(rec,n,nbWords);

    return numFoundVectors;
    
    return 0;
}