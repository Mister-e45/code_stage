#include "matrix.h"




Bits** copy_adresses(Bits** addresses,unsigned size){
    Bits** copy=calloc(size,sizeof(unsigned int));
    if(copy==NULL){
        printf("could not allocate memory to copy addresses of rows or columns\n");
        return copy;
    }
    else{
        for(unsigned int k=0;k<size;++k){
            copy[k]=addresses[k];
        }
        return copy;
    }
}

Matrix create_matrix(unsigned int nrows,unsigned int ncols){
    Matrix mat;
    mat.matrix=create_rawmatrix(nrows,ncols);
    mat.sigma_col=create_permutation(mat.matrix.ncols);
    return mat;
}

Matrix matrix_copy(Matrix mat){
    Matrix copy;
    copy.matrix=rawmatrix_copy(mat.matrix);
    copy.sigma_col=permutation_copy(mat.sigma_col);
    return copy;
}

void destroy_matrix(Matrix mat){
    destroy_rawmatrix(mat.matrix);
    destroy_permutation(mat.sigma_col);
}

Matrix random_matrix(unsigned int nrows,unsigned int ncols){
    Matrix mat;
    mat.matrix=random_rawmatrix(nrows,ncols);
    mat.sigma_col=create_permutation(mat.matrix.ncols);
    return mat;
}

Matrix identity_matrix(unsigned int n){
    Matrix Id_n;
    Id_n.matrix=identity_rawmatrix(n);
    return Id_n;
}


Bit matrix_get_coordinate_value(Matrix mat,unsigned int i,unsigned int j){
    return rawmatrix_get_value(mat.matrix,i,mat.sigma_col.permutation[j]);
}

Vector matrix_get_row_copy(Matrix mat,unsigned int i){
    Vector row_copy=rawmatrix_get_row_copy(mat.matrix,i);
    permute_vector(mat.sigma_col,&row_copy);
    return row_copy;
}

Vector matrix_get_column_copy(Matrix mat,unsigned int j){
    Vector column_copy=rawmatrix_get_column_copy(mat.matrix,mat.sigma_col.permutation[j]);
    return column_copy;
}


Matrix transposed_matrix(Matrix mat){
    RawMatrix result=transposed_rawmatrix(mat.matrix);
    rawmatrix_apply_permutation_to_rows_addresses(&result,mat.sigma_col);
    Matrix out;
    out.matrix=result;
    out.sigma_col=create_permutation(mat.matrix.nrows);
    return out;
}


void matrix_swap_rows(Matrix mat,unsigned int i,unsigned int j){
    Bits* temp=mat.matrix.rows[i];
    mat.matrix.rows[i]=mat.matrix.rows[j];
    mat.matrix.rows[j]=temp; 
}

void matrix_swap_columns(Matrix mat,unsigned int i,unsigned int j){
    apply_transposition_to_permutation(mat.sigma_col,i,j);
}


void matrix_apply_permutation_to_rows(Matrix* mat,Permutation sigma){
    rawmatrix_apply_permutation_to_rows_addresses(&(mat->matrix),sigma);

}

void matrix_apply_permutation_to_columns(Matrix* mat,Permutation sigma){
    Permutation newSigmaColumn=permutation_composition(sigma,mat->sigma_col);
    destroy_permutation(mat->sigma_col);
    mat->sigma_col=newSigmaColumn;
    mat->sigma_col.size=mat->matrix.ncols;
}

Vector matrix_rows_sum_from_positions(Matrix mat,unsigned int* rowIndexes,unsigned int sumSize){
    Vector rowSum=rawmatrix_sum_rows_from_positions(mat.matrix,rowIndexes,sumSize);
    return rowSum;
}

Vector matrix_sum_columns_from_positions(Matrix mat,unsigned int* columnIndexes,unsigned int sumSize){
    Vector colSum=create_vector(mat.matrix.nrows);
    for(unsigned int k=0;k<sumSize;++k){
        Vector column=matrix_get_column_copy(mat,mat.sigma_col.permutation[columnIndexes[k]]);
        vector_add(column,colSum,colSum);
        destroy_vector(column);
    }
    return colSum;
}

Vector matrix_sum_rows_from_vector(Matrix mat,Vector support){
    Vector sum=create_vector(mat.matrix.ncols);
    sum_rows_from_vector(mat.matrix,support,sum);
    Vector rowSum=permuted_vector(mat.sigma_col,sum);
    destroy_vector(sum);
    return rowSum;
}

Vector matrix_sum_columns_from_vector(Matrix mat,Vector support){
    Vector sum=create_vector(mat.matrix.nrows);
    for(unsigned int k=0;k<support.size;++k){
        if(get_coordinate(support,k)){
            Vector col= matrix_get_column_copy(mat,mat.sigma_col.permutation[k]);
            vector_add(col,sum,sum);
            destroy_vector(col);
        }
    }
    return sum;
}


void matrix_write_row(Matrix mat,unsigned int targetRow,Vector val){ // val is bound to mat
    free(mat.matrix.rows[targetRow]);
    mat.matrix.rows[targetRow]=val.bits;
}

void matrix_write_column(Matrix mat,int targetColumn,Vector val){
    rawmatrix_write_column(mat.matrix,mat.sigma_col.permutation[targetColumn],val);
}

void matrix_apply_sumRow_to_row(Matrix mat,unsigned int targetRow,unsigned int* rowSumPositions,unsigned int sumSize){
    rawmatrix_apply_sumRow_to_row(mat.matrix,targetRow,rowSumPositions,sumSize);
    Vector row=rawmatrix_get_row_vector(mat.matrix,targetRow);
}

void matrix_add_row_to_row(Matrix mat,unsigned int target,unsigned int rowToAdd){
    rawmatrix_add_row_to_row(mat.matrix,target,rowToAdd);
}

void matrix_write(Matrix mat,unsigned int i,unsigned int j,Bit val){
    rawmatrix_write(mat.matrix,i,mat.sigma_col.permutation[j],val);
}

void print_matrix(Matrix mat){ // printing a Matrix is not very efficient..
    for(unsigned int i=0;i<mat.matrix.nrows;++i){
        printf("[");
        for(unsigned int j=0;j<mat.matrix.ncols-1;++j){
            if(matrix_get_coordinate_value(mat,i,j)){
                printf("1,");
            }
            else{
                printf("0,");
            }
        }
        if(matrix_get_coordinate_value(mat,i,mat.matrix.ncols-1)){
            printf("1]");
        }
        else{
            printf("0]");
        }
        printf("\n");
    }
    printf("\n");
}

void matrix_print_rawmatrix(Matrix mat){ //faster but only accurate if no column (or row permutation then transposition) has been done
    print_rawmatrix(mat.matrix);
}

int matrix_row_is_zero(Matrix mat,unsigned int index){
    return rawmatrix_row_is_zero(mat.matrix,index);
}

int matrix_column_is_zero(Matrix mat,unsigned int index){
    unsigned int block=mat.sigma_col.permutation[index]/Bits_size;
    unsigned int position=mat.sigma_col.permutation[index]%Bits_size;
    Bits mask=bitmasks[position];
    for(unsigned int k=0;k<mat.matrix.nrows;++k){
        if(mat.matrix.rows[block][position] & mask){
            return 0;
        }
    }
    return 1;
}


void matrix_nullify_under_row(Matrix mat,unsigned int columnIndex,unsigned int pivotRow){
    Bits mask=bitmasks[columnIndex%(Bits_size)];
    unsigned int blockIndex=columnIndex/(Bits_size);
    for(unsigned int i=pivotRow+1;i<mat.matrix.nrows;++i){ // make rows under pivot equal 0
        if(mask & mat.matrix.rows[i][blockIndex]){
            rawmatrix_add_row_to_row(mat.matrix,i,pivotRow);
        }
    }
}

unsigned int matrix_find_one_under_row_in_column(Matrix mat,unsigned int columnIndex,unsigned int rowIndex,int* isFound){
    columnIndex= mat.sigma_col.permutation[columnIndex];
    unsigned int blockIndex= columnIndex/(Bits_size);
    unsigned int posInBlock=columnIndex%(Bits_size);
    Bits mask=bitmasks[posInBlock];

    for(unsigned int k=rowIndex;k<mat.matrix.nrows;++k){
        if(mask & mat.matrix.rows[k][blockIndex]){
            *isFound=1;
            return k;
        }
    }
    *isFound=0; //means no pivot
    return 0; 
}

void matrix_reduce_column(Matrix mat,unsigned int columnIndex,unsigned int pivotRow){
    columnIndex=mat.sigma_col.permutation[columnIndex];
    Bits mask=bitmasks[columnIndex%(Bits_size)];
    unsigned int blockIndex=columnIndex/(Bits_size);
    for(unsigned int i=pivotRow+1;i<mat.matrix.nrows;++i){ // make rows under pivot equal 0
        if(mask & mat.matrix.rows[i][blockIndex]){
            rawmatrix_add_row_to_row(mat.matrix,i,pivotRow);
        }
    }
    for(unsigned int i=pivotRow; i>0;--i){ // make rows above pivot equal 0
        if(mask & mat.matrix.rows[i-1][blockIndex]){
            rawmatrix_add_row_to_row(mat.matrix,i-1,pivotRow);
        }
    }
}

unsigned int row_echelonize_matrix(Matrix mat){ // returns the index of the last non null row
    unsigned int lastNonZeroRow=0;
    unsigned int t= mat.matrix.ncols>mat.matrix.nrows? mat.matrix.nrows:mat.matrix.ncols;
    unsigned int count=0;
    unsigned int pivotRowIndex=count;
    unsigned int pivotColumnIndex=count;
    int finished=0;
    while(!finished){
        int foundPivot=0;
        pivotRowIndex=matrix_find_one_under_row_in_column(mat,pivotColumnIndex,count,&foundPivot);
        while(!foundPivot && pivotColumnIndex<mat.matrix.ncols){
            ++pivotColumnIndex;
            pivotRowIndex=matrix_find_one_under_row_in_column(mat,pivotColumnIndex,count,&foundPivot);
        }
        if(!foundPivot){
            finished=1;
            --count;
        }
        else{
            if(pivotRowIndex!=count){// we need to swap rows
                matrix_swap_rows(mat,count,pivotRowIndex);
                pivotRowIndex=count;
            }
            matrix_nullify_under_row(mat,pivotColumnIndex,pivotRowIndex);
        }
        if(count>=t-1 || finished){
            return count;
        }
        ++count;
        ++pivotColumnIndex;
    }
    return count;
}

unsigned int row_reduce_matrix(Matrix mat){ // returns the index of the last non null row
    unsigned int lastNonZeroRow=0;
    const unsigned int t= mat.matrix.ncols>mat.matrix.nrows? mat.matrix.nrows:mat.matrix.ncols;
    unsigned int count=0;
    unsigned int pivotRowIndex=count;
    unsigned int pivotColumnIndex=count;
    int finished=0;
    while(!finished){
        int foundPivot=0;
        pivotRowIndex=matrix_find_one_under_row_in_column(mat,pivotColumnIndex,count,&foundPivot);
        while(!foundPivot && pivotColumnIndex<mat.matrix.ncols){
            ++pivotColumnIndex;
            pivotRowIndex=matrix_find_one_under_row_in_column(mat,pivotColumnIndex,count,&foundPivot);
        }
        if(!foundPivot){
            finished=1;
            --count;
        }
        else{
            if(pivotRowIndex!=count){// we need to swap rows
                matrix_swap_rows(mat,count,pivotRowIndex);
                pivotRowIndex=count;
            }
            matrix_reduce_column(mat,pivotColumnIndex,pivotRowIndex);
        }
        if(count>=t-1 || finished){
            return count;
        }
        ++count;
        ++pivotColumnIndex;
    }
    return count;
}

unsigned int matrix_find_pivot_in_row(Matrix mat,unsigned int pivotRow,int* isFound){
    unsigned int column=0;
    unsigned int block=0;
    unsigned int position=0;
    Bits mask=0;
    for(unsigned int k=pivotRow;k<mat.matrix.ncols;++k){
        column=mat.sigma_col.permutation[k];
        block=column/Bits_size;
        position=column%Bits_size;
        mask=bitmasks[position];
        if(mat.matrix.rows[pivotRow][block] & mask){
            *isFound=1;
            return k;
        }
    }
    *isFound=0;
    return 0;
}

unsigned int row_reduce_matrix_with_column_permutation(Matrix mat){
    const unsigned int t= mat.matrix.ncols>mat.matrix.nrows? mat.matrix.nrows:mat.matrix.ncols;
    unsigned int count=0;
    unsigned int numberOfZeroRows=0;
    unsigned int pivotColumnIndex=count;
    unsigned int pivotRowIndex=count;
    int finished=0;
    while(!finished){
        int found=0;
        pivotColumnIndex=matrix_find_pivot_in_row(mat,pivotRowIndex,&found);

        if(!found){
            pivotColumnIndex=count;
            pivotRowIndex=matrix_find_one_under_row_in_column(mat,pivotColumnIndex,pivotRowIndex,&found);
            if(!found){
                pivotRowIndex=count;
                matrix_swap_rows(mat,pivotRowIndex,mat.matrix.nrows-1-numberOfZeroRows);
                ++numberOfZeroRows;
            }
            else{
                matrix_swap_rows(mat,pivotRowIndex,count);
                pivotRowIndex=count;
            }
        }
        else{
            if(pivotColumnIndex!=count){
                matrix_swap_columns(mat,count,pivotColumnIndex);
                pivotColumnIndex=count;
            }
            matrix_reduce_column(mat,count,count);
            if(count>=t){
                finished=1;
                return count;
            }
            else{
                ++count;
                ++pivotColumnIndex;
                ++pivotRowIndex;
            }

        }
        if(mat.matrix.nrows-count<= numberOfZeroRows){
            finished=1;
            return count;
        }
    }
    return count;
}

Vector matrix_left_multiply(Vector v,Matrix mat){
    Vector res=left_multiply(v,mat.matrix);
    permute_vector(mat.sigma_col,&res);
    return res;
}

Vector matrix_right_mutiply(Matrix mat, Vector v){
    Vector res=matrix_sum_columns_from_vector(mat,v);
    return res;
}

Matrix rawmatrix_as_matrix(RawMatrix mat){
    Matrix out;
    out.matrix=mat;
    out.sigma_col=create_permutation(mat.ncols);
    return out;
}

Matrix create_matrix_from_rawmatrix(RawMatrix mat){
    Matrix copy;
    copy.sigma_col=create_permutation(mat.ncols);
    copy.matrix=rawmatrix_copy(mat);
    return copy;
}

RawMatrix create_rawmatrix_from_matrix(Matrix mat){
    RawMatrix out;
    if(mat.sigma_col.permutation==0){
        out=rawmatrix_copy(mat.matrix);
        return out;
    }
    else{
        out=create_empty_rows_matrix(mat.matrix.nrows);
        out.ncols=mat.matrix.ncols;
        for(unsigned k=0;k<out.nrows;++k){
            Vector row=permuted_vector(mat.sigma_col,rawmatrix_get_row_vector(mat.matrix,k));
            rawmatrix_replace_row(out,k,row);
        }
        return out;
    }
}




