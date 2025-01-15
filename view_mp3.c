#include "mp3.h"

//int to_LES(void *,int );
int view_mp3(char *str)
{
	FILE *fptr = fopen(str, "r");
	if(fptr == NULL)
	{
		printf("Error: Unable to open the input file\n");
		return 0;
	}
	fseek(fptr,10,SEEK_CUR);
	for(int i=0;i<6;i++)
	{
		unsigned char tag[4];
		fread(tag,1,4,fptr);
		int s;
		fread(&s,1,4,fptr);
		to_LES(&s,4);
		fseek(fptr,3,SEEK_CUR);
		char data[s];
		fread(data,1,s-1,fptr);
		data[s-1] = '\0';
		if(strcmp(tag,"TIT2") == 0)
		{
			printf("TITLE\t\t: \t%s\n",data);
		}
		else if(strcmp(tag,"TPE1") == 0)
		{
			printf("ARTIST\t\t: \t%s\n",data);
		}
		else if(strcmp(tag,"TALB") == 0)
		{
			printf("ALBUM\t\t: \t%s\n",data);
		}
		else if(strcmp(tag,"TYER") == 0)
		{
			printf("YEAR\t\t: \t%s\n",data);
		}
		else if(strcmp(tag,"TCON") == 0)
		{
			printf("CONTENT\t\t: \t%s\n",data);
		}
		else if(strcmp(tag,"COMM") == 0)
		{
			printf("COMMENTS  \t: \t%s\n",data);
		}
	}
	fclose(fptr);
	return e_success;
}
int to_LES(void *arr,int s)
{
	char temp;
	for(int i=0;i<(s/2);i++)
	{
		temp = *((char*)arr+i);
		*((char*)arr+i) = *((char*)arr+s-(i+1));
		*((char*)arr+s-(i+1)) = temp;
	}
}

