/*Abhash Jain
CSC541- Assignment-3 Disk Based Merge Sort*/

#include<stdio.h>
#include<sys/time.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#define BUCKET 1000		//BUCKET size
#define NAME_SIZE 14 //USED for file name size, need to reinvestigate for SUPER file
#define SUPER 15	//Number of intermediate file to combin to make super run
#define HEAP 750	//Heap size
#define BUFFER 250	//Buffer size, Based on Bucket value as 1000

//int heap_size =0;	//global variable indicate the heap size
int comp_a(const void *a, const void *b){
    const int *l = (const int *)a;
	const int *r = (const int *)b;
	return *l - *r;
}


void swap(int x, int y)
{
    int temp = x;
	x = y;
 	y = temp;
}
void pswap(int *x,int *y){
	int temp = *x;
	*x=*y;
	*y = temp;
}

int left(int i) { return (2*i + 1); }

int right(int i) { return (2*i + 2); }

void Heapify(int *input,int n,int i)
{
    int l = left(i);
    int r = right(i);
    int smallest = i;
    if (l < n && input[l] < input[i])
        smallest = l;
    if (r < n && input[r] < input[smallest])
        smallest = r;
    if (smallest != i)
    {
        pswap(&input[i], &input[smallest]);
        Heapify(input,n,smallest);
    }
}

void BuildHeap(int *input,int n){
	for(int i=(n/2)-1;i>=0;i--){
		Heapify(input,n,i);
	}
}

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

/*Function to handle Multi-Step Merge sort*/
void MultiSortFile(FILE *finput,FILE *foutput){
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
		fseek(fruns[i],0,SEEK_SET);
		//flist[i] = (char*) malloc(sizeof(char)*NAME_SIZE);
		//memcpy(flist[i],file_name,sizeof(char)*NAME_SIZE);
		//fclose(ftemp);
	}
	//Basic Run Creation done, Now work on merging them in group of SUPER

	int number_of_super_runs = number_of_runs/SUPER;
	int rem_super_run = number_of_runs%SUPER;
	if(rem_super_run>0){
		number_of_super_runs++;
	}
	for(i=0;i<BUCKET;i++){
		input[i]=INT_MAX;
	}
	//int number_from_each_super_run = BUCKET/number_of_super_runs; //66
	FILE *super[number_of_super_runs];
	for(i=0;i<number_of_super_runs;i++){
		char file_name[50];
		sprintf(file_name,"input.bin.super.%03d",i);
		super[i]=fopen(file_name,"wb+");
		fseek(super[i],0,SEEK_SET);
		//There might be a case that last super run will have less number of basic run
		int super_count =0;
		if(rem_super_run>0 && (number_of_super_runs-1==i)){
			super_count = rem_super_run;
		} else{
			super_count = SUPER;	
		}
		//Now apply the same logic which is applied in basic sorting
		int number_from_each_run = BUCKET/super_count;
		
		//Read from each subset of run
		for(j=0;j<super_count;j++){
			get_number_from_runs(input,fruns[i*SUPER+j],j*number_from_each_run,number_from_each_run);
		}
		int min_index=0,merge_so_far=0,output_index=0;
		while(merge_so_far<(super_count*BUCKET)){
			int min =INT_MAX;
			for(j=0;j<BUCKET;j++){
				if(input[j]<min){
					min_index=j;
					min = input[j];
				}
			}
			if(input[min_index]!=INT_MAX){
				output[output_index++] = input[min_index];
				if(output_index==BUCKET){
					fwrite(output,sizeof(int),BUCKET,super[i]);
					output_index =0;
				}
				input[min_index] = INT_MAX;
				if((number_from_each_run-(min_index%number_from_each_run))==1){
					//read more number for particular sub run
					int run_id= min_index/number_from_each_run;
					get_number_from_runs(input,fruns[i*SUPER+run_id],run_id*number_from_each_run,number_from_each_run);
				}
			} else {
				//This is end of run
				if(output_index>0){
					fwrite(output,sizeof(int),output_index,super[i]);
				}
				break;
			}
		}//End of while
	}//End of for loop to create super run
	
	//seek to first location of all the super runs
	for(i=0;i<number_of_super_runs;i++){
		fseek(super[i],0,SEEK_SET);
	}

	//2nd time
	int number_from_each_super = BUCKET/number_of_super_runs;
	//Read Initially number_from_each_run from each runs
	for(j=0;j<number_of_super_runs;j++){
		get_number_from_runs(input,super[j],j*number_from_each_super,number_from_each_super);
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
			if((number_from_each_super-(min_index%number_from_each_super))==1){
				int runs_id = min_index/number_from_each_super;
				get_number_from_runs(input,super[runs_id],runs_id*number_from_each_super,number_from_each_super);
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
	for(i=0;i<number_of_super_runs;i++){
		fclose(super[i]);
	}
}

void Replacement(FILE *finput,FILE *foutput){
	/*TODO:
	Read 750 Values for Heap and read input buffer 
	Initially that can be done by reading total length and do the markation
	Later fread only happen for the size of 250 buffer whenever it is empty.
	Call BuildHeap method for 750 values and use the step 3, Now 
	Next to balance the heap call only heapify method for that top most root element, that is always replaced.
	Keep on doing this until your initial heap become empty, As your Inital heap become empty or Number is finished from 
	input.bin, if it is first case then secondary heap would have filled the values which are not moved to output.
	use them to rebuild the heap.
	Currently I am creating the output intermediate file is size of 1000, as it is not mentioned
	*/
	int input[BUCKET];
	int output[BUCKET];
	fseek(finput,0,SEEK_END);
	int total_keys = ftell(finput);
	total_keys = total_keys/sizeof(int);
	int total_runs = total_keys/BUCKET;
	int rem_runs = total_keys%BUCKET;
	if(rem_runs>0)
		total_runs++;
	FILE *fruns[total_runs];		//Here it is assumed that always ouput buffer flush to a new file.
	fseek(finput,0,SEEK_SET);
	int heap = fread(input,sizeof(int),HEAP,finput);
	int buffer = fread(input+HEAP,sizeof(int),BUFFER,finput);
	int h_start = 0, h_end=heap-1,output_index=0,b_start = HEAP,b_end=HEAP+buffer-1,run_count=0,i=0,j=0;
	int s_heap=0,number_processed=0;
	//have to decide on end crieteria
	//one possibilty until there is no more fread return count and buffer and secondary heap is empty
	while(number_processed!=total_keys){		//TODO:have to change this condition
		//create file name for this file
		char file_name[50];
		sprintf(file_name,"input.bin.%03d",run_count);
		fruns[run_count] =fopen(file_name,"wb+");
		
		//create a run file using the logic of replacement
		while(1){
			BuildHeap(input,h_end);
			//Apply the condition 3
			output[output_index++]=input[h_start];
//filled:		
			if(b_start<=b_end){
				if(input[h_start]<=input[b_start]){
					//then replace H1 with B1
					input[h_start]= input[b_start];
					b_start++;
				}else{
					//replace H1 with End of heap and replace End of heap with B1
					pswap(&input[h_start],&input[h_end]);
					pswap(&input[h_end],&input[b_start]);
					s_heap = h_end;
					b_start++;
					h_end--;
				}
			} else{
				pswap(&input[h_start],&input[h_end]);
				h_end--;
			}
			if(b_start>b_end) {
				//buffer is empty,
				//option 1- read more data from input.bin
				//or consume all the data in primary heap and then copy the data from secondary heap to primary heap
				int d = fread(input+HEAP,sizeof(int),BUFFER,finput);
				b_start = HEAP;
				b_end = HEAP+d-1;
				//goto filled;
			}
			if(output_index==BUCKET){
				//output buffer is full, flush it to file
				fwrite(output,sizeof(int),output_index,foutput);
				number_processed +=output_index; 
				output_index=0;
			}
			//now check whether heap is empty
			if(h_start>h_end){
				//flush the remaining data and break from this loop,
				//but copy the data 
				if(output_index>0){
					fwrite(output,sizeof(int),output_index,foutput);
					number_processed+=output_index;
					output_index=0;
				}
				//copy the data to start of heap
				int t = input[h_end+1];
				for(int i=HEAP-1;i>=s_heap;i--){
					input[++h_end]= input[i];
				}
				input[h_end]=t;
				break;
			}
			
		}//Inner while 
		run_count++;
	}//outer While
	
	//DONE with replacement only merge the sorted runs produced by replacement
	//set the seek for all open file
	total_runs = run_count;
	for(i=0;i<total_runs;i++){
		fseek(fruns[i],0,SEEK_SET);
	}
	for(int i=0;i<BUCKET;i++){
		input[i] = INT_MAX; 
	}
	//Do the merge of total_runs
	int number_from_each_run = BUCKET/total_runs;
	for(j=0;j<total_runs;j++){
        get_number_from_runs(input,fruns[j],j*number_from_each_run,number_from_each_run);
    }
    //Loop on all the number until you have merged all the numbers.
    int number_merged_so_far=0,min_index;
	output_index=0;
    while(number_merged_so_far<total_keys){
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
	/*Close input file run file descriptors*/
    for(i=0;i<total_runs;i++){
        fclose(fruns[i]);
    }
}//end of function




int main(int argc,char *argv[]){
	/*int t[]={4,15,3,9,24,2};
	for(int i=6/2-1;i>=0;i--){
		Heapify(t,6,i);
	}
	for(int i=0;i<6;i++){
		printf("%d\t",t[i]);
	}*/
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
			MultiSortFile(finput,foutput);
		} else if(strcmp(mode,"--replacement")==0){
			Replacement(finput,foutput);	
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
