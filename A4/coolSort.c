/*
*Chad Hirsch
*/

#include "apue.h"
#include <dirent.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
void * fileSortFxn(void *x);

#define BUFFER 512

typedef struct {
	char *name;
	char *pass;
	char *blood;
	char *domain;
	int index;
	int sizeOfFile;  //allows me to nicely pass around my size of arrays useful for sorting

} dataInfo;

struct timeval start, end;

typedef struct {
	dataInfo sortedRecord;
} collectedArrays;


dataInfo* lineSplitter(char* line);
void printOutCompleateStruct(dataInfo**completeDataInfoStruct, int infoNumberData);
int compare(const void *check1, const void *check2);
void mergeArrays(dataInfo ***record, int count);
dataInfo** mergeTwoTogether(dataInfo **arrayOne, dataInfo** arrayTwo);
void writeStructToFile(dataInfo** completeDataInfoStruct, int infoNumberData);

//Main function

int main(int argc, char **argv) {
//Found how to use gettimeofday() @ http://www.cs.loyola.edu/~jglenn/702/S2008/Projects/P3/time.html
	gettimeofday(&start, NULL);
	DIR *d;
	struct dirent *dir;

	d = opendir(argv[1]);
	int counter = 0; //number of files...which means number of threads...which means size of 2d array
	int i, err;
	//** handles getting the files names **
	char **files = malloc(255 * sizeof(char*));
	if (d) {
		while ((dir = readdir(d)) != NULL) {
			if (strstr(dir->d_name, ".") == NULL) {
				counter++;
				char temp[512];
				temp[0] = '\0';
				strcpy(temp, argv[1]);
				strcat(temp, "/");  //segfaults if this inst here id dir is entered w/out last slash
				strcat(temp, dir->d_name); //makes sure i can pass full fill path to thread

				files[counter - 1] = malloc(255 * sizeof(char*));
				strcpy(files[counter - 1], temp);

			}
		}

	
		closedir(d);
		gettimeofday(&end, NULL);
		printf("%ld microseconds \n", ((end.tv_sec * 1000000 + end.tv_usec)
		 - (start.tv_sec * 1000000 + start.tv_usec)));
		
	}
	else {
		printf("You hav entered a Directory that is not valid. Please enter the directory path as a command line arg. Ex. /home/chad/cs3240/a4/coolSort ./big-data/ \n");
	}

	//start of threading stuff 
	pthread_t tid[counter];
	for (i = 0; i < counter; i++) {

		err = pthread_create(&tid[i], NULL, fileSortFxn, files[i]);
	}

	dataInfo **data[counter];

	for (i = 0; i < counter; i++) {
		err = pthread_join(tid[i], (void**) &data[i]);
	}
	//end of threading stuff

	mergeArrays(data, counter);
	for(i=0; i <counter; i++){
		free(files[i]);
	}
	free(files);

}

//This will open, read, populate and sort a struct

void * fileSortFxn(void *x) {
	char* file;
	file = (char*)x;
	FILE *fp;
	fp = fopen(file, "r");

	int indexLocation = 0;
	int arraySize = BUFFER;
	dataInfo** recordArray = malloc(arraySize * sizeof(dataInfo*));


	char line[512];
	while (fgets(line, 512, fp ) != NULL) {
		if (indexLocation >= arraySize ) {
			arraySize += BUFFER;
			recordArray = realloc(recordArray, arraySize * sizeof(dataInfo*));
		}

		recordArray[indexLocation] = lineSplitter(line);
		indexLocation++;
	}

	int closingFile = fclose(fp);


	qsort(recordArray, indexLocation, sizeof(dataInfo*), compare);
	recordArray[0]->sizeOfFile = indexLocation;

	pthread_exit(recordArray);
}

//This takes a line and splits it into a struct

dataInfo* lineSplitter(char* line) {
	char* token;
	char* ptToken;

	int tokenNum = 1;
	dataInfo* returnPointer = malloc(sizeof(dataInfo));
	token = strtok_r(line, ",", &ptToken);
	char* temp;

	while (token != NULL && tokenNum < 6) {

		switch (tokenNum) {
		case 1: //name
			returnPointer->name = malloc(strlen(token) + 1);
			strncpy(returnPointer->name, token, strlen(token) + 1);
			break;

		case 2: //pass
			returnPointer->pass = malloc(strlen(token) + 1);
			strncpy(returnPointer->pass, token, strlen(token) + 1);
			break;
		case 3:  //blood
			returnPointer->blood = malloc(strlen(token) + 1);
			strncpy(returnPointer->blood, token, strlen(token) + 1);
			break;
		case 4:  //domain
			returnPointer->domain = malloc(strlen(token) + 1);
			strncpy(returnPointer->domain, token, strlen(token) + 1);
			break;
		case 5: //int index

			returnPointer->index = atoi(token);
			break;
		default:
			break;

		}

		tokenNum++;
		token = strtok_r(NULL, ",", &ptToken);
	}
	return returnPointer;
}

//This will print the entire struct

void printOutCompleateStruct(dataInfo** completeDataInfoStruct, int infoNumberData)
{
	int i;
	for (i = 0; i < infoNumberData; i++)
	{
		printf("%s, %s, %s, %s, %d \n", completeDataInfoStruct[i]->name, completeDataInfoStruct[i]->pass,
		       completeDataInfoStruct[i]->blood, completeDataInfoStruct[i]->domain, completeDataInfoStruct[i]->index);
	}
}

//compare function to be used with qsort

int compare(const void *check1, const void *check2) {
	dataInfo *element1 = *(dataInfo **) check1;
	dataInfo *element2 = *(dataInfo **) check2;

	if (element1->index < element2->index) {
		return -1;
	}
	else if (element1->index > element2->index ) {
		return 1;
	}
	else return 0;
}

//gets two arrays from 2d array and sends then to merge sort

void mergeArrays(dataInfo ***record, int count) {
	int i;
	dataInfo **merged = malloc(sizeof(record));
	merged = mergeTwoTogether(record[0], record[1]);
	int mergeSize = 0;


	for (i = 2; i < count; i++) {
		merged = mergeTwoTogether(merged, record[i]);

	}
	int structSize = merged[0]->sizeOfFile;
	writeStructToFile(merged, merged[0]->sizeOfFile);
	for(i=0; i <structSize; i++){
		free(merged[i]->name);
		free(merged[i]->pass);
		free(merged[i]->blood);
		free(merged[i]->domain);
		free(merged[i]);
	}
	free(merged);
}


//Merge sort two arrays of my struct

dataInfo** mergeTwoTogether(dataInfo **arrayOne, dataInfo** arrayTwo) {
	int i, j, k;
	j = 0;
	k = 0;
	int arraySize1 = arrayOne[0]->sizeOfFile;
	int arraySize2 = arrayTwo[0]->sizeOfFile;
	dataInfo** combinedArray = malloc(sizeof(dataInfo) * (arraySize1 + arraySize2));

	//printf("\narray size one is: %d\n", arraySize1);
	//printf("array size two is: %d\n", arraySize2);

	for (i = 0; i < arraySize1 + arraySize2;) {
		if (j < arraySize1 && k < arraySize2) {
			if (arrayOne[j]->index < arrayTwo[k]->index) {
				combinedArray[i] = arrayOne[j];
				j++;
			}
			else {
				combinedArray[i] = arrayTwo[k];
				k++;
			}
			i++;
		}
		else if (j == arraySize1) {
			for (; i < arraySize1 + arraySize2;) {
				combinedArray[i] = arrayTwo[k];
				k++;
				i++;
			}
		}
		else {
			for ( ; i < arraySize1 + arraySize2;) {
				combinedArray[i] = arrayOne[j];
				j++;
				i++;
			}
		}
	}
	combinedArray[0]->sizeOfFile = arraySize1 + arraySize2;
	return combinedArray;

}

//Just writes the sorted array to the file

void writeStructToFile(dataInfo** completeDataInfoStruct, int infoNumberData)
{
	FILE *f = fopen("sorted.yay", "w");
	if (f == NULL) {
		printf("Error opening file!\n");
		exit(1);
	}
	int i;
	for (i = 0; i < infoNumberData; i++)
	{
		fprintf(f, "%s, %s, %s, %s, %d \n", completeDataInfoStruct[i]->name, completeDataInfoStruct[i]->pass,
		        completeDataInfoStruct[i]->blood, completeDataInfoStruct[i]->domain, completeDataInfoStruct[i]->index);
	}

	fclose(f);
}
