/*HW2- CSC548
Author - ajain28 ,ABHASH JAIN
*/

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
	ind->index_arr = (index_S*) malloc(size*sizeof(index_S));
	memset(ind->index_arr,0,size*sizeof(index_S));
	ind->used=0; 
	ind->size = size;
}

void insertIndex(index_list *ind,index_S i){
	if(ind->size == ind->used){
		ind->size*=2;
		ind->index_arr = (index_S*)realloc(ind->index_arr,ind->size*sizeof(index_S));
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
	a->a_arr = (avail_S*) malloc(size*sizeof(avail_S));
	a->used =0;
	a->size =size;
}

void insertAvail(avail_list *a,avail_S i,char *mode){
	if(a->size == a->used){
		a->size*=2;
		a->a_arr = (avail_S*) realloc(a->a_arr,a->size*sizeof(avail_S));
	}
	if(a->used == 0){
		int j = a->used++;
		a->a_arr[j].size = i.size;
		a->a_arr[j].off = i.off;	
		return;				//value is already added then return
	}
	if(strcmp(mode,"--first-fit")==0){
		int j = a->used++;
		a->a_arr[j].size = i.size;
		a->a_arr[j].off = i.off;	
	} else if(strcmp(mode,"--best-fit")==0){
		//Add in ascending order of size
		int temp = a->used -1;
		while (i.size < a->a_arr[temp+1].size && temp>=0){
			a->a_arr[temp+1].size = a->a_arr[temp].size;
			a->a_arr[temp+1].off = a->a_arr[temp].off;
			temp--;
		}
		a->used++;
		a->a_arr[temp+1].size  = i.size;
		a->a_arr[temp+1].off =i.off;
	} else if(strcmp(mode,"--worst-fit")==0){
		//add in descending order of size
		int temp = a->used -1;
		while (i.size > a->a_arr[temp].size && temp>=0){
			a->a_arr[temp+1].size = a->a_arr[temp].size;
			a->a_arr[temp+1].off = a->a_arr[temp].off;
			temp--;
		}
		a->used++;
		a->a_arr[temp+1].size  = i.size;
		a->a_arr[temp+1].off =i.off;
	}
}
/*Get total Hole size*/
long holeSize(avail_list *a){
	long size = 0;
	int i=0;
	for (;i<a->used;i++){
		size+=a->a_arr[i].size;
	}
	return size;
}

/*Compare function to sort the avail_list in ascending and descending order*/
int comp_a(const void *a, const void *b){
	 if(((avail_S*)a)->size < ((avail_S*)b)->size)
		return -1;
	 else if(((avail_S*)a)->size > ((avail_S*)b)->size)
		return 1;
	 else{
	 	if(((avail_S*)a)->off < ((avail_S*)b)->off)
			return -1;
	 	else if(((avail_S*)a)->off > ((avail_S*)b)->off)
			return 1;
		return 0;
	}
}

int comp_d(const void *a, const void *b){
	 if(((avail_S*)a)->size < ((avail_S*)b)->size)
		return 1;
	 else if(((avail_S*)a)->size > ((avail_S*)b)->size)
		return -1;
	 else{
	 	if(((avail_S*)a)->off < ((avail_S*)b)->off)
			return -1;
	 	else if(((avail_S*)a)->off > ((avail_S*)b)->off)
			return 1;
		return 0;
	}
}
void printStudent(studentRecord *s){
	printf("DEBUG: SID: %d, Student Name: %s, last name: %s, Major: %s\n",s->SID,s->fname,s->lname,s->major);
}

void printIndexList (index_list *i){
	//printf("DEBUG: size is %d and used is %d\n",i->size,i->used);
	int j;
	for(j=0;j< i->used;j++){
		printf("key=%d: offset=%ld\n",i->index_arr[j].key,i->index_arr[j].off);
	}
}	
void printAvailList (avail_list *i){
	//printf("DEBUG: size is %d and used is %d\n",i->size,i->used);
	int j;
	for(j=0;j< i->used;j++){
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
//count digits
int countDigits(int n){
	int c=0;
	while(n!=0){
		n/=10;
		c++;
	}
	return c;
}

//Get formatted student record to write in student.db
int formattedStudent(studentRecord *s,char *record){
	sprintf(record,"%d|%s|%s|%s",s->SID,s->fname,s->lname,s->major);
	int length = strlen(record);
	int finalLength = length + countDigits(length)+1;
	char *t = (char*) malloc(sizeof(char)*(finalLength));
	memset(t,0,(sizeof(char)*finalLength));
	memcpy(t,record,length);
	sprintf(record,"%d|%s",finalLength,t);
	//printf("DEBUG:record= %s\n",record);
	free(t);
	return finalLength;
}
/*Delete: Delete the Node from index List*/
/*Delete the given index and shift the other index to make this as continious*/
void deleteIndex (index_list *index, int i){
	int j;
	for(j=i;j<index->used;j++){
		index->index_arr[i].key = index->index_arr[j].key;
		index->index_arr[i].off = index->index_arr[j].off;
		i=j;
	}
	index->used--;
}
/*Delete: Delete the Node from avail List*/
/*Delete the given index and shift the other index to make this as continious*/
void deleteAvail (avail_list *index, int i){
	int j;
	for(j=i;j<index->used;j++){
		index->a_arr[i].size = index->a_arr[j].size;
		index->a_arr[i].off = index->a_arr[j].off;
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
	memset(buf,0,sizeof(char)*LINE_MAX);
	//studentRecord s;
	if(loc ==-1){
		printf("No record with SID=%d exists\n",key);
	} else {				//Key is present,read the content from given index
		int find = i->index_arr[loc].off;
		//get the size of record from the offset first
		int recSize;
		char temp[12],*t; 
		fseek(fp,find,SEEK_SET);
		fread(temp,sizeof(char),11,fp);
		t = strtok(temp,"|");
		recSize = atoi(t);
		int digit = countDigits(recSize);
		recSize = recSize - (digit+1);
		fseek(fp,find+digit+1,SEEK_SET);
		fread(buf,sizeof(char),recSize,fp);
		//printf("DEBUG: content read %s\n",buf);
		printf("%s",buf);
	}
	free(buf);
}

/*deleteRecord
	Mode is require to arrange the avail list*/
void deleteRecord(FILE *fp,index_list *i,avail_list *a,int key,char *mode){
	//look for key
	int loc = binaryIndex(i,key);
	if(loc == -1)
		printf("No record with SID=%d exists\n",key);
	else{
		int find = i->index_arr[loc].off;
		int recSize;
		char temp[12],*t;
		fseek(fp,find,SEEK_SET);
		fread(temp,sizeof(char),11,fp);
		t = strtok(temp,"|");
		recSize = atoi(t);
		avail_S te;
		te.size = recSize;
		te.off = find;
		//Insert a entry in avail_list with offset and recSize
		insertAvail(a,te,mode);
		deleteIndex(i,loc);
	}
}
/*utility function to get index of avail_list based on the size and mode
	if -1 - No specific hole is there*/
int locationFinder(avail_list *a,int size,char *mode){
	return 0;
}


/*Add a new record
FP is student file descriptor
*/
void addRecord(FILE *fp,char *mode, index_list *i, avail_list *a,studentRecord *s){
	char * temp = (char*) malloc(sizeof(char)*LINE_MAX);
	memset(temp,0,sizeof(char)*LINE_MAX);
	int n = formattedStudent(s,temp);
	int j=0,pos,remain;
	index_S ind;
	avail_S a_l;
	//Based on the mode finalize your location where is your seek to add the record
	if(strcmp(mode,"--first-fit")==0){
		if(a->used != 0){  //Free space in avail list
			//look for correct size hole by doing linear search
			while(j < a->used){
				if(a->a_arr[j].size >= n){
					fseek(fp,a->a_arr[j].off,SEEK_SET);
					pos = ftell(fp);
					fwrite(temp,sizeof(char),n,fp);
					//printf("DEBUG: added record at pos=%d\n",pos);
					ind.key = s->SID;
					ind.off = pos;
					insertIndex(i,ind);
					remain = a->a_arr[j].size - n;
					//Check remain should be >0 to add into avail_list
					if(remain>0){			
						a_l.size = remain;
						a_l.off = a->a_arr[j].off+n;							
						insertAvail(a,a_l,mode);
					}
					//TODO: Possible place for bug
					deleteAvail(a,j);
					return;		
				} else {
					j++;
					continue;
				}
			}
		}
	} else if(strcmp(mode,"--best-fit")==0) {
		if(a->used!=0){
			while(j < a->used){
				if(a->a_arr[j].size >= n){
					fseek(fp,a->a_arr[j].off,SEEK_SET);
					pos = ftell(fp);
					fwrite(temp,sizeof(char),n,fp);
					ind.key = s->SID;
					ind.off = pos;
					insertIndex(i,ind);
					remain = a->a_arr[j].size - n;
					//Check remain should be >0 to add into avail_list
					if(remain>0){
						a_l.size = remain;
						a_l.off = a->a_arr[j].off+n;
						insertAvail(a,a_l,mode);
					}
					deleteAvail(a,j);
					return;
				} else {
					j++;
					continue;
				}
			}
		}	
	} else if(strcmp(mode,"--worst-fit")==0){
		if(a->used!=0){
			if(a->a_arr[0].size > n){
				fseek(fp,a->a_arr[0].off,SEEK_SET);
				pos = ftell(fp);
				fwrite(temp,sizeof(char),n,fp);
				ind.key = s->SID;
				ind.off = pos;
				insertIndex(i,ind);
				remain = a->a_arr[j].size - n;
				//Check remain should be >0 to add into avail_list
				if(remain>0){
					a_l.size = remain;
					a_l.off = a->a_arr[j].off+n;
					insertAvail(a,a_l,mode);
				}
				deleteAvail(a,j);
				return;
			}
		}	

	}
	fseek(fp,0,SEEK_END);
	pos = ftell(fp);
	fwrite(temp,sizeof(char),n,fp);
	ind.key = s->SID;
	ind.off = pos;
	insertIndex(i,ind);
	free(temp);
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
	index->size =size;
	index->used =used;
	index->index_arr = tempIndex;
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
		index.size =0;
		index.used=0;
		av.size = 0;
		av.used = 0;
		char line[LINE_MAX];
		studentRecord *s = (studentRecord*) malloc(sizeof(studentRecord));
		memset(s,0,sizeof(studentRecord));
		//Check for index.bin file if exists load the data in file otherwise create a new datastructure
		FILE *findex = fopen("index.bin","rb");
		FILE *aindex = fopen("avail.bin","rb");
		fstudent = fopen(s_file,"r+b");
		if(fstudent == NULL){
			FILE *tf = fopen(s_file,"wb");
			fclose(tf);
			fstudent = fopen(s_file,"r+b");
		}	
		if(findex == NULL){
			//File is not there Initialize it
			initIndex(&index,5);
		}else {
			//read data from file and give it index_list
			//printIndexList(&index);
			loadIndex(findex,&index);
			fclose(findex);
		}
		if(aindex == NULL){
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
				fwrite(&av.size,sizeof(int),1,fa);
				fwrite(&av.used,sizeof(int),1,fa);
				fwrite(av.a_arr,sizeof(avail_S),av.size,fa);
				printf("Index:\n");
				printIndexList(&index);
				printf("Availability:\n");
				printAvailList(&av);
				printf("Number of holes: %d\n",av.used);
				printf("Hole space: %ld\n",holeSize(&av));
				fclose(fi);
				fclose(fa);
				fclose(fstudent);
				free(s);
				//free(index.index_arr);
				//free(av.a_arr);
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
					addRecord(fstudent,mode,&index,&av,s);	
					//printAvailList(&av);
					//printf("D1:\n");
				} else {		//Record is present with key in index list
					printf("Record with SID=%d exists\n",s->SID);
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
				deleteRecord(fstudent,&index,&av,sid,mode);
				//printAvailList(&av);
				//printf("D2:\n");
			} else {
				printf("Unknown Option\n");
			}
		}
	} else {
		printf("Usage: <exe> avail-list-order student-db-file\n");
	}
	return 0;
}
