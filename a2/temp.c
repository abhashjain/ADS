#include <stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct {
    int key;
    long off;
} index_S;

//Dynamically Growing index List
typedef struct {
    int size;
    int used;
    index_S *index_arr;
} index_list;

void printIndexList (index_list *i){
    printf("size is %d and used is %d\n",i->size,i->used);
    for(int j=0;j< i->used;j++){
        printf("Key is %d and offset is %ld\n",i->index_arr[j].key,i->index_arr[j].off);
    }
}

int main(){
	FILE *findex = fopen("index.bin","rb");
	int used ,size;
	index_list index;
	fread(&size,sizeof(int),1,findex);
    fseek(findex,sizeof(int),SEEK_SET);
    fread(&used,sizeof(int),1,findex);
    fseek(findex,2*sizeof(int),SEEK_SET);
    index_S * tempIndex = (index_S *)malloc(sizeof(index_S)*size);
            memset(tempIndex,0,sizeof(index_S)*size);
            fread(tempIndex,sizeof(index_S),size,findex);
            index.size =size;
            index.used =used;
            index.index_arr = tempIndex;
            printIndexList(&index);
	fclose(findex);
	return 0;
}
