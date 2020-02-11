/*
Chad Hirsch
CS 3240
01/12/2018
Assignment 1
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "apue.h"

#define BUFFSIZE 4096
#define _BSD_SOURCE 

	
typedef struct song{
	char *artist;
	char *title;
	char *albumName;
	float duration;
	int yearRealeased;
	double hotttness;
} song;

typedef struct _index_t {
	off_t offset;
	char *name;
} index_t;


//non skip version strtok
char *my_strtok_r(char *str, const char *delims, char **store){
	char *p, *wk;
	if(str != NULL){
		*store = str;
	}
	if(*store == NULL) return NULL;
	//*store += strspn(*store, delims);//skip delimiter
	if(**store == '\0') return NULL;
	p=strpbrk(wk=*store, delims);
	if(p != NULL){
		*p='\0';
		*store = p + 1;
	} else {
		*store = NULL;
	  }
	    return wk;
	}

	char *my_strtok(char *str, const char *delims){
		static char *p;
		return my_strtok_r(str, delims, &p);
	}

	int compare(const void *a, const void *b)
	{
	
		const song *a_node = *(song **)a; 
		const song *b_node = *(song **)b;
		return (strcmp(a_node->title, b_node->title));
	}


	int main(){
		FILE *songStream;
		int count = 0;
		int size = 100;

		song** allSongsArray = malloc(size * sizeof(song*));
		songStream = fopen("MillionSongCSV.csv", "r");
		if(songStream == NULL){
			printf("Unable to open file");  
			exit(1);			

		}
		else{					// I/O operations
			printf("Opened File \n\n");

			char buffer[BUFFSIZE + 1];

			fgets(buffer, BUFFSIZE, songStream);
		
			while((fgets(buffer, BUFFSIZE, songStream)) != NULL){	

				song *newSong = malloc(sizeof(song));
				char *line = buffer;
				int index = 0;
				char *Token;
		

				while ((Token = my_strtok(line,","))) {
					line = NULL;    // line needs to be set to NULL here, read 
								// the strtok/strsep documentation for details

					switch (index){
					case 17:
						newSong->title = malloc(strlen(Token) + 1);
						strcpy(newSong->title, Token);
						break;
					case 8:
						newSong->artist = malloc(strlen(Token) + 1);
						strcpy(newSong->artist, Token);
						break;
					case 3:
						newSong->albumName = malloc(strlen(Token) + 1);
						strcpy(newSong->albumName, Token);
						break;
					case 10:
						newSong-> duration = atof(Token);
						break;

					case 18:
						newSong->yearRealeased = atoi(Token);
						break;
					case 14:
						newSong->hotttness = atof(Token);
						break;
					}
					     index++;
				}

				allSongsArray[count++] = newSong;// store pointer to new song in array

				if(count == size){
					size = size + 100;
					allSongsArray = realloc(allSongsArray, size * sizeof(song*));
			}
		}
		fclose(songStream);		// fclose()
	}


	//Remove doube quotes.
	for (int i = 0; i < count; i++){
		char *sep = allSongsArray[i]->title;
		sep++;
		sep[strlen(sep)-1] = 0;
		allSongsArray[i]->title = sep;
	}

	//Sort the Struct alphabetically by Title using qSort
	int num_items = count;
	qsort(allSongsArray, num_items, sizeof (allSongsArray), compare);


	


	int file = creat("database.bin", 0644);
		
	for (int i = 0; i < count; i++){
		
		index_t** allindex = malloc(size * sizeof(index_t*));
		index_t *newIndex = malloc(sizeof(index_t));
		char *Token1;

		char length_artist = strlen(allSongsArray[i]->artist);
		char length_title = strlen(allSongsArray[i]->title);
		char length_albumName = strlen(allSongsArray[i]->albumName);

		write(file, &length_title, sizeof(char));
		write(file, allSongsArray[i]->title, length_title);
		
		newIndex->name = malloc(strlen(Token1) + 1);
		strcpy(newIndex->name, Token1);


		write(file, &length_artist, sizeof(char));
		write(file, allSongsArray[i]->artist, length_artist);

		write(file, &length_albumName, sizeof(char));
		write(file, allSongsArray[i]->albumName, length_albumName);
		

		write(file, &allSongsArray[i]->duration, sizeof(int));

		write(file, &allSongsArray[i]->yearRealeased, sizeof(int));

		write(file, &allSongsArray[i]->hotttness, sizeof(int));

		lseek(file, 0, SEEK_SET);

	}
	

	
	{
		return 0;
	}

}





