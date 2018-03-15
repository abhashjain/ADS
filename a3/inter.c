#include<stdio.h>

int main(int argc,char *argv[]){
	int temp[1000];
	FILE *fp = fopen(argv[1],"rb");
	fread(temp,sizeof(int),1000,fp);
	for(int i=0;i<1000;i++){
		printf("%d\t",temp[i]);
	}
	printf("\n");
	return 0;
}
