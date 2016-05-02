#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/types.h>

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
	char *files_from = NULL;
	
	int pd[2];
	pipe(pd);
	
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
		if((fp = fopen(argv[optind], "r")) == NULL)
		{
			printf("wc: can't open %s\n", argv[i]);
		}else
		{
			filecopy(fp);
			print(argv[optind],fi);
			fclose(fp);
			count++;
		}
		optind++;
	}
	
	if(count>1)
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
			printf("%3d ",foo.lines);
		}
		if(po.w)
		{
			printf("%3d ",foo.words);
		}
		if(po.c)
		{
			printf("%3d ", foo.characters);
		}
		if(po.L)
		{
			printf("%4d ", foo.max_line_length);
		}
	}else
	{
		printf("%3d %3d %3d ", foo.lines,foo.words,foo.characters);
	}
	printf("%s\n", name);
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

	total.max_line_length += fi.max_line_length;
	total.lines += fi.lines;
	total.words += fi.words;
	total.spaces += fi.spaces;
	total.characters += fi.characters;
}
