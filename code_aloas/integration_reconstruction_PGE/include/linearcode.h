#ifndef LINEARCODE_H
#define LINEARCODE_H

#include "rawmatrix.h"
#include "matrix.h"

//typedef struct QC_LDPC QC_LDPC;
//typedef struct LinearCode LinearCode;


struct LinearCode{
    RawMatrix generatorMatrix;
    RawMatrix parityMatrix;
    unsigned int dimension;
    unsigned int length;
};
typedef struct LinearCode LinearCode;

struct QC_LDPC{
    LinearCode code;
    unsigned int blockWidth;
};

typedef struct QC_LDPC QC_LDPC;


LinearCode create_LinearCode(RawMatrix generator,int isFullRank);
QC_LDPC create_random_qc_ldpc_const_weight(unsigned int n,unsigned int dim,unsigned int circulantWidth,unsigned int weight);


void destroy_LinearCode(LinearCode code);
void destroy_qcldpc(QC_LDPC C);

void orthogonalize(LinearCode* code);

RawMatrix dual_code_generator_matrix(RawMatrix mat);
RawMatrix dual_code_generator_matrix_with_permutation(RawMatrix mat,Permutation sigma);
RawMatrix dual_code_generator_matrix_no_copy(RawMatrix mat);// the function destroys the rows in mat
RawMatrix dual_code_generator_matrix_with_permutation(RawMatrix mat,Permutation sigma);//destroys the rows in mat
RawMatrix dual_code_generator_matrix_with_permutation_no_copy(RawMatrix mat,Permutation sigma);
RawMatrix linearcode_get_parity_check_matrix(LinearCode code);
int vector_is_in_code(LinearCode code,Vector vector);
int vector_is_in_dual_code(LinearCode code,Vector vector);
int vectors_are_in_dual_code(LinearCode code,Vector* vectors,unsigned int numberOfVectors);
int vectors_are_in_code(LinearCode code,Vector* vectors,unsigned int numberOfVectors);
int linearcode_C1_included_in_C2(LinearCode C1,LinearCode C2);
int linearcode_is_equal(LinearCode C1,LinearCode C2);
Vector syndrome(LinearCode code,Vector vector);
unsigned int syndrome_weight(LinearCode code,Vector vector);
Vector linearcode_encode(LinearCode* code,Vector* message);
Bits** linearcode_get_some_noisy_code_words(LinearCode code,unsigned int numberOfWords,double* errorProbability);

int save_linear_code_in_file(LinearCode code,char* filename);
int load_linear_code_from_file(LinearCode* code,char* filename);
int load_qc_ldpc_code_from_file(QC_LDPC* C,char* filename);
Bits** load_received_code_words_from_file(char* filename,unsigned int* n,unsigned int* k,unsigned int* nbWords,double* errorRate);
#endif