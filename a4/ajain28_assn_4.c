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

typedef struct{
	int key;
	int isValid;
	long offset;
}returnNode;

typedef struct{
	long offset;
	qNode* next;
}qNode;

int comp_a(const void *a, const void *b){
    const int *l = (const int *)a;
    const int *r = (const int *)b;
    return *l - *r;
}


/* add an element in the last*/
void push(qNode *root, long offset){
	if(root==NULL){
		//this is the first Node to add in the queue
		qNode *temp = (qNode *)malloc(sizeof(qNode));
		temp->next = NULL;
	}
}

/*Remove an element from front of the queue*/
void pop(qNode *root){

}

/*Return the element at front*/
qNode* front(qNode *root){

}

btree_node* makeNode(int order){
	btree_node *temp = (btree_node *)malloc(sizeof(btree_node));
	temp->n =0;
	temp->key = (int *)calloc(order-1,sizeof(int));
	temp->child = (long *)calloc(order,sizeof(long));
	return (temp);
}

btree_node* readNode(FILE *fp,long offset,int order){
	btree_node *temp = makeNode(order);
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
/*This method search the target and return the offset to the node,
	if tg not there then return -1*/
long bsearch_tree(int target,long offset,int order,FILE *fp){
	b_tree *root =  readFromFile(fp,offset,order);
	long s=0;
	while(s < (order-1)){
		if(target == root->key[s]){
			return root->child[s];
		} else if(target < root->key[s]){
			break;
		} else {
			s++;
		}
	}//end of while
	//read the content of root->child[s] offset value from disk file
	if(root->ref[s]!=0){
		return bsearch_tree(target,root->ref[s],order,fp);
	} else {
		return -1;
	}
}

retrunNode addNode(FILE *fp,int order,int key,int offset){
	//Given a offset read the node from b tree file
	btree_node *node = readNode(fp,offset,order);
	int i,isLeaf = 1,j=0,k=0;
	returnNode rNode;

	if(node == NULL){
		rNode.isValid = -1;
		return rNode;
	}
	for(i=0;i<node.n;i++){
		if(key<node.key[i])
			break;
	}
	if(node.child[i]!=0){
		isLeaf=0;
		rNode = addNode(fp,order,key,node.child[i]);
	}

	//IF it is a leaf node then add it here, Before adding check if this node is full
	//or not, If not full, create a new node and arrange them in sorted order with new key
	//otherwise the split node and return the spl;it key to parent call
	if(isLeaf==1){
		//leaf case
		if(node.n<=order-1){
			//Node has place to accomodate this node
			btree_node *newNode = makeNode(order);
			k=0;
			newNode.n = node.n + 1;
			for(i=0;i<=newNode.n;i++){
				if(node.key[i]<key){
					newNode.key[k++] = node.key[i]; 
				} else {
					newNode.key[k++] = key;
				}
			}
			retrunNode temp;
			temp.isValid = -1 ; // invalidate return object
			//overwrite node to current nodes offset
			fseek(fp,offset,SEEK_SET);
			fwrite(&newNode.n,sizeof(int),1,fp);
			fwrite(newNode.key,sizeof(int),order-1,fp);
			fwrite(newNode.child,sizeof(long),order,fp);
			return temp;
		} else {
			//have to split this node to insert this key
			//copy all the node to a temp array of size node.n+1 and then sort them
			

			int tempArr[node.n+1];
			for(j=0;j<node.n;j++){
				tempArr[j] = node.key[i]; 	
			}
			tempArr[j] = key;
			//sort them
			qsort(tempArr,j+1,sizeof(int),comp_a);
			//to split find the middle element
			int m = order/2;
			//create two node and place tempArr[0...m-1] in one and tempArr[m+1...order-1] in other node
			btree_node *leNode = makeNode(order);
			btree_node *rtNode = makeNode(order);
			for(i=0;i<m;i++){
				leNode->key[i] = tempArr[i];
			}
			leNode->n = m;
			k=0;
			for(i=m+1;i<order;i++){
				rtnode->key[k++] = tempArr[i];
			}
			rtNode->n = k;
			fseek(fp,offset,SEEK_SET);
			fwrite(&leNode.n,sizeof(int),1,fp);
			fwrite(leNode.key,sizeof(int),order-1,fp);
			fwrite(leNode.child,sizeof(long),order,fp);
			//save the given offset and key in ret Structure
			returnNode temp; 
			temp.isValid = 1;
			temp.key  = tempArr[m];
			fseek(fp,0,SEEK_END);
			temp.offset= ftell(fp);
			//write right node to end of file
			fwrite(&rtNode.n,sizeof(int),1,fp);
			fwrite(rtNode.key,sizeof(int),order-1,fp);
			fwrite(rtNode.child,sizeof(long),order,fp);
			return temp;

		}
	} else {
		//non leaf case, you have reach here to check the return object with current node
		//Here also in the internal node overflow can occur or not, if not then simply add the keys with copying their child
		if(rNode.isValid ==-1){
			return rNode;	//nothing needs to be done as there is nothing to add
		}
		//check for overflow
		if(node.n < order-1){
			btree_node *newNode = makeNode(order);
			k=0;
			newNode.n = node.n+1;
			//copy the node keys and its child key in sorted order.
			for(k=0;k<i;k++){
				newNode.key[k] = node.key[k];
				newNode.child[k] = node.child[k];
			}
			//copy the node from return structure
			newNode.key[k] = rNode.key;
			newNode.child[k++] = rNode.offset;
			for(j=i;j<order-1;j++){
				newNode.key[k] = node.key[j];
				newNode.child[k++] = node.child[j];
			}
			returnNode temp;
			temp.isValid =-1;
			fseek(fp,offset,SEEK_SET);
			fwrite(&newNode.n,sizeof(int),1,fp);
			fwrite(newNode.key,sizeof(int),order-1,fp);
			fwrite(newNode.child,sizeof(long),order,fp);
			return temp;
		} else {
			//overflow on internal node, do the similar thing as before by creaing 2 Node and 
			//rt will be written in end and left will be overwritten the same location but, we careful about the 
			//child pointer now
			//TODO
			int temp;
		}
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
				//here check for the node using find method then find it
				//if node not found then only call add method.
				if(bsearch_tree(data,root_offset,order,fp)!=-1){
					printf("Entry with key=%d already exists\n",data);
				} else {
					//call add this entry
				}
			} else if(strcmp(ch,"find")==0){
				ch = strtok(NULL," ");
				int data = atoi(ch);
				if(bsearch_tree(data,root_offset,order,fp)!=-1){
					printf("Entry with key=%d exists\n",data);
				} else {
					printf("Entry with key=%d does not exist\n",data);
				}
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
