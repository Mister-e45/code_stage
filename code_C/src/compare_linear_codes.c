#include <stdio.h>
#include <string.h>
#include <math.h>
#include "list.h"
#include "sparse_vector.h"
#include "PGE.h"
#include "SVR.h"
#include "linearcode.h"
#include "statistics.h"


int main(int argc,char** argv){
    char* filename1;
    char* filename2;

    if(argc==3){
        filename1=argv[1];
        filename2=argv[2];
        printf("received args \n");
        printf("%s\n",filename1);
        printf("%s\n",filename2);
    }
    else if(argc==1){
        printf("./compareLinCodes [file name of linear code A] [file name of linear code B]\n");
        return 0;
    }

    LinearCode codeA;
    load_linear_code_from_file(&codeA,filename1);
    LinearCode codeB;
    load_linear_code_from_file(&codeB,filename2);

    if(linearcode_is_equal(codeA,codeB)){
        printf("A=B\n");
    }
    else if(linearcode_C1_included_in_C2(codeA,codeB)){
        printf("A is in B\n");
    }
    else if(linearcode_C1_included_in_C2(codeB,codeA)){
        printf("B is in A\n");
    }
    if(codeA.dimension==0 || codeA.length==0){
        printf("A is null space\n");
    }
    if(codeB.dimension==0 || codeB.length==0){
        printf("B is null space\n");
    }

    destroy_LinearCode(codeA);
    destroy_LinearCode(codeB);

    return 0;
}