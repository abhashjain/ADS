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

void insertAvail(avail_list *a,avail_S i,char *mode){
	if(a->size == a->used){
		a->size*=2;
		a->a_arr = (avail_list *) realloc(a->a_arr,a->size*sizeof(avail_S));
	}
	if(strcmp(mode,"--first-fit")==0){
		int j = a->used++;
		a->a_arr[j].size = i.size;
		a->a_arr[j].off = i.off;	
	} else if(strcmp(mode,"--best-fit")==0){
		//Add in ascending order of size
		int temp = a->used -1;
		while (i.size < a->avail_arr[temp+1].size && temp>=0){
			a->avail_arr[temp+1].size = a->avail_arr[temp].size;
			a->avail_arr[temp+1].offset = a->avail_arr[temp.off];
			temp--;
		}
		a->used++;
		a->avail_arr[temp+1].size  = i.size;
		a->avail_arr[temp+1].off =i.off;
	} else if(strcmp(mode,"--worst-fit")==0){
		//add in descending order of size
		int temp = a->used -1;
		while (i.size > a->avail_arr[temp+1].size && temp>=0){
			a->avail_arr[temp+1].size = a->avail_arr[temp].size;
			a->avail_arr[temp+1].offset = a->avail_arr[temp.off];
			temp--;
		}
		a->used++;
		a->avail_arr[temp+1].size  = i.size;
		a->avail_arr[temp+1].off =i.off;
	}
}
/*Get total Hole size*/
long holeSize(avail_list *a){
	long size = 0;
	int i=0;
	for (;i<a->used;a++){
		size+=a->a_arr[i].size;
	}
	return size;
}

/*Compare function to sort the avail_list in ascending and descending order*/
int comp_a(const void *a, const void *b){
	 if(((avail_S*)a)->size > ((avail_S*)b)->size)
		return -1;
	 else if(((avail_S*)a)->size < ((avail_S*)b)->size)
		return 1;
	 else{
	 	if(((avail_S*)a)->off > ((avail_S*)b)->off)
			return -1;
	 	else if(((avail_S*)a)->off < ((avail_S*)b)->off)
			return 1;
		return 0;
	}
}

int comp_d(const void *a, const void *b){
	 if(((avail_S*)a)->size > ((avail_S*)b)->size)
		return 1;
	 else if(((avail_S*)a)->size < ((avail_S*)b)->size)
		return -1;
	 else{
	 	if(((avail_S*)a)->off > ((avail_S*)b)->off)
			return -1;
	 	else if(((avail_S*)a)->off < ((avail_S*)b)->off)
			return 1;
		return 0;
	}
}
void printStudent(studentRecord *s){
	printf("DEBUG: SID: %d, Student Name: %s, last name: %s, Major: %s\n",s->SID,s->fname,s->lname,s->major);
}

void printIndexList (index_list *i){
	//printf("DEBUG: size is %d and used is %d\n",i->size,i->used);
	for(int j=0;j< i->used;j++){
		printf("key=%d: offset=%ld\n",i->index_arr[j].key,i->index_arr[j].off);
	}
}	
void printAvailList (avail_list *i){
	//printf("DEBUG: size is %d and used is %d\n",i->size,i->used);
	for(int j=0;j< i->used;j++){
		printf("size=%d: offset=%ld\n",i->a_arr[j].size,i->a_arr[j].off);
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
/*Delete: Delete the Node from index List*/
/*Delete the given index and shift the other index to make this as continious*/
void deleteIndex (index_list *index, int i){
	int j=0;
	for(j=0;j<index->used;j++){
		index->index_arr[i].key = index->index_arr[j].key;
		index->index_arr[i].off = index->index_arr[j].off;
		i=j;
	}
	index->used--;
}
/*Delete: Delete the Node from avail List*/
/*Delete the given index and shift the other index to make this as continious*/
void deleteIndex (avail_list *index, int i){
	int j=0;
	for(j=0;j<index->used;j++){
		index-avail_arr[i].size = index->avail_arr[j].size;
		index->avail_arr[i].off = index->index_arr[j].off;
		i=j;
	}
	index->used--;
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

/*deleteRecord
	Mode is require to arrange the avail list*/
void deleteRecord(File *fp,index_list *i,avail_list *a,int key,char *mode){
	//look for key
	int loc = binaryIndex(i,key);
	if(loc == 1)
		printf("No record with SID=%d exists\n",key);
	else{
		int find = i->index_arr[loc].off;
		int recSize;
		char temp[11];
		fseek(fp,find,SEEK_SET);
		fread(temp,sizeof(char),10,fp);
		recSize = atoi(temp);
		avail_S temp;
		temp.size = recSize;
		temp.off = find;
		//Insert a entry in avail_list with offset and recSize
		insertAvail(a,temp,mode);
		deleteIndex(i,loc);
	}
}
/*utility function to get index of avail_list based on the size and mode
	if -1 - No specific hole is there*/
int locationFinder(avail_list *a,int size,char *mode){
	
}


/*Add a new record
FP is student file descriptor
*/
void addRecord(File *fp,char *mode, index_list *i, avail_list *a,studentRecord *s){
	char * temp = (char*) malloc(sizeof(char)*LINE_MAX);
	memset(temp,0,sizeof(char)*LINE_MAX);
	int n = formattedStudent(s,temp);
	int loc = -1,j=0;
	//Based on the mode finalize your location where is your seek to add the record
	if(strcmp(mode,"--first-fit")==0){
		if(a->used != 0){  //Free space in avail list
			//look for correct size hole by doing linear search
			while(j < a->used){
				
			}
		}
	} else if(strcmp(mode,"--best-fit")==0) {
		
	} else if(strcmp(mode,"--worst-fit")==0){
	
	}
	fseek(fp,0,SEEK_END);
	int pos = ftell(fp);
	fwrite(temp,sizeof(char),n,fp);
	index_S i;
	i.key = s->SID;
	i.off = pos;
	insertIndex(&index,i);
}
/*Load index file*/
void loadIndex(FILE *findex,index_list *index){
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
}

/*Load Availablity list from file based on the mentioned mode*/
void loadAvailFile(FILE *fp,char *mode,avail_list *a){
	int size = 0;
	int used = 0;
	fread(&size,sizeof(int),1,fp);
	fseek(fp,sizeof(int),SEEK_SET);
	fread(&used,sizeof(int),1,fp);
	fseek(fp,2*sizeof(int),SEEK_SET);
	avail_S *temp = (avail_S *)malloc(sizeof(avail_S)*size);
	memset(temp,0,sizeof(avail_S)*size);
	fread(temp,sizeof(avail_S),size,fp);
	a->size = size;
	a->used = used;
	a->a_arr = temp;
	if(strcmp(mode,"--best-fit")==0){
		qsort(a->a_arr,used,sizeof(avail_S),comp_a);
	} else if(strcmp(mode,"--worst-fit")==0) {
		qsort(a->a_arr,used,sizeof(avail_S),comp_d);
	}
}


int main(int argc,char *argv[]){
	if(argc==3){
		char *mode = argv[1];
		char *s_file = argv[2];
		char temp[LINE_MAX];
		FILE *fstudent;
		index_list index;
		avail_list av;		
		char line[LINE_MAX];
		studentRecord *s = (studentRecord*) malloc(sizeof(studentRecord));
		memset(s,0,sizeof(studentRecord));
		//Check for index.bin file if exists load the data in file otherwise create a new datastructure
		FILE *findex = fopen("index.bin","rb");
		FILE *aindex = fopen("avail.bin","rb");
		fstudent = fopen(s_file,"ab+");
		if(findex == NULL){
			//File is not there Initialize it
			initIndex(&index,5);
		}else {
			//read data from file and give it index_list
			//printIndexList(&index);
			loadIndex(findex,&index);
			fclose(findex);
		}
		if(aindex ==NULL){
			initAvail(&av,5);
		} else {
			//Based on the mode specified in argument load the file into DS
			loadAvailFile(aindex,mode,&av);
			fclose(aindex);
		}
		while(fgets(line,LINE_MAX,stdin)!=NULL){
			memcpy(temp,line,LINE_MAX);
			//printf("Read : '%s'\n",line);
			//printf("Length is %d\n",strlen(line));
			char *ch = strtok(temp," ");
			if(strcmp(ch,"end\n")==0){
				//before ending save the content of index in index.bin and avail.bin
				remove("index.bin");
				remove("avail.bin");
				FILE *fi = fopen("index.bin","wb");
				FILE *fa = fopen("avail.bin","wb");
				fwrite(&index.size,sizeof(int),1,fi);
				fwrite(&index.used,sizeof(int),1,fi);
				fwrite(index.index_arr,sizeof(index_S),index.size,fi);
				//printf("DEBUG: Number of records written is %d\n",index.size);
				//Print the avail and index list and size and hole count
				printf("Index:\n");
				printIndexList(index);
				printf("Availability:\n");
				printAvailList(av);
				printf("Number of holes: %d\n",av->used);
				printf("Hole space: %ld\n",holeSize(av));
				fclose(fi);
				fclose(fa)
				fclose(fstudent);
				free(index);
				free(av);
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
				//add a record with specified strategy
				if(binaryIndex(&index,s->SID)==-1){	//Record not found
					addRecord(fstudent,mode,index,av,s);	
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
				//printf("delete with key %d\n",sid);
				deleteRecord(fstudent,index,av,sid,mode);
			} else {
				printf("Unknown Option\n");
			}
		}
	} else {
		printf("Usage: <exe> avail-list-order student-db-file\n");
	}
	return 0;
}
