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

    unsigned int ns=0;
    unsigned int ms=0;
    unsigned int w_thres=0;
    unsigned int d_s=0;
    unsigned int N_iter=0;
    unsigned int limSampling=0;
    unsigned int circulantSize=0;
    unsigned int targetDualDimension=0;
    double errorRate=0.0;
    char* filename;
    char* foundCodeFilename;

    int foundVectors=0;

    if(argc==12){
        ns=atoi(argv[1]);
        ms=atoi(argv[2]);
        w_thres=atoi(argv[3]);
        d_s=atoi(argv[4]);
        targetDualDimension=atoi(argv[5]);
        N_iter=atoi(argv[6]);
        circulantSize=atoi(argv[7]);
        limSampling=atoi(argv[8]);
        errorRate=atof(argv[9]);
        filename=argv[10];
        foundCodeFilename=argv[11];
    }
    else if(argc==11){
        ns=atoi(argv[1]);
        ms=atoi(argv[2]);
        w_thres=atoi(argv[3]);
        d_s=atoi(argv[4]);
        targetDualDimension=atoi(argv[5]);
        N_iter=atoi(argv[6]);
        circulantSize=atoi(argv[7]);
        limSampling=atoi(argv[8]);
        filename=argv[9];
        foundCodeFilename=argv[10];
    }
    else if(argc==1){
        printf("./svr [ns] \n[ms] \n[weight threshold]\n [d_s]\n [number of independent parity vectors to find]\n [sparse vector search number of iterations]\n [quasi cyclic bloc size]\n [maximum number of sampling and searches]\n [error rate]\n [received code words]\n [file name of output linear code]\n");
        printf("or\n");
        printf("./svr [ns]\n [ms]\n [weight threshold]\n [d_s]\n [sparse vector search number of iterations]\n [quasi cyclic bloc size]\n [maximum number of sampling and searches]\n [received code words]\n [file name of output linear code]\n\n");
        return 0;
    }
    else{
        printf("%u arguments provided but 10 or 11 required (error rate is optional if it is provided in received words file\n",argc-1);
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

    RawMatrix reconstructed=reconstruction_SVR(rec,nbWords,n,targetDualDimension,ns,ms,d_s,w_thres,N_iter,circulantSize,limSampling);
    if(reconstructed.nrows==0){
        printf("did not find any vector\n");
    }
    else{
        printf("found %u parity check vectors\n",reconstructed.nrows);
        foundVectors=reconstructed.nrows;
    }


    LinearCode reconstructedCode=create_LinearCode(reconstructed,0);
    
    
    orthogonalize(&reconstructedCode);

    
    save_linear_code_in_file(reconstructedCode,foundCodeFilename);
    destroy_LinearCode(reconstructedCode);
    
    

    destroy_rawmatrix(reconstructed);
    destroy_received_words(rec,n,nbWords);

    return foundVectors;
}