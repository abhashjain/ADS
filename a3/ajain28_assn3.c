/*Abhash Jain
CSC541- Assignment-3 Disk Based Merge Sort*/

#include<stdio.h>
#include<sys/time.h>
#include<stdlib.h>
#include<string.h>
#define BUCKET 1000
#define NAME_SIZE 14

int comp_a(const void *a, const void *b){
     if(((int*)a) < ((int*)b))
        return -1;
     else if(((int*)a) > ((int*)b))
        return 1;
     return 0;
}

int basicSortFile(FILE *finput, char **flist,int size){
	int temp[BUCKET];
	int i=0;
	fseek(finput,0,SEEK_SET);
	for(;i<size/1000;i++){
		char *file_name = (char*)malloc(sizeof(char)*NAME_SIZE);
		sprintf(file_name,"input.bin.%03d",i);
		fread(temp,sizeof(int),BUCKET,finput);
		qsort(temp,BUCKET,sizeof(int),comp_a);
		FILE *ftemp = fopen(file_name,"wb");
		fseek(ftemp,0,SEEK_SET);
		fwrite(temp,sizeof(int),BUCKET,ftemp);
		memcpy(flist[i],file_name,sizeof(char)*NAME_SIZE);
		//flist[i]=file_name;
		fclose(ftemp);
	}
return i;
}

int main(int argc,char *argv[]){
	if(argc==4){
		char *mode = argv[1];
		char *input_file = argv[2];
		char *output_file = argv[3];
		FILE *finput = fopen(input_file,"rb");
		FILE *foutput = fopen(output_file,"wb");
		char **flist;
		if(finput==NULL){
			printf("Input files doesn't exist\n");
		}
		if(strcmp(mode,"--basic")==0){
			//read the data from input and sort the result and store in sub-file
			//get the input file size
			fseek(finput,0,SEEK_END);
			int size = ftell(finput);
			size = size/sizeof(int);
			printf("number of key %d\n",size);
			basicSortFile(finput,flist,size);
		} else if(strcmp(mode,"--multistep")==0){
		
		} else if(strcmp(mode,"--replacement")==0){
		
		} else {
			printf("Unknown Option\n");	
		}
		fclose(finput);
		fclose(foutput);
	} else {
		printf("Usage:<exe> <merge-mode> <input> <output>\n");
	}	
	return 0;
	
}
