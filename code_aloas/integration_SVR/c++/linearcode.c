#include "linearcode.h"


LinearCode create_LinearCode(RawMatrix generator,int isFullRank){
    LinearCode code;
    code.parityMatrix.ncols=0;
    code.parityMatrix.nrows=0;
    code.parityMatrix.rows=NULL;
    code.length=generator.ncols;
    code.dimension=0;
    code.generatorMatrix=rawmatrix_copy(generator);
    if(isFullRank){
        code.dimension=generator.nrows;
    }
    else if(code.generatorMatrix.nrows>0){
        unsigned int rank=row_echelonize_rawmatrix(code.generatorMatrix);
        ++rank;
        Bits** resizedMatrix=calloc(rank,sizeof(Bits*));
        if(resizedMatrix==0){
            printf("could not created resized matrix in computation of genrator matrix for a linear code\n");
        }
        else{
            for(unsigned int i=0;i<rank;++i){
                resizedMatrix[i]=code.generatorMatrix.rows[i];
            }
            for(unsigned int i=rank;i<code.generatorMatrix.nrows;++i){
                free(code.generatorMatrix.rows[i]);
            }
            free(code.generatorMatrix.rows);
            code.generatorMatrix.rows=resizedMatrix;
            code.generatorMatrix.nrows=rank;
            code.dimension=rank;
        }

    }
    return code;
}

RawMatrix generate_circulant_rawmatrix_from_vectors(Bits** vecsBits,unsigned int n,unsigned int numVecs,unsigned int circulantWidth){
    RawMatrix mat;
    unsigned int orbitSize=n/circulantWidth;
    unsigned int numRows=orbitSize*numVecs;
    Bits** binMat=calloc(numRows,sizeof(Bits*));
    if(binMat==0){
        printf("could not allocate memory to create circulant block matrix \n");
        mat.ncols=0;
        mat.nrows=0;
        mat.rows=0;
        return mat;
    }
    else{
        for(unsigned int s=0;s<numVecs;++s){
            binMat[s*orbitSize]=vecsBits[s];
            for(unsigned int l=1;l<orbitSize;++l){
                Vector previousRow= bits_to_vector(binMat[s*orbitSize+l-1],n);
                binMat[s*orbitSize+l]=cyclic_right_shift_vector(previousRow,circulantWidth).bits;
            }
        }
        mat.rows=binMat;
        mat.nrows=numRows;
        mat.ncols=n;
        return mat;
    }
    return mat;
}


QC_LDPC create_random_qc_ldpc_const_weight(unsigned int n,unsigned int dim,unsigned int circulantWidth,unsigned int weight){
    QC_LDPC C;
    if(n%circulantWidth !=0){
        printf("incompatible circulant width with requested code length\n");
        return C;
    }
    else{
        unsigned int orbitSize=n/circulantWidth;
        if((n-dim)%orbitSize!=0){
            printf("incompatible circulant block width with requested dimension\n");
        }
        else{
            unsigned int g=(n-dim)/orbitSize; // how many rows of circulant matrices there will be
            C.code.length=n;
            C.code.dimension=0;
            C.blockWidth=circulantWidth;
            while(C.code.dimension!=dim){
                Bits** vecs=calloc(g,sizeof(Bits*));
                if(vecs==0){
                    printf("could not allocate memory to store the vectors that generate the circulant blocks\n");
                    return C;
                }
                else{
                    for(unsigned int l=0;l<g;++l){
                        vecs[l]=const_weight_random_vector(n,weight).bits;
                    }
                    C.code.parityMatrix=generate_circulant_rawmatrix_from_vectors(vecs,n,g,circulantWidth);
                    free(vecs);
                    C.code.generatorMatrix=dual_code_generator_matrix(C.code.parityMatrix);
                    if(C.code.generatorMatrix.nrows!=dim){ 
                        destroy_qcldpc(C);
                        printf("randomly generated parity check matrix did not yield a %u-dimension code but a %u-dimension one instead\n",dim,C.code.generatorMatrix.nrows);
                    }
                    else{
                        C.code.dimension=C.code.generatorMatrix.nrows;
                        return C;
                    }
                }
            }
        }
        return C;
    }
    return C;
}

void destroy_qcldpc(QC_LDPC C){
    destroy_LinearCode(C.code);
    C.blockWidth=0;
}

RawMatrix linearcode_get_parity_check_matrix(LinearCode code){
    if(code.generatorMatrix.rows==NULL && code.generatorMatrix.ncols!=0){
        RawMatrix parityCheck=identity_rawmatrix(code.length);
        return parityCheck;
    }
    else if(code.parityMatrix.ncols==0){// then we need to compute the party check matrix
        code.parityMatrix=dual_code_generator_matrix(code.generatorMatrix);
        return code.parityMatrix;
    }
    else{
        return code.parityMatrix;
    }
}

RawMatrix generate_partial_parity_no_copy(RawMatrix mat,Permutation sigma_col){  //the non identity part of a parity check matrix derived from a systematic form matrix. codeWords will be altered!!
    unsigned int rank = row_reduce_rawmatrix_with_column_permutation(mat,sigma_col);
    if(rank<mat.nrows){
        for(unsigned int k=rank;k<mat.nrows;++k){
            free(mat.rows[k]); // we free the null rows
            mat.rows[k]=0;
        }
        mat.nrows=rank;
    }
    unsigned int dualDim=mat.ncols-rank;
    Bits** A=calloc(dualDim,sizeof(Bits*));
    RawMatrix partialParityCheckMatrix;
    partialParityCheckMatrix.nrows=dualDim;
    partialParityCheckMatrix.ncols=rank;
    if(A==0){
        printf("could not allocate memory to create partial parity check matrix\n");
    }
    else{
        //printf("dualdim=%i and rank=%u and ncols=%u\n",dualDim,rank,mat.ncols);
        for(unsigned k=rank;k<mat.ncols;++k){
            Vector column=rawmatrix_get_column_copy(mat,sigma_col.permutation[k]);
            A[k-rank]=column.bits;
        }
        partialParityCheckMatrix.rows=A;
    }

    for(unsigned int i=0;i<mat.nrows;++i){//we have to free the rows ourselves because we ignored and free the null rows ourselves
        free(mat.rows[i]);
    }

    return partialParityCheckMatrix;
}

RawMatrix generate_partial_parity(RawMatrix matrix,Permutation sigma_col){  //the non identity part of a parity check matrix derived from a systematic form matrix. codeWords will be altered!!
    RawMatrix mat=rawmatrix_copy(matrix);
    RawMatrix parity=generate_partial_parity_no_copy(mat,sigma_col);
    free(mat.rows);
    return parity;
}


RawMatrix dual_code_generator_matrix_with_permutation(RawMatrix mat,Permutation sigma){
    RawMatrix partialParity=generate_partial_parity(mat,sigma);
    RawMatrix result;
    result.ncols=mat.ncols;
    result.nrows=partialParity.nrows;
    result.rows=calloc(result.nrows,sizeof(Bits*));
    if(result.rows==0){
        printf("could not create rawmatrix to store dual code generator\n");
        result.ncols=0;
        result.nrows=0;
        return result;
    }
    unsigned int numBlocks=bitToBits(partialParity.ncols);
    for(unsigned int i=0;i<result.nrows;++i){
        Vector row=create_vector(result.ncols);
        for(unsigned int k=0;k<numBlocks;++k){
            row.bits[k]=partialParity.rows[i][k];
        }
        put_coordinate(row,partialParity.ncols+i,1);
        result.rows[i]=row.bits;
    }// at the end of this loop result is a matrix ot the form [A|Id] where A is the transposed non identity block of the mat in it's systematic form

    destroy_rawmatrix(partialParity);

    return result;
}

RawMatrix dual_code_generator_matrix_with_permutation_no_copy(RawMatrix mat,Permutation sigma){
    RawMatrix result;
    result.ncols=mat.ncols;

    RawMatrix partialParity=generate_partial_parity_no_copy(mat,sigma);

    result.nrows=partialParity.nrows;

    result.rows=calloc(result.nrows,sizeof(Bits*));
    if(result.rows==0){
        printf("could not create rawmatrix to store dual code generator\n");
        result.ncols=0;
        result.nrows=0;
        return result;
    }
    unsigned int numBlocks=bitToBits(partialParity.ncols);
    for(unsigned int i=0;i<result.nrows;++i){
        Vector row=create_vector(result.ncols);
        for(unsigned int k=0;k<numBlocks;++k){
            row.bits[k]=partialParity.rows[i][k];
        }
        put_coordinate(row,partialParity.ncols+i,1);
        result.rows[i]=row.bits;
    }// at the end of this loop result is a matrix ot the form [A|Id] where A is the transposed non identity block of the mat in it's systematic form

    destroy_rawmatrix(partialParity);

    return result;
}

RawMatrix dual_code_generator_matrix(RawMatrix mat){
    Permutation sigma_col=create_permutation(mat.ncols);
    RawMatrix partialParity=generate_partial_parity(mat,sigma_col);
    Permutation inv=inverse_permutation(sigma_col);
    RawMatrix result;
    result.ncols=mat.ncols;
    result.nrows=partialParity.nrows;
    result.rows=calloc(result.nrows,sizeof(Bits*));
    if(result.rows==0){
        printf("could not create rawmatrix to store dual code generator\n");
        result.ncols=0;
        result.nrows=0;
        return result;
    }
    unsigned int numBlocks=bitToBits(partialParity.ncols);
    for(unsigned int i=0;i<result.nrows;++i){
        Vector row=create_vector(result.ncols);
        for(unsigned int k=0;k<numBlocks;++k){
            row.bits[k]=partialParity.rows[i][k];
        }
        put_coordinate(row,partialParity.ncols+i,1);
        permute_vector(inv,&row);
        result.rows[i]=row.bits;
    }// at the end of this loop result is a matrix ot the form [A|Id] where A is the transposed non identity block of the mat in it's systematic form



    destroy_permutation(sigma_col);
    destroy_permutation(inv);
    destroy_rawmatrix(partialParity);

    return result;
    
}

RawMatrix dual_code_generator_matrix_no_copy(RawMatrix mat){
    Permutation sigma_col=create_permutation(mat.ncols);
    RawMatrix partialParity=generate_partial_parity_no_copy(mat,sigma_col); // the difference 
    Permutation inv=inverse_permutation(sigma_col);
    RawMatrix result;
    result.ncols=mat.ncols;
    result.nrows=partialParity.nrows;
    result.rows=calloc(result.nrows,sizeof(Bits*));
    if(result.rows==0){
        printf("could not create rawmatrix to store dual code generator\n");
        result.ncols=0;
        result.nrows=0;
        return result;
    }
    unsigned int numBlocks=bitToBits(partialParity.ncols);
    for(unsigned int i=0;i<result.nrows;++i){
        Vector row=create_vector(result.ncols);
        for(unsigned int k=0;k<numBlocks;++k){
            row.bits[k]=partialParity.rows[i][k];
        }
        put_coordinate(row,partialParity.ncols+i,1);
        permute_vector(inv,&row);
        result.rows[i]=row.bits;
    }// at the end of this loop result is a matrix ot the form [A|Id] where A is the transposed non identity block of the mat in it's systematic form



    destroy_permutation(sigma_col);
    destroy_permutation(inv);

    return result;
}

int vector_is_in_code(LinearCode code,Vector vector){
    RawMatrix mat;
    mat.ncols=code.length;
    mat.nrows=code.dimension+1;
    mat.rows=calloc(mat.nrows,sizeof(Bits*));
    if(mat.rows==0){
        printf("could not create temporary matrix to check belonging of a vector in a linear code\n");
        return -1;
    }
    else{
        for(unsigned int i=0;i<mat.nrows-1;++i){
            mat.rows[i]=vector_copy(rawmatrix_get_row_vector(code.generatorMatrix,i)).bits; // we put into mat a copy of the row i of the generator matrix of the code
        }
        mat.rows[mat.nrows-1]=vector_copy(vector).bits;

        unsigned int rank=row_echelonize_rawmatrix(mat);// row_echelonize_rawmatrix gives the index of the last non null row so the rank is equal to +1 of that value
        destroy_rawmatrix(mat);
        if(rank+1==code.dimension){// if we have not gained in rank then the vector is in the code
            return 1;
        }
        else{
            return 0;
        }
    }
}

int vectors_are_in_code(LinearCode code,Vector* vectors,unsigned int numberOfVectors){
    RawMatrix mat;
    mat.ncols=code.length;
    mat.nrows=code.dimension+numberOfVectors;
    mat.rows=calloc(mat.nrows,sizeof(Bits*));
    if(mat.rows==0){
        printf("could not create temporary matrix to check belonging of a vector in a linear code\n");
        return -1;
    }
    else{
        for(unsigned int i=0;i<mat.nrows-numberOfVectors;++i){
            mat.rows[i]=vector_copy(rawmatrix_get_row_vector(code.generatorMatrix,i)).bits; // we put into mat a copy of the row i of the generator matrix of the code
        }
        for(unsigned int i=0;i<numberOfVectors;++i){
            mat.rows[mat.nrows-numberOfVectors+i]=vector_copy(vectors[i]).bits;
        }

        unsigned int rank=row_echelonize_rawmatrix(mat);// row_echelonize_rawmatrix gives the index of the last non null row so the rank is equal to +1 of that value
        destroy_rawmatrix(mat);
        if(rank+1==code.dimension){// if we have not gained in rank then the vectors are all in the code
            return 1;
        }
        else{
            return 0;
        }
    }
}

int vector_is_in_dual_code(LinearCode code,Vector vector){
    RawMatrix temp=linearcode_get_parity_check_matrix(code);
    unsigned int dim=code.dimension;
    code.parityMatrix=code.generatorMatrix;
    code.generatorMatrix=temp;
    code.dimension=code.length-dim;
    int isInDual=vector_is_in_code(code,vector);
    return isInDual;
}

int vectors_are_in_dual_code(LinearCode code,Vector* vectors,unsigned int numberOfVectors){
    RawMatrix temp=linearcode_get_parity_check_matrix(code);
    unsigned int dim=code.dimension;
    code.parityMatrix=code.generatorMatrix;
    code.generatorMatrix=temp;
    code.dimension=code.length-dim;
    int areInDual=vectors_are_in_code(code,vectors,numberOfVectors);
    return areInDual;
}

int linearcode_C1_included_in_C2(LinearCode C1,LinearCode C2){
    Vector* gens=rawmatrix_as_vector_array(C1.generatorMatrix);
    if(vectors_are_in_code(C2,gens,C1.dimension)){
        free(gens);
        return 1;
    }
    else{
        free(gens);
        return 0;
    }
}

int linearcode_is_equal(LinearCode C1,LinearCode C2){
    return linearcode_C1_included_in_C2(C1,C2) && linearcode_C1_included_in_C2(C2,C1);
}

Vector linearcode_encode(LinearCode* code,Vector* message){
    return left_multiply(*message,code->generatorMatrix);
}

Bits** linearcode_get_some_noisy_code_words(LinearCode code,unsigned int numberOfWords,double* errorProbability){
    Vector message=create_vector(code.dimension);
    Vector noise=create_vector(code.length);
    Bits** noisyCodeWords;
    noisyCodeWords=calloc(numberOfWords,sizeof(Bits*));
    if(noisyCodeWords==0){
        printf("could not allocate memory to store generated noisy code words\n");
        return noisyCodeWords;
    }
    for(unsigned int k=0;k<numberOfWords;++k){
        write_random_bits_in_vector(message);
        write_error_positions_in_vector(noise,errorProbability);
        Vector codeWord=linearcode_encode(&code,&message);
        vector_add(codeWord,noise,codeWord);
        noisyCodeWords[k]=codeWord.bits;
    }

    destroy_vector(message);
    destroy_vector(noise);

    return noisyCodeWords;
}

Vector syndrome(LinearCode code,Vector vector){
    if(code.parityMatrix.rows!=0){
        return right_multiply(code.parityMatrix,vector);
    }
    else{
        RawMatrix mat=linearcode_get_parity_check_matrix(code);
        return right_multiply(mat,vector);
    }
}

unsigned int syndrome_weight(LinearCode code,Vector vector){
    Vector synd=syndrome(code,vector);
    unsigned int w=HW(synd);
    destroy_vector(synd);
    return w;
}


void destroy_LinearCode(LinearCode code){
    destroy_rawmatrix(code.generatorMatrix);
    destroy_rawmatrix(code.parityMatrix);
    code.dimension=0;
    code.length=0;
}

int save_linear_code_in_file(LinearCode code,char* filename){
    FILE* file=fopen(filename, "wb");
    if(file==NULL){
        printf("could not open file to store linear code\n");
        return 0;
    }
    fwrite(&code.length,sizeof(unsigned int),1,file);
    fwrite(&code.dimension,sizeof(unsigned int),1,file);
    unsigned int numBlocks=bitToBits(code.length);
    for(unsigned int i=0;i<code.dimension;++i){
        fwrite(code.generatorMatrix.rows[i],sizeof(Bits),numBlocks,file);
    }
    RawMatrix parityMat=linearcode_get_parity_check_matrix(code);
    for(unsigned int i=0;i<code.length-code.dimension;++i){
        fwrite(parityMat.rows[i],sizeof(Bits),numBlocks,file);
    }
    fclose(file);
}

int load_linear_code_from_file(LinearCode* code,char* filename){
    FILE* file=fopen(filename, "rb");
    if(file==NULL){
        printf("could not load file containing linear code \n");
        return 0;
    }
    fread(&code->length,sizeof(unsigned int),1,file);
    fread(&code->dimension,sizeof(unsigned int),1,file);

    unsigned int numBlocks=bitToBits(code->length);
    RawMatrix gen=create_rawmatrix(code->dimension,code->length);
    code->generatorMatrix=gen;
    for(unsigned int i=0;i<code->dimension;++i){
        fread(code->generatorMatrix.rows[i],sizeof(Bits),numBlocks,file);
    }
    RawMatrix parity=create_rawmatrix(code->length-code->dimension,code->length);
    code->parityMatrix=parity;
    for(unsigned int i=0;i<code->length-code->dimension;++i){
        fread(code->parityMatrix.rows[i],sizeof(Bits),numBlocks,file);
    }

    fclose(file);
}

int load_qc_ldpc_code_from_file(QC_LDPC* C,char* filename){
    FILE* file=fopen(filename, "rb"); 

    if(file==NULL){
        printf("could not open file to store qc ldpc linear code\n");
        return 0; 
    }
    fread(&C->code.length,sizeof(unsigned int),1,file);
    fread(&C->code.dimension,sizeof(unsigned int),1,file);

    unsigned int numBlocks=bitToBits(C->code.length);
    RawMatrix gen=create_rawmatrix(C->code.dimension,C->code.length);
    C->code.generatorMatrix=gen;
    for(unsigned int i=0;i<C->code.dimension;++i){
        fread(C->code.generatorMatrix.rows[i],sizeof(Bits),numBlocks,file);
    }
    RawMatrix parity=create_rawmatrix(C->code.length-C->code.dimension,C->code.length);
    C->code.parityMatrix=parity;
    for(unsigned int i=0;i<C->code.length-C->code.dimension;++i){
        fread(C->code.parityMatrix.rows[i],sizeof(Bits),numBlocks,file);
    }
    fread(&C->blockWidth,sizeof(unsigned int),1,file);

    fclose(file);
}

Bits** load_received_code_words_from_file(char* filename,unsigned int* n,unsigned int* k,unsigned int* nbWords,double* errorRate){

    FILE* file=fopen(filename,"rb");
    if(file==NULL){
        printf("could not open file to load code words\n");
        *errorRate=0;
        return NULL;
    }

    fread(n,sizeof(unsigned int),1,file);
    fread(nbWords,sizeof(unsigned int),1,file);
    Bits** rec=calloc(*nbWords,sizeof(Bits*));
    if(rec==NULL){
        printf("could not allocate memory to store array of code words\n");
        *errorRate=0;
        fclose(file);
        return NULL;
    }
    unsigned int numBitsBlock=bitToBits(*n);
    for(unsigned int k=0;k<*nbWords;++k){
        Bits* vecData=calloc(numBitsBlock,sizeof(Bits));
        if(vecData==NULL){
            printf("could not allocate memory to store a vector\n");
            *errorRate=0;
            fclose(file);
            return NULL;
        }
        fread(vecData,sizeof(Bits),numBitsBlock,file);
        rec[k]=vecData;
    }
    fread(k,sizeof(unsigned int),1,file);
    if(errorRate!=NULL){
        fread(errorRate,sizeof(double),1,file);
    }

    fclose(file);

    return rec;
}

void orthogonalize(LinearCode* code){
    RawMatrix dual=linearcode_get_parity_check_matrix(*code);
    code->parityMatrix=code->generatorMatrix;
    code->generatorMatrix=dual;
    code->dimension=code->length-code->dimension;
}