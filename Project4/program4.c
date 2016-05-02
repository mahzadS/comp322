#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

void main(int argc, char *argv)
{
	DIR *d;
	struct dirent *dir;
	if(argc == 1)
	{
		printf("here\n");
		d = opendir(".");
	}else
	{
		char *name = "~/"+argv[1];
		printf("%s\n", &argv[1]);
		d = opendir(name);
	}
	if(d)
	{
		while((dir = readdir(d)) != NULL)
		{
			struct stat fileStat;
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
