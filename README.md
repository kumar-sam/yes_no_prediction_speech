
--------------------------------------------------------------------------
    Assignment: Detecting multiple yes/no on a recorded file with yes/no.
--------------------------------------------------------------------------


Project structure and how to run the project?

The project has mainly 3 files:
index.cpp - main project file
yes_no_data.txt - sample recorded data by sir
ReadMe - details of the project.

- To run the project, simply execute the project where index.cpp is the main file. It will internally take data
from yes_no_data.txt and it will create 2 more files and at the end, output on the console.


--------------------------------------------------------------------------
Dataset:
--------------------------------------------------------------------------
we are using a sample file of sir, which has multiple yes/no in the file. It will output the
start marker, end marker and the yes/no class of the speech.

--------------------------------------------------------------------------
Approach of Project:
--------------------------------------------------------------------------
1. first we stored the audio file in the .txt format.
2. We removed the DC value and normalized the data saved in a file.
3. we excluded the first 1000 samples(10 frames) as it was disturbance during the start saved in a file.
4. we read the data from file and compute zcr and energy for each sample(n=100) and again saved into file.
5. we read the latest file store into array.
6. we find multiple start and end marker in file and then store in the array.
7. for each of the start,end marker pair, we are running the algo and predicting yes or not and storing in array.
8. and at the end we output <start marker, end market, yes/no> on the console.
9. The algo is able to detect multiple yes/no in the file.

--------------------------------------------------------------------------
Observations:
--------------------------------------------------------------------------
1. For Yes File DC Shift is -2 and max value of array is 14763 for normalization.
2. DC Shift is -2 and max value of array is 10182 for normalization.
3. last 40 percent data:
- yes: high zcr and high energy compared to previous case
- no: low zcr and high energy in all case.

--------------------------------------------------------------------------
Result:
--------------------------------------------------------------------------
[Ambient Noise] AVG ZCR : 21 AVG STE : 41
avg_zcr_before: 16, avg_zcr_after: 55 Outcome: yes
avg_zcr_before: 12, avg_zcr_after: 62 Outcome: yes
avg_zcr_before: 16, avg_zcr_after: 57 Outcome: yes
avg_zcr_before: 16, avg_zcr_after: 59 Outcome: yes
avg_zcr_before: 2, avg_zcr_after: 2 Outcome: No
avg_zcr_before: 3, avg_zcr_after: 3 Outcome: No
avg_zcr_before: 3, avg_zcr_after: 2 Outcome: No
avg_zcr_before: 3, avg_zcr_after: 1 Outcome: No



319 404 Outcome: Yes
516 601 Outcome: Yes
725 818 Outcome: Yes
950 1039 Outcome: Yes
1224 1299 Outcome: No
1428 1502 Outcome: No
1658 1730 Outcome: No
1867 1937 Outcome: No

--------------------------------------------------------------------------
