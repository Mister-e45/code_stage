#include <stdlib.h>
#include "linearcode.h"


int main(int argc,char** argv){
    srand(time(NULL));
    unsigned int n=0;
    unsigned int k=0;
    unsigned int blockSize=0;
    unsigned int weight=0;
    char* filename;

    if(argc==6){
        n=atoi(argv[1]);
        k=atoi(argv[2]);
        blockSize=atoi(argv[3]);
        weight=atoi(argv[4]);
        filename=argv[5];
    }
    else if(argc==1){
        printf("./qcldpc [code length] [dimension] [quasi cyclic block size] [weight of vectors generating the dual space] [file name of output]\n");
        return 0;
    }
    else if(argc==2){
        QC_LDPC code;
        load_qc_ldpc_code_from_file(&code,argv[1]);
        printf("qcldpc in file %s is: \n",argv[1]);
        printf("code length: %u\n",code.code.length);
        printf("code dimension: %u\n",code.code.dimension);
        printf("circulant block size: %u\n",code.blockWidth);

        destroy_qcldpc(code);
        return 0;
    }
    else{
        printf("%u arguments provided but 5 required\n",argc-1);
        return 0;
    }

    QC_LDPC C;
    C=create_random_qc_ldpc_const_weight(n,k,blockSize,weight);
    save_linear_code_in_file(C.code,filename);
    FILE* file=fopen(filename,"ab");
    if(file==NULL){
        printf("could not open file to complete saving\n");
        return -1;
    }
    fwrite(&blockSize,sizeof(unsigned int),1,file);
    fclose(file);

    destroy_qcldpc(C);

}