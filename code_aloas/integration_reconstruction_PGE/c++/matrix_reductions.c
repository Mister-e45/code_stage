#include "matrix_reductions.h"

unsigned int rawmatrix_find_pivot_row_in_columm_over_row(RawMatrix mat,unsigned int pivotColumn,unsigned int startingRow,int* isFound){
    unsigned int blockIndex= pivotColumn/(sizeof(Bits)*8);
    unsigned int posInBlock=pivotColumn%(sizeof(Bits)*8);
    Bits mask=bitmasks[posInBlock];

    for(unsigned int k=startingRow+1;k>0;--k){
        if(mask & mat.rows[k-1][blockIndex]){
            *isFound=1;
            return k-1;
        }
    }
    *isFound=0; //means no pivot
    return 0; 
}

unsigned int matrix_find_pivot_row_in_columm_over_row(Matrix mat,unsigned int pivotColumn,unsigned int startingRow,int* isFound){
    pivotColumn= mat.sigma_col.permutation[pivotColumn];
    unsigned int blockIndex= pivotColumn/(sizeof(Bits)*8);
    unsigned int posInBlock=pivotColumn%(sizeof(Bits)*8);
    Bits mask=bitmasks[posInBlock];

    for(unsigned int k=startingRow+1;k>0;--k){
        if(mask & mat.matrix.rows[k-1][blockIndex]){
            *isFound=1;
            return k-1;
        }
    }
    *isFound=0; //means no pivot
    return 0; 
}

unsigned int rawmatrix_find_pivot_column_for_left_block_reduce(RawMatrix mat,Permutation currentPermutation,unsigned int pivotRow,unsigned int pivotColumn,int* isFound){
    unsigned int column=0;
    unsigned int block=0;
    unsigned int position=0;
    Bits mask=0;
    for(unsigned int k=pivotRow+pivotColumn;k<mat.ncols+pivotColumn;++k){

        column=currentPermutation.permutation[k%mat.ncols];
        block=column/Bits_size;
        position=column%Bits_size;
        mask=bitmasks[position];
        if(mat.rows[pivotRow][block] & mask){
            *isFound=1;
            return k%mat.ncols;
        } 
    }
    *isFound=0;
    return 0;
}

unsigned int matrix_find_pivot_column_for_left_block_reduce(Matrix mat,unsigned int pivotRow,unsigned int pivotColumn,int* isFound){
    unsigned int column=0;
    unsigned int block=0;
    unsigned int position=0;
    Bits mask=0;
    for(unsigned int k=pivotRow+pivotColumn;k<mat.matrix.ncols+pivotColumn;++k){

        column=mat.sigma_col.permutation[k%mat.matrix.ncols];
        block=column/Bits_size;
        position=column%Bits_size;
        mask=bitmasks[position];
        if(mat.matrix.rows[pivotRow][block] & mask){
            *isFound=1;
            return k%mat.matrix.ncols;
        } 
    }
    *isFound=0;
    return 0;
}


unsigned int left_block_row_reduce_rawmatrix(RawMatrix mat,Permutation initial){
    const unsigned int t= mat.ncols>mat.nrows? mat.nrows:mat.ncols;
    unsigned int count=0;
    unsigned int numberOfZeroRows=0;
    unsigned int pivotColumnIndex;
    unsigned int pivotRowIndex;
    int finished=0;
    while(!finished){
        int found=0;
        pivotColumnIndex=mat.ncols-1-count;
        pivotRowIndex=mat.nrows-1-count;

        pivotRowIndex=rawmatrix_find_pivot_row_in_columm_over_row(mat,initial.permutation[pivotColumnIndex],pivotRowIndex,&found);
        if(!found){

            pivotRowIndex=mat.nrows-1-count;

            pivotColumnIndex=rawmatrix_find_pivot_column_for_left_block_reduce(mat,initial,pivotRowIndex,pivotColumnIndex,&found);
            if(!found){

                rawmatrix_swap_rows(mat,pivotRowIndex,numberOfZeroRows);
                ++numberOfZeroRows;
            }
            else{

                if(pivotColumnIndex!=mat.ncols-1-count){

                    apply_transposition_to_permutation(initial,pivotColumnIndex,mat.ncols-1-count);
                    pivotColumnIndex=mat.ncols-1-count;

                }
            }
        }
        else{

            if(pivotRowIndex!=mat.nrows-1-count){

                rawmatrix_swap_rows(mat,pivotRowIndex,mat.nrows-1-count);

                pivotRowIndex=mat.nrows-1-count;
            }

            rawmatrix_reduce_column(mat,initial.permutation[pivotColumnIndex],pivotRowIndex);

            if(count>=t){
                finished=1;
                return count;
            }
            else{
                ++count;
            }
        }
        if(numberOfZeroRows>=mat.nrows-count){
            finished=1;
            return count;
        }
    }
    return count;
}

unsigned int left_block_row_reduce_matrix(Matrix mat){
    const unsigned int t= mat.matrix.ncols>mat.matrix.nrows? mat.matrix.nrows:mat.matrix.ncols;
    unsigned int count=0;
    unsigned int numberOfZeroRows=0;
    unsigned int pivotColumnIndex;
    unsigned int pivotRowIndex;
    int finished=0;
    while(!finished){
        int found=0;
        pivotColumnIndex=mat.matrix.ncols-1-count;
        pivotRowIndex=mat.matrix.nrows-1-count;
        pivotRowIndex=matrix_find_pivot_row_in_columm_over_row(mat,pivotColumnIndex,pivotRowIndex,&found);
        if(!found){

            pivotRowIndex=mat.matrix.nrows-1-count;

            pivotColumnIndex=matrix_find_pivot_column_for_left_block_reduce(mat,pivotRowIndex,pivotColumnIndex,&found);
            if(!found){
                matrix_swap_rows(mat,pivotRowIndex,numberOfZeroRows);
                ++numberOfZeroRows;
            }
            else{

                if(pivotColumnIndex!=mat.matrix.ncols-1-count){
                    matrix_swap_columns(mat,pivotColumnIndex,mat.matrix.ncols-1-count);
                    pivotColumnIndex=mat.matrix.ncols-1-count;

                }
            }
        }
        else{

            if(pivotRowIndex!=mat.matrix.nrows-1-count){
                matrix_swap_rows(mat,pivotRowIndex,mat.matrix.nrows-1-count);
                pivotRowIndex=mat.matrix.nrows-1-count;
            }

            matrix_reduce_column(mat,pivotColumnIndex,pivotRowIndex);

            if(count>=t){
                finished=1;
                return count;
            }
            else{
                ++count;
            }
        }
        if(numberOfZeroRows>=mat.matrix.nrows-count){
            finished=1;
            return count;
        }
    }
    return count;
}


Vector rawmatrix_row_PGE(RawMatrix mat,unsigned int t,Permutation col_sigma){
    Vector noPivotBinaryVector=create_vector(t); // vector entry is zero iff a pivot on the coresponding row is found
    unsigned int count=0;
    unsigned int pivotRowIndex=count;
    unsigned int pivotColumnIndex=count;
    int finished=0;
    while(!finished){
        int foundPivot=0;
        pivotRowIndex=rawmatrix_find_one_under_row_in_column(mat,col_sigma.permutation[pivotColumnIndex],count,&foundPivot);
        if(!foundPivot){
            put_coordinate(noPivotBinaryVector,pivotColumnIndex,1);
        }
        else{
            if(pivotRowIndex!=count){// we need to swap rows
                rawmatrix_swap_rows(mat,count,pivotRowIndex);
                pivotRowIndex=count;
            }
            rawmatrix_reduce_column(mat,col_sigma.permutation[pivotColumnIndex],pivotRowIndex);
        }
        if(count>=t-1 || finished){
            return noPivotBinaryVector;
        }
        ++count;
        ++pivotColumnIndex;
    }
    return noPivotBinaryVector;

}


Vector matrix_row_PGE(Matrix mat,unsigned int t){
    Vector noPivotBinaryVector=create_vector(t); // vector entry is zero iff a pivot on the coresponding row is found
    unsigned int count=0;
    unsigned int pivotRowIndex=count;
    unsigned int pivotColumnIndex=count;
    int finished=0;
    while(!finished){
        int foundPivot=0;
        pivotRowIndex=matrix_find_one_under_row_in_column(mat,pivotColumnIndex,count,&foundPivot);
        if(!foundPivot){
            put_coordinate(noPivotBinaryVector,pivotColumnIndex,1);
        }
        else{
            if(pivotRowIndex!=count){// we need to swap rows
                matrix_swap_rows(mat,count,pivotRowIndex);
                pivotRowIndex=count;
            }
            matrix_reduce_column(mat,pivotColumnIndex,pivotRowIndex);
        }
        if(count>=t-1 || finished){
            return noPivotBinaryVector;
        }
        ++count;
        ++pivotColumnIndex;
    }
    return noPivotBinaryVector;

}


