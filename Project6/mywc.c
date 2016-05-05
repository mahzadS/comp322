#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

struct file_info
{
	int lines;
	int max_line_length;
	int words;
	int spaces;
	int characters;
};
typedef struct file_info FILE_INFO;

struct print_options
{
	bool l;
	bool L;
	bool w;
	bool c;
};
typedef struct print_options PRINT_OPTIONS;

FILE_INFO fi,total;
PRINT_OPTIONS po;

void filecopy(FILE*);
void print(char*, FILE_INFO);

void main(int argc, char *argv[])
{
	FILE *fp, *fopen();
	po.l = po.L = po.w = po.c = false;
	total.max_line_length = 0;
	total.lines = 0;
	total.words = 0;
	total.spaces = 0;
	total.characters = 0;
	
	int i = 0;
	int optc;
	int count = 0;
	
	//new stuff
	int values[5];
	int fd[2];
	int pid;
	int ret = pipe(fd);
	
	if(ret == -1)
	{
		perror("Pipe creation failed\n");
		exit(-1);
	}
	
	if(argc == 1)
	{
		filecopy(stdin);
		print("",fi);
	}else
	{
		while ((optc = getopt(argc, argv, "clLw")) != -1)
		{
			switch(optc)
			{
				case 'c':
					po.c = true;
					break;
				case 'l':
					po.l = true;
					break;
				case 'w':
					po.w = true;
					break;
				case 'L':
					po.L = true;
					break;				
				default:
					printf("SYSTEM ERROR!\n");
					return;
			}
		}
	}
	
	if(!(po.c || po.l || po.w || po.L))
	{
		po.c = po.l = po.w = true;
	}
	
	while(argv[optind] != '\0')
	{
		pid = fork();
		if(pid == 0)
		{
			if((fp = fopen(argv[optind], "r")) == NULL)
			{
				printf("wc: can't open %s\n", argv[i]);
			}else
			{
				filecopy(fp);
				print(argv[optind],fi);
				
				values[0] = fi.lines;
				values[1] = fi.max_line_length;
				values[2] = fi.words;
				values[3] = fi.spaces;
				values[4] = fi.characters;
				
				//write values to pipe
				write(fd[1], &values, sizeof(values));
				
				fclose(fp);
				exit(0);
			}
		}else
		{
			wait(NULL);
			if(strlen(argv[optind])<= 4)
				printf("\t");
			printf("\tPID: %d\n",pid);
			//parent process read from pipe
			read(fd[0], &values, sizeof(values));
			
			total.lines 			+= values[0];
			total.max_line_length 	+= values[1];
			total.words 			+= values[2];
			total.spaces 			+= values[3];
			total.characters 		+= values[4];
		}
		optind++;
	}
	
	if(argc > 1)
	{
		print("total", total);
	}
}

void print(char *name, FILE_INFO foo)
{
	if(po.l||po.w||po.c||po.L)
	{
		if(po.l)
		{
			printf("%5d ",foo.lines);
		}
		if(po.w)
		{
			printf("%5d ",foo.words);
		}
		if(po.c)
		{
			printf("%5d ", foo.characters);
		}
		if(po.L)
		{
			printf("%5d ", foo.max_line_length);
		}
	}else
	{
		printf("%5d %5d %5d ", foo.lines,foo.words,foo.characters);
	}
	printf("%s", name);
	if(name == "total")
		printf("\n");
}

void filecopy(FILE *fp)
{
	fi.max_line_length = 0;
	fi.lines = 0;
	fi.words = 0;
	fi.spaces = 0;
	fi.characters = 0;
	int c;
	int i =0;
	int wc = 0;
	bool new_line_check = true;
	int last = -1;
	int current_line_length = 0;
	while((c = getc(fp)) != EOF)
	{
		i++;
		switch(c)
		{
			case '\n':
				new_line_check = false;
				if(current_line_length >fi.max_line_length)
				{
					fi.max_line_length = current_line_length;
				}
				current_line_length = 0;
				fi.lines++;
				if(c != last)
				{
					fi.words++;
				}
				break;
			case ' ':
				fi.words++;
				break;
		}
		current_line_length++;
		last = c;
	}
	fi.characters = i;
}
