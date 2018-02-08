#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<string.h>

#define NAMESIZE 256

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

//Availablity List

typedef struct {
	int size;
	long off;
} avail_S;

typedef struct{
	int size;
	int used;
	avail_S *a_arr;
} avail_list;

typedef struct {
	int SID;
	char fname[NAMESIZE];
	char lname[NAMESIZE];
	char major[NAMESIZE];
}studentRecord;

void initIndex(index_list* ind,int size){
	ind->index_arr = (index_list*) malloc(size*sizeof(index_S));
	memset(ind->index_arr,0,size*sizeof(index_S));
	ind->used=0; 
	ind->size = size;
}

void insertIndex(index_list *ind,index_S i){
	if(ind->size == ind->used){
		ind->size*=2;
		ind->index_arr = (index_list*)realloc(ind->index_arr,ind->size*sizeof(index_S));
	}
	int temp = ind->used - 1;
	while(i.key < ind->index_arr[temp].key && temp>=0){
		ind->index_arr[temp+1].key = ind->index_arr[temp].key;
		ind->index_arr[temp+1].off = ind->index_arr[temp].off;
		temp--;
	}
	ind->used++;
	ind->index_arr[temp+1].key = i.key;
	ind->index_arr[temp+1].off = i.off;
}

void initAvail(avail_list *a,int size){
	a->a_arr = (avail_list *) malloc(size*sizeof(avail_S));
	a->used =0;
	a->size =size;
}

void insertAvail(avail_list *a,avail_S i){
	if(a->size == a->used){
		a->size*=2;
		a->a_arr = (avail_list *) realloc(a->a_arr,a->size*sizeof(avail_S));
	}
	a->a_arr[a->used++]=i;
}

void printStudent(studentRecord *s){
	printf("DEBUG: SID: %d, Student Name: %s, last name: %s, Major: %s\n",s->SID,s->fname,s->lname,s->major);
}

void printIndexList (index_list *i){
	printf("DEBUG: size is %d and used is %d\n",i->size,i->used);
	for(int j=0;j< i->used;j++){
		printf("DEBUG:Key is %d and offset is %ld\n",i->index_arr[j].key,i->index_arr[j].off);
	}
}	

int searchIndex(index_S *i,int l,int h,int key){
	if(l>h)
		return -1;
	int mid = (l+h)/2;
	if(i[mid].key == key)
		return mid;
	else if(i[mid].key > key)
		return searchIndex(i,l,mid-1,key);
	else
		return searchIndex(i,mid+1,h,key);
}
//Search key in index list
int binaryIndex(index_list *i,int key){
	return searchIndex(i->index_arr,0,i->used,key);
}

//Get formatted student record to write in student.db
int formattedStudent(studentRecord *s,char *record){
	sprintf(record,"%d|%s|%s|%s",s->SID,s->fname,s->lname,s->major);
	int length = strlen(record);
	char *t = (char*) malloc(sizeof(char)*(length+11));
	memcpy(t,record,length);
	sprintf(record,"%010d|%s",length+11,t);
	//printf("DEBUG:record= %s\n",record);
	return length+11;
}
/*
Search Element with key if found read the content on the off and print the key otherwise give 
Appropriate msg.
I/P: key/SID and list and FILE pointer for student.db
*/
void findElement(FILE *fp, index_list *i,int key){
	int loc = binaryIndex(i,key);
	char *buf = (char *)malloc(sizeof(char)*LINE_MAX);
	//studentRecord s;
	if(loc ==-1){
		printf("No record with SID=%d exists\n",key);
	} else {				//Key is present,read the content from given index
		int find = i->index_arr[loc].off;
		//get the size of record from the offset first
		int recSize;
		char temp[11]; 
		fseek(fp,find,SEEK_SET);
		fread(temp,sizeof(char),10,fp);
		recSize = atoi(temp);
		fseek(fp,find+11,SEEK_SET);
		fread(buf,sizeof(char),recSize-11,fp);
		//printf("DEBUG: content read %s\n",buf);
		printf("%s\n",buf);
	}
	free(buf);
}

int main(int argc,char *argv[]){
	if(argc==3){
		char *mode = argv[1];
		char *s_file = argv[2];
		char temp[LINE_MAX];
		FILE *fstudent;
		index_list index;
		//avail_list *av;
		char line[LINE_MAX];
		studentRecord *s = (studentRecord*) malloc(sizeof(studentRecord));
		memset(s,0,sizeof(studentRecord));
		//Check for index.bin file if exists load the data in file otherwise create a new datastructure
		FILE *findex = fopen("index.bin","rb");
		fstudent = fopen(s_file,"ab+");
		if(findex == NULL){
			//File is not there Initialize it
			initIndex(&index,5);
		}else {
			//read data from file and give it index_list
			int size = 0;
			int used = 0; 
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
			//printIndexList(&index);
			fclose(findex);
		}

		while(fgets(line,LINE_MAX,stdin)!=NULL){
			memcpy(temp,line,LINE_MAX);
			//printf("Read : '%s'\n",line);
			//printf("Length is %d\n",strlen(line));
			char *ch = strtok(temp," ");
			if(strcmp(ch,"end\n")==0){
				//before ending save the content of index in index.bin
				remove("index.bin");
				FILE *fi = fopen("index.bin","wb");
				fwrite(&index.size,sizeof(int),1,fi);
				fwrite(&index.used,sizeof(int),1,fi);
				fwrite(index.index_arr,sizeof(index_S),index.size,fi);
				//printf("DEBUG: Number of records written is %d\n",index.size);
				fclose(fi);
				fclose(fstudent);
				exit(0);			//if it is marked as end Tx then exit from program Normally
			}else if(strcmp(ch,"add")==0){
				ch = strtok(NULL," ");
				//printf("Look for SID %d\t",atoi(ch));
				ch = strtok(NULL,"|");
				s->SID = atoi(ch);
				ch = strtok(NULL,"|");
				memcpy(s->fname,ch,NAMESIZE);
				ch = strtok(NULL,"|");
				memcpy(s->lname,ch,NAMESIZE);
				ch = strtok(NULL,"|");
				memcpy(s->major,ch,NAMESIZE);
				//printStudent(s);
				if(strcmp(mode,"--first-fit")==0){
					//Search in the list
					if(binaryIndex(&index,s->SID)==-1){	//Record not found
						//add a new record and write it into the index found by avail list or at the end
						//TODO: Check for the avail list before adding
						char * temp = (char*) malloc(sizeof(char)*LINE_MAX);
						memset(temp,0,sizeof(char)*LINE_MAX);
						int n = formattedStudent(s,temp);
						//printf("temp= %s\n",temp);
						fseek(fstudent,0,SEEK_END);
						int pos = ftell(fstudent);
						//printf("postion= %d\n",pos);
						fwrite(temp,sizeof(char),n,fstudent);
						index_S i;
						i.key = s->SID;
						i.off = pos;
						insertIndex(&index,i);
						//printf("Element Inserted\n");
					} else {		//Record is present with key in index list
						printf("Record with SID=%d exists\n",s->SID);
					}
				}
			} else if(strcmp(ch,"find") == 0){
				ch= strtok(NULL," ");
				int sid = atoi(ch);
				findElement(fstudent,&index,sid);			
				//printf("Find with key %d\n",sid);
			} else if(strcmp(ch,"del") == 0){
				ch= strtok(NULL," ");
				int sid = atoi(ch);
				printf("delete with key %d\n",sid);
			} else {
				printf("Unknown Option\n");
			}
		}
	} else {
		printf("Usage: <exe> avail-list-order student-db-file\n");
	}
	return 0;
}
