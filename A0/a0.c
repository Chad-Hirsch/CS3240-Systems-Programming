/*
Chad Hirsch
CS 3240
01/12/2018
Assignment 0
*/

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "apue.h"

	#define BUFFSIZE 512
	#define _BSD_SOURCE 
	typedef struct song{
	    char *artist;
	    char *title;
	    char *albumName;
	    float duration;
	    int yearRealeased;
	    double hotttness;
	} song;


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


	int main(){
		FILE *songStream;
		int count = 0;
		int size = 100;

		song** allSongsArray = malloc(size * sizeof(song*));

		songStream = fopen("SongCSV.csv", "r");	/* fopen() the file, r Opens an existing text file 							*  for reading purpose. */

		if(songStream == NULL){
			printf("Unable to open file");  //err_sys function prints an informative
			exit(1);			//* message describing what type of error was 
						 	//encountered */

		}
		else{					// I/O operations
			printf("Opened File \n\n");

			char buffer[BUFFSIZE + 1];

			fgets(buffer, BUFFSIZE, songStream);
		
			while((fgets(buffer, BUFFSIZE, songStream)) != NULL){	/* fgets()-- The fgets
										* function, for example, 										* reads an entire line. */

			song *newSong = malloc(sizeof(song));

			char *line = buffer;
			int index = 0;
			char *Token;
		

			while ((Token = my_strtok(line,","))) {
				line = NULL;    // line needs to be set to NULL here, read 
						// the strtok/strsep documentation for details

				switch (index)
				{
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

			allSongsArray[count++] = newSong;     // store pointer to new song in array

			if(count == size){
				size = size + 100;
				allSongsArray = realloc(allSongsArray, size * sizeof(song*));
			}
		    }
		     fclose(songStream);		// fclose()
	     }
	//Sort the Struct alphabetically by Title
		int i, j, temp;
			int num_items = count;
			for (j = 0; j < num_items - 1; j++) 
			{
			     for (i = 0; i < num_items - 1; i++) 
			     {
				    if (strcasecmp(allSongsArray[i]->title,allSongsArray[i + 1]->title ) > 0) 
				    {
					temp = allSongsArray[i];
					allSongsArray[i] = allSongsArray[i+1];
					allSongsArray[i+1] = temp;
				    }
			     }
	}

		// display all songs read (count is the number of songs)
		/*for (int i = 0; i < count; i++)
		{
			fprintf(stdout, "%s, %s, %s, %f, %d, %f \n\n", 
			allSongsArray[i]->title, allSongsArray[i]->artist, 
			allSongsArray[i]->albumName, allSongsArray[i]->duration, 
			allSongsArray[i]->yearRealeased, allSongsArray[i]->hotttness);
		}*/

	char sno[512];
	while ( strcmp(sno, "ZZZ")<0){
	unsigned char flag=0;
	printf("Enter title to search :\n");
	//fgets(%sno, 512, stdin);
	scanf("%[^\n]%*c", %sno);
	printf("%s\n", sno);

	for(int i=0;i<count;i++) // where count is maximum number of records
	{

	     if(strcmp(sno,allSongsArray[i]->title)==0)
	     {
		 flag=1;
		 
		fprintf(stdout, "%s, %s, %s, %f, %d, %f \n\n", 
			allSongsArray[i]->title, allSongsArray[i]->artist, 
			allSongsArray[i]->albumName, allSongsArray[i]->duration, 
			allSongsArray[i]->yearRealeased, allSongsArray[i]->hotttness);
		 break;
	     }
	} // end of for loop
	if(0==flag) { printf("\nSorry !!! Record not found with title : %s\n",sno); }


	}


	{
		return 0;
		}

	}





