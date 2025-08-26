#include <stdio.h>
#include <string.h>



int main (int argc,char **argv){
    unsigned char bitmask[8]={0x1,0x2,0x4,0x8,0x10,0x20,0x40,0x80};
    unsigned int bits_block_size=8;
    char* filename;
    char*outFileName;

    if(argc==3){
        filename=argv[1];
        bits_block_size=atoi(argv[2]);
        outFileName=argv[3];
    }
    else if(argv==1){
        printf("./compactor [received words file name] [number of bits in a single bits block] [output file name]\n");
        return 0;
    }
    else{
        printf("%u arguments provided but 3 ae required\n",argc-1);
    }

    FILE* inFile=fopen(filename,"rb");
    if(inFile==NULL){
        printf("could not open input file\n");
        fclose(inFile);
        return 0;
    }

    unsigned int n;
    unsigned int numberWords;
    fread(&n,sizeof(unsigned int),1,inFile);
    fread(&numberWords,sizeof(unsigned int),1,inFile);

    FILE* outFile=fopen(outFileName,"wb");
    if(outFile==NULL){
        printf("could not open output file\n");
        fclose(inFile);
        fclose(outFile);
        return 0;
    }

    unsigned int numBits=numberWords*n;
    unsigned char byte=0;
    unsigned char outByte=0;
    unsigned int virtualCursor=0;

    unsigned int numGarbageBytes= (bits_block_size*(n%bits_block_size==0 ? n/bits_block_size : (n/bits_block_size)+1)) - (8*(n%8==0 ? n/8 : (n/8)+1));

    for(unsigned int i=0;i<numberWords;++i){
        unsigned int coordIndex=0;
        for(unsigned int k=0;k<n;++k){
            if(virtualCursor%8==0){
                fread(&byte,sizeof(unsigned char),1,inFile);
            }
            if(byte & bitmask[coordIndex%8]){
                outByte ^= (1<<(virtualCursor%8));
            }
            ++virtualCursor;
            ++coordIndex;

            if(virtualCursor%8==0){
                fwrite(&outByte,sizeof(unsigned char),1,outFile);
                outByte=0;
            }
        }
        for(unsigned int l=0;l<numGarbageBytes;++l){
            fread(&byte,sizeof(unsigned char),1,inFile);
        }
    }
    if(virtualCursor%8!=0){
        fwrite(&outByte,sizeof(unsigned char),1,outFile);
    }




    fclose(outFile);
    fclose(inFile);
    return 0;
}