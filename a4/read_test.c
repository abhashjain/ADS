#include<stdio.h>
#include<stdlib.h>


int main(int argc, char *argv[]){
	int t[] ={12,4,5,1,2,10,7};
	int n=7;
	int i=0;
	int temp[100];
	if(argc!=3){
		char *file_name = argv[1];
		FILE *fp = fopen(file_name,"rb+");
		if(fp!=NULL){
			fseek(fp,0,SEEK_SET);
			fread(temp,sizeof(int),n,fp);
			printf("First read\n");
			for(int k=0;k<n;k++){
				printf("%d\t",temp[k]);
			}
			printf("\ntry to write in read mode\n");
			fseek(fp,0,SEEK_END);
			fwrite(t,sizeof(int),n,fp);
			printf("Read now\n");
			fseek(fp,0,SEEK_SET);
			fread(temp,sizeof(int),2*n,fp);
			for(int k=0;k<2*n;k++){
				printf("%d\t",temp[k]);
			}
			
		} else {
			fp = fopen(file_name,"wb+");
			fseek(fp,0,SEEK_SET);
            fwrite(t,sizeof(int),n,fp); 
			printf("\n write done in write mode\n");
			fseek(fp,0,SEEK_SET);
            fread(temp,sizeof(int),n,fp);
            printf(" read in write mode\n");
            for(int k=0;k<n;k++){
                printf("%d\t",temp[k]);
            }
		}
	} else {
		printf("Usage: <exe> <file>");
	}
	return 0;
}
