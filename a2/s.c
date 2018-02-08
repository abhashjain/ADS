#include<stdio.h>
#include<stdlib.h>
#include<string.h>


int main(){
	FILE *fp = fopen("student.db","rb+");	
	char *temp = (char*) malloc(sizeof(char)*2048);
	fseek(fp,0,SEEK_END);	
	int pos = ftell(fp);
	fseek(fp,0,SEEK_SET); 
	fread(temp,sizeof(char),pos,fp);
	printf("%s\n",temp);
	fclose(fp);
	free(temp);
	return 0;
}

