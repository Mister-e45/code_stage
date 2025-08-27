#include "rawmatrix.h"

RawMatrix create_rawmatrix(unsigned int nrows,unsigned int ncols){
    RawMatrix mat;
    mat.nrows=nrows;
    mat.ncols=ncols;
    mat.rows=calloc(mat.nrows,sizeof(Bits*));
    if(mat.rows==NULL){
        printf("could not create RawMatrix");
        return mat;
    }
    for(unsigned int i=0;i<mat.nrows;++i){
        Vector row=create_vector(mat.ncols);
        mat.rows[i]=row.bits;
    }
    return mat;
}

RawMatrix create_empty_rows_matrix(unsigned int nrows){
    RawMatrix mat;
    mat.nrows=nrows;
    mat.ncols=0;
    mat.rows=calloc(nrows,sizeof(Bits*));
    if(mat.rows==0){
        printf("could not create empty rows matrix\n");
    }
    return mat;
}

RawMatrix create_rawmatrix_from_bits_array(Bits** vectors,unsigned int numWords,unsigned int n){
    RawMatrix mat=create_empty_rows_matrix(numWords);
    for(unsigned int k=0;k<numWords;++k){
        Vector row= bits_to_vector(vectors[k],n);
        rawmatrix_replace_row(mat,k,row);
    }
    return mat;
}


RawMatrix random_rawmatrix(unsigned int nrows,unsigned int ncols){
    Bits** array=calloc(nrows,sizeof(Bits*));
    if(array==NULL){
        RawMatrix mat;
        printf("could not allocate memory to create random rawmatrix\n");
        mat.ncols=0;
        mat.nrows=0;
        mat.rows=0;
        return mat;
    }
    else{
        RawMatrix mat;
        mat.nrows=nrows;
        mat.ncols=ncols;
        for(unsigned int i=0;i<nrows;++i){
            Vector randomVector=random_vector(ncols);
            array[i]=randomVector.bits;
        }
        mat.rows=array;
        return mat;
    }
    
}

void destroy_rawmatrix(RawMatrix mat){
    for(unsigned int i=0;i<mat.nrows;++i){
        free(mat.rows[i]);
        mat.rows[i]=0;
    }
    free(mat.rows);
}

void destroy_rawmatrix_rows(RawMatrix mat){
    for(unsigned int k=0;k<mat.nrows;++k){
        free(mat.rows[k]);
        mat.rows[k]=0;
    }
}

Vector rawmatrix_get_row_vector(RawMatrix mat,unsigned int i){
    if(i>=mat.nrows){
        printf("in rawmatrix_get_row_vector requested line out of range: RawMatrix shape is (%i,%i) and requested line is %i\n",mat.nrows,mat.ncols,i);
        return create_vector(mat.ncols);
    }
    Vector row;
    row.size=mat.ncols;
    row.bits=mat.rows[i];
    return row;
}

Bit rawmatrix_get_value(RawMatrix mat,unsigned int i,unsigned int j){
    Vector row=rawmatrix_get_row_vector(mat,i);
    return get_coordinate(row,j);
}



Vector rawmatrix_get_row_copy(RawMatrix mat,unsigned int i){
    if(i>=mat.nrows){
        printf("requested line out of range: RawMatrix shape is (%i,%i) and requested line is %i\n",mat.nrows,mat.ncols,i);
        return create_vector(mat.ncols);
    }
    return vector_copy(rawmatrix_get_row_vector(mat,i));
} 

Vector rawmatrix_get_column_copy(RawMatrix mat,unsigned int j){
    Vector column=create_vector(mat.nrows);
    for(unsigned int i=0;i<mat.nrows;++i){
        put_coordinate(column,i,rawmatrix_get_value(mat,i,j));
    }
    return column;
}

void rawmatrix_write(RawMatrix mat,unsigned int i,unsigned int j,Bit val){
    if(i>=mat.nrows || j>=mat.ncols){
        printf("can not write in RawMatrix because coordinate are not in range: RawMatrix of shape (%i,%i) and write attemps in (%i,%i)\n",mat.nrows,mat.ncols,i,j);
    }
    else{
        Vector line;
        line.size=mat.ncols;
        line.bits=mat.rows[i];
        put_coordinate(line,j,val);
    }
}

RawMatrix identity_rawmatrix(unsigned int n){
    RawMatrix Id_n=create_rawmatrix(n,n);
    for(unsigned int i=0;i<n;++i){
        rawmatrix_write(Id_n,i,i,1);
    }
    return Id_n;
}

void print_rawmatrix_coordinates(RawMatrix mat){
    for(unsigned int i=0;i<mat.nrows;++i){
        Vector row;
        row.size=mat.ncols;
        row.bits= mat.rows[i];
        printf("[");
        print_coordinates(row);
        printf("]");
        printf("\n");
    }
    printf("\n");
}

void print_rawmatrix(RawMatrix mat){
    for(unsigned int i=0;i<mat.nrows;++i){
        Vector row;
        row.size=mat.ncols;
        row.bits= mat.rows[i];
        print_vector(row);
    }
    printf("\n");
}

void rows_sum(RawMatrix mat,unsigned int* rows,unsigned int sumSize,Vector sum){
    unsigned int numBlocks=bitToBits(mat.ncols);
    for(unsigned int k=0;k<numBlocks;++k){
        sum.bits[k]=0;
        for(unsigned int i=0;i<sumSize;++i){
            sum.bits[k] ^= mat.rows[rows[i]][k];
        }
    }
#ifdef COMPLEXITY
    increment_complexity((sumSize+1)*mat.ncols);
#endif
}

void sum_rows_from_vector(RawMatrix mat,Vector v,Vector result){
    for(unsigned int i=0;i<v.size;++i){
        if(get_coordinate(v,i)){
            Vector row=rawmatrix_get_row_vector(mat,i); // not a copy of the row of mat!
            vector_add(result,row,result); // result=result+row
        }
    }
}

Vector rawmatrix_sum_rows_from_positions(RawMatrix mat,unsigned int* positions,unsigned int sumSize){
    Vector result=create_vector(mat.ncols);
    Vector row;
    row.size=mat.ncols;
    for(unsigned int i=0;i<sumSize;++i){
        row.bits=mat.rows[positions[i]];
        vector_add(result,row,result);
    }
    return result;
}


Vector left_multiply(Vector v,RawMatrix mat){  // returns v*mat
    Vector result=create_vector(mat.ncols);
    if(v.size != mat.nrows){
        printf("cannot left-multiply %i size vector with RawMatrix of shape (%i,%i\n",v.size,mat.nrows,mat.ncols);
        return result;
    }
    else{
        Vector rowSum=create_vector(mat.ncols);
        sum_rows_from_vector(mat,v,result);
        destroy_vector(rowSum);
    }
    return result;
}

Vector right_multiply(RawMatrix mat,Vector v){
    Vector result=create_vector(mat.nrows);
    if(v.size != mat.ncols){
        printf("cannot right-multiply %i size vector with RawMatrix of shape (%i,%i)\n",v.size,mat.nrows,mat.ncols);
        return result;
    }
    else{
        for(unsigned int k=0;k<v.size;++k){
            if(get_coordinate(v,k)){
                Vector column=rawmatrix_get_column_copy(mat,k);
                vector_add(result,column,result);
                destroy_vector(column);
            }
        }
    }
    return result;
}

void rawmatrix_write_row(RawMatrix mat,unsigned int i,Vector v){
    Vector new_row=vector_copy(v);
    free(mat.rows[i]);
    mat.rows[i]=new_row.bits;
}

void rawmatrix_replace_row(RawMatrix mat,unsigned int i,Vector v){
    free(mat.rows[i]);
    mat.rows[i]=v.bits;
}

void rawmatrix_write_column(RawMatrix mat,unsigned int j,Vector v){
    unsigned int block=j/Bits_size;
    unsigned int positionInBlock=j%Bits_size;
    Bits mask=bitmasks[positionInBlock];
    for(unsigned int i=0;i<mat.nrows;++i){
        if(get_coordinate(v,i)){
            mat.rows[i][block] |= mask;
        }
        else{
            mat.rows[i][block] &= (~mask);
        }
    }
}

RawMatrix rawmatrix_from_vector_array(Vector* array,unsigned int nrows){
    RawMatrix mat;
    mat.ncols=0;
    mat.nrows=0;
    mat.rows=0;
    if(nrows!=0){
        RawMatrix mat=create_rawmatrix(nrows,array[0].size);
        for(unsigned int i=0;i<nrows;++i){
            rawmatrix_write_row(mat,i,array[i]);
        }
        return mat;
    }
    else{
        return mat;
    }
}

RawMatrix rawmatrix_from_bits_array(Bits** array,unsigned int nrows,unsigned int ncols){
    RawMatrix mat;
    mat.ncols=0;
    mat.nrows=0;
    mat.rows=0;
    if(nrows!=0){
        mat.rows=calloc(nrows,sizeof(Bits*));
        if(mat.rows==0){
            printf("could not create rawmatrix to make rawmatrix from bits array\n");
            return mat;
        }
        mat.ncols=ncols;
        mat.nrows=nrows;
        for(unsigned int i=0;i<nrows;++i){
            Vector row=vector_from_bits_array(array[i],ncols);
            mat.rows[i]=row.bits;
        }
        return mat;
    }
    else{
        return mat;
    }
}

RawMatrix transposed_rawmatrix(RawMatrix mat){
    RawMatrix t_mat=create_rawmatrix(mat.ncols,mat.nrows);
    Bits** bits=calloc(t_mat.nrows,sizeof(Bits*));
    if(bits==NULL){
        printf("error: could not allocate memory for transposed RawMatrix");
        t_mat.ncols=0;
        t_mat.nrows=0;
        return t_mat;
    }
        
    for(unsigned int i=0;i<mat.ncols;++i){
        Vector column=rawmatrix_get_column_copy(mat,i);
        t_mat.rows[i]=column.bits;
    }
    return t_mat;
}


void rawmatrix_add_row_to_row(RawMatrix mat,unsigned int target,unsigned int rowToAdd){
    unsigned int numBlocks=bitToBits(mat.ncols);
    for(unsigned int k=0;k<numBlocks;++k){
        mat.rows[target][k]^=mat.rows[rowToAdd][k];
    }
#ifdef COMPLEXITY
    increment_complexity(mat.ncols);
#endif
}

void rawmatrix_apply_sumRow_to_row(RawMatrix mat,unsigned int targetRow,unsigned int* rowSumPositions,unsigned int sumSize){
    Vector row;
    row.size=mat.ncols;
    Vector selectedRow;
    selectedRow.size=mat.ncols;
    for(unsigned int i=0;i<sumSize;++i){
        row.bits=mat.rows[targetRow];
        selectedRow.bits=mat.rows[rowSumPositions[i]];
        vector_add(row,selectedRow,row);
    }
}

void rawmatrix_swap_rows(RawMatrix mat,unsigned int i,unsigned int j){
    Bits* temp=mat.rows[i];
    mat.rows[i]=mat.rows[j];
    mat.rows[j]=temp;
}

void rawmatrix_apply_permutation_to_rows_addresses(RawMatrix* mat,Permutation sigma){
    Bits** newRowSequence=calloc(mat->nrows,sizeof(Bits*));
    if(newRowSequence==NULL){
        printf("could not allocate memory to create new permuted rows\n");
    }
    else{
        for(unsigned int k=0;k<mat->nrows;++k){
            newRowSequence[k]=mat->rows[sigma.permutation[k]];
        }
        free(mat->rows);
        mat->rows=newRowSequence;
    }
}

int rawmatrix_row_is_zero(RawMatrix mat,unsigned int rowIndex){
    unsigned int numBlocks=bitToBits(mat.ncols);
    for(unsigned int k=0;k<numBlocks;++k){
        if(mat.rows[rowIndex][k]!=0){
            return 0;
        }
    }
    return 1;
}

unsigned int rawmatrix_find_one_under_row_in_column(RawMatrix mat,unsigned int columnIndex,unsigned int rowIndex,int* isFound){
    unsigned int blockIndex= columnIndex/(Bits_size);
    unsigned int posInBlock=columnIndex%(Bits_size);
    Bits mask=bitmasks[posInBlock];

    for(unsigned int k=rowIndex;k<mat.nrows;++k){
        if(mask & mat.rows[k][blockIndex]){
            *isFound=1;
            return k;
        }
    }
    *isFound=0; //means no pivot
    return 0; 
}



void rawmatrix_reduce_column(RawMatrix mat,unsigned int columnIndex,unsigned int pivotRow){
    Bits mask=bitmasks[columnIndex%(Bits_size)];
    unsigned int blockIndex=columnIndex/(Bits_size);
    for(unsigned int i=pivotRow+1;i<mat.nrows;++i){ // make rows under pivot equal 0
        if(mask & mat.rows[i][blockIndex]){
            rawmatrix_add_row_to_row(mat,i,pivotRow);
        }
    }
    for(unsigned int i=pivotRow; i>0;--i){ // make rows above pivot equal 0
        if(mask & mat.rows[i-1][blockIndex]){
            rawmatrix_add_row_to_row(mat,i-1,pivotRow);
        }
    }
}

void rawmatrix_nullify_under_row(RawMatrix mat,unsigned int columnIndex,unsigned int pivotRow){
    Bits mask=bitmasks[columnIndex%(Bits_size)];
    unsigned int blockIndex=columnIndex/(Bits_size);
    for(unsigned int i=pivotRow+1;i<mat.nrows;++i){ // make rows under pivot equal 0
        if(mask & mat.rows[i][blockIndex]){
            rawmatrix_add_row_to_row(mat,i,pivotRow);
        }
    }
}


unsigned int row_echelonize_rawmatrix(RawMatrix mat){ // returns the index of the last non null row
    unsigned int lastNonZeroRow=0;
    unsigned int t= mat.ncols>mat.nrows? mat.nrows:mat.ncols;
    unsigned int count=0;
    unsigned int pivotRowIndex=count;
    unsigned int pivotColumnIndex=count;
    int finished=0;
    while(!finished){
        int foundPivot=0;
        pivotRowIndex=rawmatrix_find_one_under_row_in_column(mat,pivotColumnIndex,count,&foundPivot);
        while(!foundPivot && pivotColumnIndex<mat.ncols){
            ++pivotColumnIndex;
            pivotRowIndex=rawmatrix_find_one_under_row_in_column(mat,pivotColumnIndex,count,&foundPivot);
        }
        if(!foundPivot){
            finished=1;
            --count;
        }
        else{
            if(pivotRowIndex!=count){// we need to swap rows
                rawmatrix_swap_rows(mat,count,pivotRowIndex);
                pivotRowIndex=count;
            }
            rawmatrix_nullify_under_row(mat,pivotColumnIndex,pivotRowIndex);
        }
        if(count>=t-1 || finished){
            return count;
        }
        ++count;
        ++pivotColumnIndex;
    }
    return count;
}

unsigned int row_reduce_rawmatrix(RawMatrix mat){ // returns the index of the last non null row
    const unsigned int t= mat.ncols>mat.nrows? mat.nrows:mat.ncols;
    unsigned int count=0;
    unsigned int pivotRowIndex=count;
    unsigned int pivotColumnIndex=count;
    int finished=0;
    while(!finished){
        int foundPivot=0;
        pivotRowIndex=rawmatrix_find_one_under_row_in_column(mat,pivotColumnIndex,count,&foundPivot);
        while(!foundPivot && pivotColumnIndex<mat.ncols){
            ++pivotColumnIndex;
            pivotRowIndex=rawmatrix_find_one_under_row_in_column(mat,pivotColumnIndex,count,&foundPivot);
        }
        if(!foundPivot){
            finished=1;
            --count;
        }
        else{
            if(pivotRowIndex!=count){// we need to swap rows
                rawmatrix_swap_rows(mat,count,pivotRowIndex);
                pivotRowIndex=count;
            }
            rawmatrix_reduce_column(mat,pivotColumnIndex,pivotRowIndex);
        }
        if(count>=t-1 || finished){
            return count;
        }
        ++count;
        ++pivotColumnIndex;
    }
    return count;

}

unsigned int rawmatrix_find_pivot_in_row(RawMatrix mat,Permutation initialColumnPermutation,unsigned int pivotRow,int* isFound){
    unsigned int column=0;
    unsigned int block=0;
    unsigned int position=0;
    Bits mask=0;
    for(unsigned int k=pivotRow;k<mat.ncols;++k){
        column=initialColumnPermutation.permutation[k];
        block=column/Bits_size;
        position=column%Bits_size;
        mask=bitmasks[position];
        if(mat.rows[pivotRow][block] & mask){
            *isFound=1;
            return k;
        }
    }
    *isFound=0;
    return 0;
}

unsigned int row_reduce_rawmatrix_with_column_permutation(RawMatrix mat,Permutation intitialColumnPermutation){
    const unsigned int t= mat.ncols>mat.nrows? mat.nrows:mat.ncols;
    unsigned int count=0;
    unsigned int numberOfZeroRows=0;
    unsigned int pivotColumnIndex=count;
    unsigned int pivotRowIndex=count;
    int finished=0;
    while(!finished){
        int found=0;
        pivotColumnIndex=rawmatrix_find_pivot_in_row(mat,intitialColumnPermutation,pivotRowIndex,&found);

        if(!found){
            pivotColumnIndex=count;
            pivotRowIndex=rawmatrix_find_one_under_row_in_column(mat,intitialColumnPermutation.permutation[pivotColumnIndex],pivotRowIndex,&found);
            if(!found){
                pivotRowIndex=count;
                rawmatrix_swap_rows(mat,pivotRowIndex,mat.nrows-1-numberOfZeroRows);
                ++numberOfZeroRows;
            }
            else{
                rawmatrix_swap_rows(mat,pivotRowIndex,count);
                pivotRowIndex=count;
            }
        }
        else{
            if(pivotColumnIndex!=count){
                apply_transposition_to_permutation(intitialColumnPermutation,count,pivotColumnIndex);
                pivotColumnIndex=count;
            }
            rawmatrix_reduce_column(mat,intitialColumnPermutation.permutation[count],count);
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
        if(mat.nrows-count<= numberOfZeroRows){
            finished=1;
            return count;
        }
    }
    return count;
}


void print_permuted_rawmatrix(RawMatrix mat,Permutation columnPermutation){
    for(unsigned int k=0;k<mat.nrows;++k){
        Vector row=rawmatrix_get_row_copy(mat,k);
        permute_vector(columnPermutation,&row);
        print_vector(row);
        destroy_vector(row);
    }
}

RawMatrix rawmatrix_copy(RawMatrix mat){
    RawMatrix copy;
    copy.ncols=mat.ncols;
    copy.nrows=mat.nrows;
    copy.rows=calloc(mat.nrows,sizeof(Bits*));
    if(copy.rows==0){
        copy.ncols=0;
        copy.ncols=0;
        return copy;
    }
    for(unsigned int k=0;k<mat.nrows;++k){
        Vector row=rawmatrix_get_row_copy(mat,k);
        copy.rows[k]=row.bits;
    }

    return copy;
}

Vector* rawmatrix_as_vector_array(RawMatrix mat){
    Vector* vectors=calloc(mat.nrows,sizeof(Vector));
    if(vectors==0){
        printf("could not allocate memory to create Vector array from rawmatrix\n");
        return vectors;
    }
    else{
        for(unsigned int i=0;i<mat.nrows;++i){
            Vector v;
            v.size=mat.ncols;
            v.bits=mat.rows[i];
            vectors[i]=v;
        }
        return vectors;
    }
}

