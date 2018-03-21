#include<stdio.h>
#include<stdlib.h>
int main(int argc,char *argv[]){
	if(argc==3){
		int t = atoi(argv[2]);
		int *temp = (int*)malloc(sizeof(int)*t);
		FILE *fp = fopen(argv[1],"rb");
		fseek(fp,0,SEEK_SET);
		fread(temp,sizeof(int),t,fp);
		for(int i=0;i<t;i++){
			printf("%d\n",temp[i]);
		}
		printf("\n");
		free(temp);
		} else {
			printf("Usage: <exe> <file_name> <Number of key>\n");
		}
	return 0;
}
