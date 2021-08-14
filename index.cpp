// testing1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define MAX_LEN 256
#define SAMPLE_SIZE 100

/*
This function find the dc shift then adjust the data value accordingly and at the same time also normalize the data. The input is from txt file and 
output is also txt file. we are saving the data to file because we want to save memory issues.
*/
int DC_Shift_and_Normalization(){

	char buffer[MAX_LEN];
	
	FILE* file = fopen ("yes_no_data.txt", "r");

	long long int abs_max = -100000, dc_shift = 0, count=0, x=0;

	int i=0;
	while (fgets(buffer, MAX_LEN - 1, file))
    {	
		i++;
		if(i<=5){
		   continue;
		}

		//ignoring first 10 frame as it may contain noise.
		if(i<1000){
			continue;
		}
        // Remove trailing newline
        buffer[strcspn(buffer, "\n")] = 0;
		
		x = atoi(buffer);
		
		//calcuating abs max  which will help in normalization of the data.
		if(abs(x)>abs_max){
			abs_max = abs(x);
		}    

		//for dc shift
		count++;
		dc_shift += x;
    }
	dc_shift = dc_shift/ count;
    fclose(file);

	printf("DC Shift is %lld and max value of array is %lld for normalization. \n",dc_shift,abs_max);
	FILE* fp1 = fopen ("yes_no_data.txt", "r");
	FILE* fp2 = fopen ("yes_no_data_normalized.txt", "a");

	i=0;x=0;
	while (fgets(buffer, MAX_LEN - 1, fp1))
	{
		i++;
		if(i<=5){
		   continue;
		}

		//ignoring first 10 frame as it may contain noise.
		if(i<1000){
			continue;
		}

        // Remove trailing newline
        buffer[strcspn(buffer, "\n")] = 0;
		
		x = atoi(buffer);

		x = (((x - dc_shift)*5000)/ abs_max);

		fprintf(fp2, "%lld\n", x);
	
	}

	fclose(fp1);
	fclose(fp2);
	
	return 1;
}


int _tmain(int argc, _TCHAR* argv[])
{
		
	
	
	char buffer[MAX_LEN];
	
	long long int ste=0 , zcr=0, x=0, index_cnt=0, prev_x = 0;
    long long int temp_var;
	DC_Shift_and_Normalization();
	
	FILE* file = fopen ("yes_no_data_normalized.txt", "r");

	// -1 to allow room for NULL terminator for really long string
    while (fscanf(file,"%lld\n",&temp_var)!= EOF)
    {	

        // Remove trailing newline
		prev_x = x;
		x = temp_var;
		
		if(index_cnt==100){
			FILE *fw = fopen("yes_no_data_attr.txt", "a");
			ste = ste/100;
			fprintf(fw, "%lld %lld\n", zcr, ste);
			index_cnt = 0;
			ste = 0;	
			zcr= 0;
			fclose(fw);
			continue;
			
		}		

		index_cnt++;
		ste = ste + (x*x);
		if(x*prev_x < 0){
			zcr++;
		}
        
    }
		
    fclose(file);
	

	//second part --calculating threshold for noise.
	
	long long int avg_ste = 0 , avg_zcr = 0;
	zcr=0;
	ste=0;
	FILE *temp = fopen("yes_no_data_attr.txt", "r");
	char buf[100];
	int count=0;
	while (fscanf(temp,"%lld %lld\n",&zcr, &ste)!= EOF){
		count++;
		avg_ste += ste;
		avg_zcr += zcr;
		if(count==25){
			break;
		}
		
	}
	avg_ste = avg_ste/count;
	avg_zcr = avg_zcr/count;
    printf("[Ambient Noise] AVG ZCR : %lld AVG STE : %lld\n",avg_zcr ,avg_ste);    
	fclose(temp);
	
	/*
	We get following outcome from above code for example -- for the noise
	Threshold Silent:
	- zcr: 28
	- ste: 148
	*/

	// step3 - starting the main logic--------------------------------------------------------------------------------
	
	int AVG_STE_SILENCE = avg_ste;
	int THRESHOLD_SILENCE = 20* AVG_STE_SILENCE;
	
	int arr_zcr[100000], arr_ste[100000];

	FILE *fp3 = fopen("yes_no_data_attr.txt", "r");
	int temp1=0, temp2=0;
	int count_yes=0;

	while (fscanf(fp3,"%lld %lld\n",&temp1, &temp2)!= EOF){
		arr_zcr[count_yes] = temp1;
		arr_ste[count_yes] = temp2;
		count_yes++;
	}
	fclose(fp3);
	int start_arr[100],end_arr[100],output[100], pointer=0;
	int start_index =0, end_index =0,k;

	//this loop will help us in deciding the start and end marker. and we store it in start_arr and end_arr respectively.
	for(int i=10; i<count_yes-10; i++){
		if(start_index==0 && arr_ste[i]< THRESHOLD_SILENCE && arr_ste[i+1] > THRESHOLD_SILENCE){
			for(k=1; k<5; k++){
				if(arr_ste[i+k]>THRESHOLD_SILENCE){
					continue;
				}else{
					break;
				}
			}
			if(k==5){
				start_index = i+1;
				k=0;
			}
		} 
		if(start_index!=0 && arr_ste[i]< THRESHOLD_SILENCE && arr_ste[i-1] > THRESHOLD_SILENCE){
			for(k=1; k<5; k++){
				if(arr_ste[i+k]< THRESHOLD_SILENCE){
					continue;
				}else{
					break;
				}
			}
			if(k==5){
				end_index = i-1;
				start_arr[pointer] = start_index;
				end_arr[pointer] = end_index;
				pointer++;
				start_index =0;
				end_index = 0;
			}
			
		}
	}
	
	
	//now main coding logic-- deciding yes or no
	for(int l=0; l<pointer; l++){
		start_index = start_arr[l];
		end_index = end_arr[l];
		int temp_index = start_index + 0.6*(end_index-start_index);

		long long int avg_zcr_before=0, avg_zcr_after=0;
		int i;

		for(i= start_index; i< temp_index ; i++){
			avg_zcr_before += arr_zcr[i];
		}
		avg_zcr_before = avg_zcr_before/ (temp_index- start_index);

	
		for(i= temp_index; i< end_index ; i++){
			avg_zcr_after += arr_zcr[i];
		}
	
		avg_zcr_after = avg_zcr_after/ (end_index - temp_index);
		printf("avg_zcr_before: %lld, avg_zcr_after: %lld ", avg_zcr_before, avg_zcr_after);

		//main logic to decide.
		if(avg_zcr_after > avg_zcr_before && avg_zcr_after > 50){
			output[l] = 1;
			printf("Outcome: yes\n");
		}else{
			output[l] = 0;
			printf("Outcome: No\n");
		}
	
	}
	

	//------------------------------------------------------------
	char *ptr;
	printf("\n\n\n");
	for(int i=0; i<pointer; i++){

		if(output[i]==1){
			printf("%d %d Outcome: Yes\n", start_arr[i], end_arr[i], output[i]);
		}else{
			printf("%d %d Outcome: No\n", start_arr[i], end_arr[i], output[i]);
		}
		
	}
	
	getchar();
	return 0;
}

