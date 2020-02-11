/*
Chad Hirsch
CS 3240
01/12/2018
Assignment 2
*/

#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>


void displayDirectory(char *dir, int fileDepth)
{
	DIR *directoryPointer; //DIR -- A type representing a directory stream.
	struct dirent *entry;
	struct stat statbuf;
	int indent = fileDepth*4;

	if((directoryPointer = opendir(dir)) == NULL) {
		fprintf(stderr,"This directory cannot be opened : %s\n", dir);
		return;
	}
	
	/*chdir() function shall cause the directory named by the 
	*pathname pointed to by the path argument to become 
	*the current working directory. */
	chdir(dir);
    
	/* readdir() returns a pointer to an object of 
	*type struct dirent. When an error is encountered, 
	*a null pointer is returned and errno is set to 
	*indicate the error. */
	while((entry = readdir(directoryPointer)) != NULL) {
		/*Upon successful completion, lstat() shall return 0. 
		*Otherwise, it shall return -1 and set errno to 
		*indicate the error.*/
		lstat(entry->d_name,&statbuf); 
		/*The S_ISDIR macro returns non-zero if the file is a directory.*/
		if(S_ISDIR(statbuf.st_mode)) {/* st_mode member contains meaningful 							*information, which can be used with the 
						*file type macros*/

			/*The character array d_name is of unspecified size, 
			*but the number of bytes preceding the terminating 
			*null byte will not exceed {NAME_MAX}.*/

			/* In this case we found a directory,
                        * however we ignore . and .. */
			if(strcmp(".",entry->d_name) == 0 || 
				strcmp("..",entry->d_name) == 0)
				continue;
				printf("%*s%s/\n",indent,"",entry->d_name);
				/* We recurse be going to a new level of indentation*/
				displayDirectory(entry->d_name,fileDepth+1);
			}
			else printf("%*s%s\n",indent,"",entry->d_name);
    		}
	chdir("..");
	closedir(directoryPointer);
	}

/*  Now we move onto the main function.  */

int main(int argc, char* argv[])
{
	char *startFile, presentWorkingDirectory[2]=".";
    
	if (argc != 2)
		startFile=presentWorkingDirectory;
	else
		startFile=argv[1];

    printf("The Directory to be scanned is: %s\n",startFile);
    displayDirectory(startFile,0);
    printf("\n The search of the Directory has finished.\n");

    return 0;
}
