//#define True 1
//#define False 0

#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>

void main(int argc, char *argv[])
{
	DIR *d;
	struct dirent *dir;
	FILE *fp;			//file
	char* temp = argv[1];		//used for string
	struct stat fileStat;
	struct passwd *pw=getpwuid(getuid());	//passwd struct
	
	int flag = false
	
	char* check = argv[0];
	while(*check)
	{
		
	}
	
	if(argc == 1)
	{
		d = opendir(".");
	}else
	{
		char *name = pw->pw_dir;	//gets home directory
		int size1 = sizeof(name)/sizeof(name[0]);
		int size2 = sizeof(temp)/sizeof(temp[0]);
		int max = size1+size2+1;
		
		char arr[max];
		arr[0] = '\0';
		
		strcat(arr, name);
		strcat(arr, "/");
		strcat(arr, temp);
		d = opendir(arr);
	}
	if(d)
	{
		while((dir = readdir(d)) != NULL)
		{
			if(stat(dir->d_name,&fileStat) < 0)
			

			//prints permissions
			printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
			printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
			printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
			printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
			printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
			printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
			printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
			printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
			printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
			printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
			
			//prints filename	
			printf(" %s\n", dir->d_name);
		}
		closedir(d);
	}
}
