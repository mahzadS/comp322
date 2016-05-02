#include <stdio.h>

void filecopy(FILE*);

void main(int argc, char *argv[])
{
	FILE *fp, *fopen();
	if(argc == 1)
	{
		printf("No arguments passed\n");
	}else
	{
		while(--argc > 0)
		{
			if((fp = fopen(*++argv, "r")) == NULL)
			{
				printf("cat: can't open %s\n", *argv);
			}else
			{
				filecopy(fp);
				fclose(fp);
			}
		}
	}
}

void filecopy(FILE *fp)
{
	int c,d;
	int i = 1;
	c=getc(fp);
	printf("%d\t",i++);
	while((d = getc(fp)) != EOF)
	{	
		putc (c, stdout);
		if(c=='\n' && d!=EOF)
			printf("%d\t",i++);
		c=d;
	}
	printf("\n\n");
}
