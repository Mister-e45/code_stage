#include "PGE.h"

/*double log2(double x){
    return log(x)/log(2);
}*/

unsigned long int binomial(unsigned int n,unsigned int k){
    if(n==k){
        return 1;
    }
    unsigned int kmax= n-k > k ? n-k : k;
    unsigned long int denominator=1;
    unsigned long int numerator=1;
    unsigned int countDenominator=2;
    for(unsigned int num=kmax+1;num<=n;++num){
        numerator*=num;
        denominator=1;
        for(unsigned int d=countDenominator;d<=n-kmax;++d){
            denominator*=d;
            if(numerator<denominator){
                break;
            }
            if(numerator%denominator==0){
                numerator/=denominator;
                denominator=1;
                countDenominator=d+1;
            }
        }

    }
    return numerator;
}

double log2_binomial(unsigned int n,unsigned int k){
    return log2(binomial(n,k));
}

void init_const_weight_sequence(unsigned int* vector,unsigned int weight){
    for(unsigned int i=0;i<weight;++i){
        vector[i]=i;
    }
}

int is_displacable_one(unsigned int* vector,unsigned int n,unsigned int weight,unsigned int ith_One){//returns -1 if the index is out of range or if the index contains a 0. returns 1 is a 1 has no 1 on it's right, returns 0 otherwise
    if(ith_One<weight-1){
        if(vector[ith_One]< vector[ith_One+1]-1){
            return 1;
        }
    }
    else if(vector[weight-1]<n-1){
        return 1;
    }
    return 0;
}

unsigned int minimal_index_of_displacable_one(unsigned int* vector,unsigned int n,unsigned int weight,int* reachedEnd){
    for(unsigned int i=0;i<weight;++i){
        if(is_displacable_one(vector,n,weight,i)){
            *reachedEnd=0;
            return i;
        }
    }
    *reachedEnd=1;
    return n; // means we have reached the end of the sequence
}


void put_ones_on_the_left(unsigned int* vector,unsigned int numberOfOnes){
    for(unsigned int i=0;i<numberOfOnes;++i){
        vector[i]=i;
    }
}


void next_vector_in_const_weight_sequence(unsigned int* vector,unsigned int n,unsigned int weight,int* reachedEnd){
    unsigned int index=minimal_index_of_displacable_one(vector,n,weight,reachedEnd);
    if(!(*reachedEnd)){
        vector[index]+=1;
        put_ones_on_the_left(vector,index);
    }

}

Vector get_portion_of_column(Matrix mat,unsigned int columnIndex,unsigned int beginingIndex,unsigned int endIndex){
    columnIndex=mat.sigma_col.permutation[columnIndex];
    unsigned int blockIndex=columnIndex/Bits_size;
    unsigned int positionInBlock=columnIndex%Bits_size;
    unsigned int mask= bitmasks[positionInBlock];
    Vector column=create_vector(endIndex-beginingIndex+1);
    for(unsigned int i=beginingIndex;i<endIndex;++i){
        if(mat.matrix.rows[i][blockIndex] & mask){
            put_coordinate(column,i,1);
        }
    }
    return column;
}

void transpose_sub_matrix(Matrix mat,unsigned int startingRow,unsigned int startingCol,RawMatrix* result){ // result should be created with correct dimensions
    for(unsigned int j=startingCol;j<mat.matrix.ncols;++j){
        Vector column=get_portion_of_column(mat,j,startingRow,mat.matrix.nrows-1);
        rawmatrix_replace_row(*result,j-startingCol,column);
    }
}

void unsigned_int_array_free(void* array){
    free((unsigned int*)array);
}

unsigned int parity_check_on_noisy_words(Bits** receivedWords,unsigned int wordsLength,unsigned int arraySize,Vector parityCheckVector){
    unsigned int count=0;
    for(unsigned int k=0;k<arraySize;++k){
        Vector word=bits_to_vector(receivedWords[k],wordsLength);
        if(scalar_product(parityCheckVector,word)){
            ++count;
        }
    }
    return count;
}

double theta(unsigned int numberOfTrials,unsigned int parityVectorWeight,double errorRate){
    return numberOfTrials*(0.5 - pow(1-2*errorRate,parityVectorWeight)/4.0); // average between expected fails if candidate was not a parity check vector and the expected fails if candidate was a parity vector
}

double alpha(double errorRate,unsigned int weight){
    return (1.0-pow(1-2*errorRate,weight) )/2.0;
}

/*int statistical_test_parity_vector(unsigned int numberOfWords,double errorRate,unsigned int numberFailedParityChecks,Vector candidate){
    unsigned int w=HW(candidate);
    double failProbability= parity_check_fail_probability(errorRate,w);
    double threshold= theta(numberOfWords,w,errorRate);
    if(numberFailedParityChecks<= threshold){
        return 1;
    }
    else{
        return 0;
    }

}*/

void destroy_unsigned_int_array_list(void* pointerToList){
    destroy_List((List*)pointerToList,&free);
    free(pointerToList);
}

Vector find_parity_vector_PGE(Bits** receivedWords,unsigned int numberReceived,unsigned int numberWordsToUse,double* errorRate,unsigned int n,unsigned int t,unsigned int p,unsigned int iterationLimit){
    RawMatrix receivedRawMatrix= create_rawmatrix_from_bits_array(receivedWords,numberWordsToUse,n);
    Bits** remainingWords=NULL;
    if(numberWordsToUse<numberReceived){
        remainingWords=&receivedWords[numberWordsToUse];
    }
    unsigned int N=n-t;
    unsigned int M=numberWordsToUse-t;

    unsigned long long int card = binomial(N/2,p/2);
    unsigned long int logCard = log2(card);

    unsigned int l=logCard;
    unsigned int threshold=theta(M,p,*errorRate);
    if(*errorRate==0.0){
        threshold=0;
    }

    if(M<l){ // in this case there are not enough code words to perform a good projection method search
        return create_vector(n);
    }
    Vector output;

    Matrix workMatrix=create_matrix_from_rawmatrix(receivedRawMatrix);
    Permutation invSigmaCol=create_permutation(n);

    List candidates= create_List();


    for(unsigned k=0;k<iterationLimit;++k){

        Vector noPivotColumns= rawmatrix_row_PGE(workMatrix.matrix,t,workMatrix.sigma_col);
        BinaryTree searchTree=create_BinaryTree(l);

        RawMatrix columns=create_empty_rows_matrix(N); // used to store the columns as rows (makes computations faster)
        columns.ncols=M;
        columns.nrows=N;
        transpose_sub_matrix(workMatrix,t,t,&columns);

        unsigned int I[l]; // coordinates to be analysed in projection method
        random_distinct_elements_in_range(l,0,M,I);

        get_inverse_permutation(workMatrix.sigma_col,invSigmaCol);

        for(unsigned int i=0;i<card;++i){
            unsigned int* colCombination=give_random_distinct_integers(p/2,N/2);
            Vector s= rawmatrix_sum_rows_from_positions(columns,colCombination,p/2);
            Vector s_proj=projection(s,I,l);
            List* list=(List*)binarytree_get(searchTree,s_proj);
            if(list==NULL){
                list=(List*)malloc(sizeof(List));
                *list= create_List();
                append_list(list,colCombination);
                binarytree_put_in(searchTree,s_proj,list);
            }
            else if(list->listSize<4){ // 4 is quite arbitrary 
                append_list(list,colCombination);
            }
            else{
                free(colCombination);
            }
            destroy_vector(s);
            destroy_vector(s_proj);
        }
        unsigned int secondPartCombination[p/2];
        for(unsigned int i=0;i<card;++i){
            random_distinct_elements_in_range(p/2,N/2,N,secondPartCombination);
            Vector partialSum2= rawmatrix_sum_rows_from_positions(columns,secondPartCombination,p/2);
            Vector s_proj2=projection(partialSum2,I,l);
            List* list = (List*)binarytree_get(searchTree,s_proj2);
            if(list!=NULL)
            if(list->listSize!=0){
                for(ListElement* element=list->firstElement;element!=NULL;element=next_ListElement(element)){
                    unsigned int* firstPartCombination=(unsigned int*)(element->value);
                    Vector partialSum1= rawmatrix_sum_rows_from_positions(columns,firstPartCombination,p/2);
                    Vector sum=create_vector(M);
                    vector_add(partialSum1,partialSum2,sum);
                    if(HW(sum)<=threshold){
                        Bits* out=create_vector(n).bits;
                        Vector vec=bits_to_vector(out,n);
                        vector_set_positions_to_one(vec,firstPartCombination,p/2,t);
                        vector_set_positions_to_one(vec,secondPartCombination,p/2,t);
                        for(unsigned int h=0;h<t;++h){
                            Vector row=matrix_get_row_copy(workMatrix,h);
                            Bit bit=scalar_product(row,vec);
                            if(bit && !get_coordinate(noPivotColumns,h)){
                                put_coordinate(vec,h,1);
                            }
                            destroy_vector(row);
                        }
                        permute_vector(invSigmaCol,&vec);
                        if(remainingWords!=NULL){
                            if(is_likely_parity(remainingWords,vec,numberReceived-numberWordsToUse,*errorRate)){
                                append_list(&candidates,vec.bits); // we add the vector to the output list
                            }
                            else{
                                destroy_vector(vec);
                            }
                        }
                    }
                    destroy_vector(partialSum1);
                    destroy_vector(sum);
                    if(candidates.listSize>=1){ 
                        break;
                    }
                }
            }
            destroy_vector(partialSum2);
            destroy_vector(s_proj2);
            if(candidates.listSize>=1){
                break;
            }
        }
        
        destroy_rawmatrix(columns);
        destroy_BinaryTree(searchTree,&destroy_unsigned_int_array_list);
        destroy_vector(noPivotColumns);
        if(candidates.listSize>=1){
            break;
        }

        Permutation sigma=random_permutation(n);
        matrix_apply_permutation_to_columns(&workMatrix,sigma);
        destroy_permutation(sigma);
    }
    destroy_matrix(workMatrix);
    free(receivedRawMatrix.rows);
    destroy_permutation(invSigmaCol);
    
    if(candidates.listSize>=1){
        output= vector_copy(bits_to_vector((Bits*)candidates.firstElement->value,n));
    }
    else{
        output=create_vector(n);
    }
    destroy_List(&candidates,free);

    return output;
}

List find_parity_check_vectors_PGE(Bits** receivedWords,unsigned int numberReceived,unsigned int numberOfWordsToUse,double* errorRate,unsigned int n,unsigned int t,unsigned int p,unsigned int collisionListMaxSize,unsigned int numIter,unsigned int maxNumVectorsFind){
    RawMatrix receivedRawMatrix= create_rawmatrix_from_bits_array(receivedWords,numberOfWordsToUse,n);
    Bits** remainingWords=NULL;
    if(numberOfWordsToUse<numberReceived){
        remainingWords=&receivedWords[numberOfWordsToUse];
    }
    unsigned int N=n-t;
    unsigned int M=numberOfWordsToUse-t;

    unsigned long long int card = binomial(N/2,p/2);
    unsigned long int logCard = log2(card);
    unsigned int l=logCard;

    if(M<l){ // in this case there are not enough code words to perform a good projection method search
        return create_List();
    }

    unsigned int threshold=theta(M,p,*errorRate);
    if(*errorRate==0.0){
        threshold=0;
    }

    Matrix workMatrix=create_matrix_from_rawmatrix(receivedRawMatrix);
    Permutation invSigmaCol=create_permutation(n);

    List candidates= create_List();



    for(unsigned k=0;k<numIter;++k){

        Vector noPivotColumns= rawmatrix_row_PGE(workMatrix.matrix,t,workMatrix.sigma_col);
        BinaryTree searchTree=create_BinaryTree(l);

        RawMatrix columns=create_empty_rows_matrix(N); // used to store the columns as rows (makes computations faster)
        columns.ncols=M;
        columns.nrows=N;
        transpose_sub_matrix(workMatrix,t,t,&columns);

        unsigned int I[l]; // coordinates to be analysed in projection method
        random_distinct_elements_in_range(l,0,M,I);

        get_inverse_permutation(workMatrix.sigma_col,invSigmaCol);

        for(unsigned int i=0;i<card;++i){
            unsigned int* colCombination=give_random_distinct_integers(p/2,N/2);
            Vector s= rawmatrix_sum_rows_from_positions(columns,colCombination,p/2);
            Vector s_proj=projection(s,I,l);
            List* list=(List*)binarytree_get(searchTree,s_proj);
            if(list==NULL){
                list=(List*)malloc(sizeof(List));
                *list= create_List();
                append_list(list,colCombination);
                binarytree_put_in(searchTree,s_proj,list);
            }
            else if(list->listSize<collisionListMaxSize){
                append_list(list,colCombination);
            }
            else{
                free(colCombination);
            }
            destroy_vector(s);
            destroy_vector(s_proj);
        }
        unsigned int secondPartCombination[p/2];
        for(unsigned int i=0;i<card;++i){
            random_distinct_elements_in_range(p/2,N/2,N,secondPartCombination);
            Vector partialSum2= rawmatrix_sum_rows_from_positions(columns,secondPartCombination,p/2);
            Vector s_proj2=projection(partialSum2,I,l);
            List* list = (List*)binarytree_get(searchTree,s_proj2);
            if(list!=NULL)
            if(list->listSize!=0){
                for(ListElement* element=list->firstElement;element!=NULL;element=next_ListElement(element)){
                    unsigned int* firstPartCombination=(unsigned int*)(element->value);
                    Vector partialSum1= rawmatrix_sum_rows_from_positions(columns,firstPartCombination,p/2);
                    Vector sum=create_vector(M);
                    vector_add(partialSum1,partialSum2,sum);
                    if(HW(sum)<=threshold){
                        Bits* out=create_vector(n).bits;
                        Vector vec=bits_to_vector(out,n);
                        vector_set_positions_to_one(vec,firstPartCombination,p/2,t);
                        vector_set_positions_to_one(vec,secondPartCombination,p/2,t);
                        for(unsigned int h=0;h<t;++h){
                            Vector row=matrix_get_row_copy(workMatrix,h);
                            Bit bit=scalar_product(row,vec);
                            if(bit && !get_coordinate(noPivotColumns,h)){
                                put_coordinate(vec,h,1);
                            }
                            destroy_vector(row);
                        }
                        permute_vector(invSigmaCol,&vec);
                        if(remainingWords!=NULL){
                            if(is_likely_parity(remainingWords,vec,numberReceived-numberOfWordsToUse,*errorRate)){
                                append_list(&candidates,vec.bits); // we add the vector to the output list
                            }
                            else{
                                destroy_vector(vec);
                            }
                        }
                    }
                    destroy_vector(partialSum1);
                    destroy_vector(sum);
                    if(candidates.listSize>=maxNumVectorsFind){
                        break;
                    }
                }
            }
            destroy_vector(partialSum2);
            destroy_vector(s_proj2);
            if(candidates.listSize>=maxNumVectorsFind){
                break;
            }
        }
        
        destroy_rawmatrix(columns);
        destroy_BinaryTree(searchTree,&destroy_unsigned_int_array_list);
        destroy_vector(noPivotColumns);
        if(candidates.listSize>=maxNumVectorsFind){
            break;
        }

        Permutation sigma=random_permutation(n);
        matrix_apply_permutation_to_columns(&workMatrix,sigma);
        destroy_permutation(sigma);
    }
    destroy_matrix(workMatrix);
    free(receivedRawMatrix.rows);
    destroy_permutation(invSigmaCol);
    
    
    return candidates;
}



RawMatrix QCLDPC_reconstruction_PGE(Bits** receivedWords,unsigned int numberReceived,unsigned int numberOfWordsToUse,double* errorRate,unsigned int n,unsigned int numberOfIndependentVectorToFind,unsigned int circulantBlockSize,unsigned int t,unsigned int p,unsigned int N_iter,unsigned int itLimit){
    int circulantWidth=circulantBlockSize;
    if(circulantBlockSize==0){
        circulantWidth=n;
    }
    unsigned int orbitSize=n/circulantWidth;
    if(n%circulantWidth!=0){
        orbitSize=1;
    }

    unsigned int rank=0;
    unsigned int dualDim=n-numberOfIndependentVectorToFind;
    unsigned int iteration=0;
    RawMatrix matRec=create_rawmatrix_from_bits_array(receivedWords,numberReceived,n);
    RawMatrix out;
    out.ncols=n;
    out.nrows=0;
    out.rows=0;

    while(rank<dualDim && iteration<itLimit){
        ++iteration;
        
        List found=find_parity_check_vectors_PGE(receivedWords,numberReceived,numberOfWordsToUse,errorRate,n,t,p,4,N_iter,circulantWidth);

        append_rows_to_rawmatrix(&out,found);

        List extraVectors =create_List();
        for(unsigned int i=0;i<out.nrows;++i){ // we use the structure of the code to get more parity check vectors
            Vector vec=rawmatrix_get_row_vector(out,i);
            for(unsigned int l=1;l<orbitSize;++l){
                Vector shifted=cyclic_right_shift_vector(vec,l*circulantWidth);
                vec=shifted;
                append_list(&extraVectors,vec.bits);
            }
        }

        append_rows_to_rawmatrix(&out,extraVectors);

        int rank=row_echelonize_rawmatrix(out)+1;

        row_resize_rawmatrix(&out,rank);
        
        destroy_List(&extraVectors,do_nothing);
        destroy_List(&found,do_nothing);
    }
    free(matRec.rows);

    return out;
}
