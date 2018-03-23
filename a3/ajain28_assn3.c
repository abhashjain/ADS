/*Abhash Jain
CSC541- Assignment-3 Disk Based Merge Sort*/

#include<stdio.h>
#include<sys/time.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#define BUCKET 1000
#define NAME_SIZE 14


//int heap_size =0;	//global variable indicate the heap size
int comp_a(const void *a, const void *b){
    const int *l = (const int *)a;
	const int *r = (const int *)b;
	return *l - *r;
}

/*struct HeapNode{
	int element;
	int i;
	int j;
};

void swap(HeapNode *x, HeapNode *y)
{
    MinHeapNode temp = *x;
	*x = *y;
 	*y = temp;
}

int left(int i) { return (2*i + 1); }

int right(int i) { return (2*i + 2); }

void Heapify(int i)
{
    int l = left(i);
    int r = right(i);
    int smallest = i;
    if (l < heap_size && harr[l].element < harr[i].element)
        smallest = l;
    if (r < heap_size && harr[r].element < harr[smallest].element)
        smallest = r;
    if (smallest != i)
    {
        swap(&harr[i], &harr[smallest]);
        Heapify(smallest);
    }
}
*/
void get_number_from_runs(int *input,FILE *frun,int start_pos,int number){
	int i;
	for(i=0;i<number;i++){
		input[start_pos+i]=INT_MAX;
	}
	fread(input+start_pos,sizeof(int),number,frun);
	/*if(start_pos==0){
	printf("DEBUG: condition \n");
	for(int i=0;i<number;i++){
		printf("%d\t",input[i]);
	}
	printf("\n\nHI Abhash\n");
	}*/
}

void basicSortFile(FILE *finput, FILE *foutput){
	int i=0,j;
	//char **flist;
	int input[BUCKET];
	int output[BUCKET];
	fseek(finput,0,SEEK_END);
	int size = ftell(finput);
	size = size/sizeof(int);
	int number_of_runs = size/BUCKET;
	int rem_number = size%BUCKET;
	if(rem_number>0)
		number_of_runs++;

	FILE *fruns[number_of_runs];
	printf("number of key %d\n",size);
	//flist = (char **)malloc(sizeof(char *)*(size/1000));
	
	fseek(finput,0,SEEK_SET);
	for(;i<number_of_runs;i++){
		char file_name[50];
		//char *file_name = (char*)malloc(sizeof(char)*NAME_SIZE);
		sprintf(file_name,"input.bin.%03d",i);
		fread(input,sizeof(int),BUCKET,finput);
		//this is to fill the remaining number to set INT_MAX in extra run
		if(i==number_of_runs-1 && rem_number>0){
			//printf("DEBUG:creating extra run\n");
			for(j=rem_number;j<BUCKET;j++){
				input[j] = INT_MAX;
			}
		}
		qsort(input,BUCKET,sizeof(int),comp_a);
		fruns[i] = fopen(file_name,"wb+");
		fseek(fruns[i],0,SEEK_SET);
		fwrite(input,sizeof(int),BUCKET,fruns[i]);
		//clearerr(fruns[i]);
		fseek(fruns[i],0,SEEK_SET);
		//flist[i] = (char*) malloc(sizeof(char)*NAME_SIZE);
		//memcpy(flist[i],file_name,sizeof(char)*NAME_SIZE);
		//fclose(ftemp);
	}
	
	int number_from_each_run = BUCKET/number_of_runs;
	//Read Initially number_from_each_run from each runs
	for(j=0;j<number_of_runs;j++){
		get_number_from_runs(input,fruns[j],j*number_from_each_run,number_from_each_run);
	}
	//Loop on all the number until you have merged all the numbers.
	int number_merged_so_far=0,min_index,output_index=0;
	while(number_merged_so_far<size){
		int min = INT_MAX;
		for(i=0;i<BUCKET;i++){
			if(input[i] < min){
				min_index=i;
				min = input[i];
			}	
		}
		
		if(input[min_index]!=INT_MAX){
			//then store in output buffer, if output buffer is full,flush it containt to output file
			//then check whether this was the last value from this run file,if it is then fetch few more values
			output[output_index++] = input[min_index];
			if(output_index==BUCKET){
				fwrite(output,sizeof(int),BUCKET,foutput);
				//printf("DEBUG:writting to file\n");
				output_index=0;
			}
			input[min_index]= INT_MAX;
			//This part of checking whether all the number from particular run is exhausted or not
			if((number_from_each_run-(min_index%number_from_each_run))==1){
				int runs_id = min_index/number_from_each_run;
				get_number_from_runs(input,fruns[runs_id],runs_id*number_from_each_run,number_from_each_run);
			}
		} else {
			//flush all the values of output to file and then break
			if(output_index > 0){
				fwrite(output,sizeof(int),output_index,foutput);
			}
			break;
		}
	}//End of while
	printf("Debug: Number of runs %d, remiainder runs %d\n",number_of_runs,rem_number);
	/*Close input file run file descriptors*/
	for(i=0;i<number_of_runs;i++){
		fclose(fruns[i]);
	}
//return i;
}

int main(int argc,char *argv[]){
	if(argc==4){
		char *mode = argv[1];
		char *input_file = argv[2];
		char *output_file = argv[3];
		FILE *finput = fopen(input_file,"rb");
		FILE *foutput = fopen(output_file,"wb+");
		struct timeval start,end,fin;
		if(finput==NULL){
			printf("Error: Input files doesn't exist\n");
		}
		gettimeofday(&start,NULL);
		if(strcmp(mode,"--basic")==0){
			//read the data from input and sort the result and store in sub-file
			//get the input file size
			basicSortFile(finput,foutput);
			/*for(int i=0;i<size/1000;i++){
				printf("%s\n",flist[i]);
			}*/
		} else if(strcmp(mode,"--multistep")==0){
		
		} else if(strcmp(mode,"--replacement")==0){
		
		} else {
			printf("Unknown Option\n");	
		}
		fclose(finput);
		fclose(foutput);
		gettimeofday(&end,NULL);
		timersub(&end,&start,&fin);
		printf( "Time: %ld.%06ld\n", fin.tv_sec, fin.tv_usec );
	} else {
		printf("Usage:<exe> <merge-mode> <input> <output>\n");
	}	
	return 0;
	
}
