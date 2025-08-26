#include <stdio.h>
#include <string.h>
#include <math.h>
#include "list.h"
#include "sparse_vector.h"
#include "PGE.h"
#include "SVR.h"
#include "linearcode.h"
#include "statistics.h"

/**generates a file of noisy code words from a linear code. The linear code is obtained from a file.
 * The output file is of format :
 * [words length] (unsigned int) 
 * [number of words] (unsigned int) 
 * [words data](blocks of type Bits) 
 * [dimension of the code] (unsigned int) 
 * [errorRate] (double)
 */

 void destroy_received_words(Bits** rec,unsigned int n,unsigned int Nrec){
    for(unsigned int k=0;k<Nrec;++k){
        destroy_vector(bits_to_vector(rec[k],n));
    }
    free(rec);
}

int main(int argc,char** argv){

    unsigned int Nb=0;
    double errorRate=0.0;
    char* filename;
    char* outFilename;

    if(argc==5){
        Nb=atoi(argv[1]);
        errorRate=atof(argv[2]);
        filename=argv[3];
        outFilename=argv[4];
    }
    else if(argc==1){
        printf("./wordgen [number of words] [error rate] [linear code's file name] [output file name]\n");
        return 0;
    }
    else if(argc==2){
        unsigned int n;
        unsigned int k;
        unsigned int nbwords;
        double errorrate;
        Bits** rec=load_received_code_words_from_file(argv[1],&n,&k,&nbwords,&errorrate);
        printf("file %s contains: \n",argv[1]);
        printf("%u code words of length %u\n",nbwords,n);
        printf("they were generated with error rate of %f\n",errorrate);
        destroy_received_words(rec,n,nbwords);
        return 0;
    }
    else{
        printf("%u arguments provided but 4 required\n",argc-1);
        return 0;
    }

    LinearCode code;
    load_linear_code_from_file(&code,filename);
    Bits** words=linearcode_get_some_noisy_code_words(code,Nb,&errorRate);
    FILE* file=fopen(outFilename,"wb");
    fwrite(&(code.length),sizeof(unsigned int),1,file);
    fwrite(&Nb,sizeof(unsigned int),1,file);

    unsigned int numBitsBlocks=bitToBits(code.length);
    for(unsigned int k=0;k<Nb;++k){
        fwrite(words[k],sizeof(Bits),numBitsBlocks,file);
    }
    fwrite(&(code.dimension),sizeof(unsigned int),1,file);
    fwrite(&errorRate,sizeof(double),1,file);

    fclose(file);

    for(unsigned int k=0;k<Nb;++k){
        Vector vec=bits_to_vector(words[k],code.length);
        destroy_vector(vec);
    }
    free(words);
    destroy_LinearCode(code);


    return 0;
}