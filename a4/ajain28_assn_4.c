/*
CSC541- Assignment-4
Author: Abhash Jain, ajain28
*/
#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<string.h>

typedef struct{
	int n;
	int *key;
	long *child;
}btree_node;

btree_node* MakeNode(int order){
	btree_node *temp = (btree_node *)malloc(sizeof(btree_node));
	temp->n =0;
	temp->key = (int *)calloc(order-1,sizeof(int));
	temp->child = (long *)calloc(order,sizeof(long));
	return (temp);
}

btree_node* readFromFile(FILE *fp,long offset,int order){
	btree_node *temp = MakeNode(order);
	fseek(fp,offset,SEEK_SET);
	fread(temp->n,sizeof(int),1,fp);
	int t = fread(&(temp->key),sizeof(int),order-1,fp);
	fread(&(temp->child),sizeof(long),order,fp);
	if(t ==0){
		free(temp);
		return NULL;
	} else {
		return temp;
	}
}

int main(int argc,char *argv[]){
	if(argc == 3){
		char *index_file = argv[1];
		char *order_c = argv[2];
		int order = atoi(order_c);
		char line[LINE_MAX];
		char temp[LINE_MAX];
		FILE *fp  = NULL;
		int root_offset = -1;
		//printf("File order is %d\n",order); 
		//read the content from a input file redirection
		//Get the root node from index file which is present and open in rb+ mode
		//otherwise create a new node and push it to this file
		fp = fopen(index_file,"rb+");
		if(fp!=NULL){
			//get the offset long type first and then read the root
			fread(root_offset,sizeof(long),1,fp);
			
		}else{
			//create a new file and continue from there
			fp = fopen(index_file,"wb+");
			fwrite(root_offset,sizeof(long),1,fp);
			
		}
		while(fgets(line,LINE_MAX,stdin)!=NULL){
			memset(temp,0,LINE_MAX);
			memcpy(temp,line,LINE_MAX);
			char *ch = strtok(temp," ");
			if(strcmp(ch,"end\n")==0){
				printf("Will write the root offset to the front and then close\n");
				exit(0);
			} else if(strcmp(ch,"add")==0){
				ch = strtok(NULL," ");
				int data = atoi(ch);
				printf("add %d key to B tree\n",data);
			} else if(strcmp(ch,"find")==0){
				ch = strtok(NULL," ");
				int data = atoi(ch);
				printf("Find %d in B-tree\n",data);
			} else if(strcmp(ch,"print\n")==0){
				printf("Print the whole B-tree by level\n");
			} else {
				printf("Invalid Option\n");
			}
		}
	} else {
		printf("Usage: <executatable> <index_file> <order>\n");
	}
	return 0;	
}
