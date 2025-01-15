#include "mp3.h"
#include "view.h"
void to_BES(void *arr,int s)
{
	char temp;
	for(int i=0;i<(s/2);i++)
	{
		temp = *((char*)arr+i);
		*((char*)arr+i) = *((char*)arr+s-(i+1));
		*((char*)arr+s-(i+1)) = temp;
	}
}
//when the tag match

int edit_main(FILE *fptr, FILE *fp, char *argv[], char *tag) 
{
	fwrite(tag, 1,4,fp); // Write the tag to the new file
	int size;
	fread(&size, 1,4,fptr);  // Read the size of the tag data
	to_LES(&size, 4); // Convert size to Little Endian format
	int len = strlen(argv[3])+1; // Calculate the length of the new data
	int new_len = len;
	to_BES(&len, 4); // Convert length to Big Endian format
	fwrite(&len,1,4,fp); // Write the new length to the file
	char flag[3];
	fread(flag, 1, 3, fptr); // Read the flags
	fwrite(flag, 1, 3, fp); // Write the flags to the new file 
	char *data = malloc(new_len*sizeof(char)); // Allocate memory for new data

	if(data = NULL)
	{
		printf("Error: Memory allocation failed\n");
		fclose(fptr);
		fclose(fp);
		return 0;
	}
	strncpy(data, argv[3],new_len-1); //copy the new data new_len-1 times, since the size contains the null also, so excluding it read the
	data[new_len-1] = '\0'; //terminate the string with NULL
	fwrite(data, 1, new_len-1,fp); //write the new data to the file
	free(data); //free allocated memory 
	fseek(fptr, size-1, SEEK_CUR); //skip size-1 bytes in original file to skip old data
	char ch;

	while(fread(&ch,1,1,fptr) > 0) // Copy remaining data to new file
	{
		fwrite(&ch, 1,1,fp);
	}
}
//when the tag doesn't match
int edit_loop(FILE *fptr, FILE *fp, char *tag)
{
	fwrite(tag, 1,4,fp); // Write the tag to the new file
	int copy_s = 0;
	fread(&copy_s,1,4, fptr); //read the size of the tag data
	fwrite(&copy_s,1,4,fp); //write the size to the new file
	char fl[3];
	fread(fl,1,3, fptr); //read the flags
	fwrite(fl,1,3,fp);//write the flags to the new file 
	to_LES(&copy_s, 4); //convert size to little endian format
	char *copy_d= malloc(copy_s * sizeof(char)); // Allocate memory for tag data
	if(copy_d== NULL)
	{
		printf("Error: Memory allocation failed\n");
		fclose(fptr);
		fclose(fp);
		return 0;
	}
	fread(copy_d, 1, (copy_s-1), fptr); // Read the tag data copy_s-1 times, since the size contains the null also, so excluding it rea
	copy_d[copy_s-1] = '\0'; //termiante the string with NULL
	fwrite(copy_d,1, copy_s-1,fp); //write the data to the new file
	free(copy_d); //free allocated memory
}

void editing(FILE *fptr, FILE *fp, char *argv[], char *tar) 
{
Label:char tag[4],str[4];
      while(fread(tag, 1,4, fptr) > 0) // Read tags from the file
      {
	      for(int i=0;i<4;i++) //copy tag to string
	      {
		      str[i] = tag[i];
	      }
	      str[4] = '\0'; //terminate the string with NULL
	      if(strcmp(str,tar) == 0) // If tag matches target
	      {
		      edit_main(fptr, fp, argv, tag); // Edit, when the tag matches
	      }
	      else //otherwise skip the size, tag, flag, content till the tag natches
	      {
		      edit_loop(fptr, fp, tag);
		      goto Label;
	      }
      }
}

int edit_opr(char *argv[])
{
	FILE *fptr = fopen("sample.mp3", "r"); //open sample file in read mode
	if(fptr== NULL)
	{
		printf("Error: Unable to open the file\n");
		return 0;
	}
	FILE *fp = fopen("temp.mp3", "w"); //open teap file in write mode
	if(fp== NULL)
	{
		printf("Error: Unable to open the file\n");
		fclose(fptr);
		return 0;
	}
	char head [10];
	fread(head, 1, 10, fptr); //read the header
	fwrite(head, 1,10,fp); //write the header to the new file
			       //check the option provided and edit the corresponding data
	if(strcmp(argv[2],"-t") == 0) //edit title
	{
		printf("-----------Selected to edit the title-------------- \n");
		char tag[4], str[4];
		fread(tag, 1,4, fptr);
		for(int i=0;i<4;i++)
		{
			str[i] = tag[i];
		}
		str[4] = '\0';
		if(strcmp(str, "TIT2") == 0) 
		{
			edit_main(fptr, fp, argv, tag);
		}
		printf("Title: %s\n",argv[3]);

		printf("---------------The title edited successfully--------------- \n");
	} 
	else if(strcmp(argv[2],"-a")==0) //edit artist
	{
		printf("----------------Selected to edit the artist name----------------- \n");
		editing(fptr, fp, argv, "TPEI");
		printf("Artist: %s\n",argv[3]) ;
		printf("--------------The artist name edited successfully-----------------\n");
	}
	else if(strcmp(argv[2],"-A") == 0) //edit album
	{
		printf("------------Selected to edit the album name-----------------\n");
		editing(fptr, fp, argv, "TALB");
		printf("Album: %s\n",argv[3]);
		printf("------------------The album name edited successfully--------------- \n");
	}
	else if(strcmp(argv[2],"-y")==0) //edit year
	{
		printf("------------Selected to edit the year---------------\n");
		editing (fptr, fp, argv, "TYER");
		printf("Year: %s\n",argv[3]);
		printf("------------The year edited successfully------------- \n");
	} 
	else if(strcmp(argv[2],"-c")==0) //edit comments
	{
		printf("-------------Selected to edit the comments------------\n");
		editing (fptr, fp,argv, "COMM");
		printf("Comments: %s\n",argv[3]);
		printf("------------The comments edited successfully-----------\n");
	}
	else if(strcmp(argv[2],"")==0) //edit music track
	{
		printf("-------------Selected to edit the music track------------\n");
		editing(fptr, fp, argv, "TCON");
		printf("Music: %s\n",argv[3]);
		printf("----------The music track edited successfully-------------\n");
	}
	fclose(fptr); 
	fclose(fp);
	fptr= fopen("sample.mp3","w");
	if(fptr== NULL)
	{
		printf("Error: Unable to open the file\n");
		return 0;
	} 
	fp = fopen("temp.mp3","r");
	if(fp== NULL)
	{
		printf("Error: Unable to open the file\n");
		return 0;
	} //copy all the data from the temp file to sample file
	char cha;
	while(fread(&cha,1,1,fp) > 0)
	{
		fwrite(&cha,1,1,fptr);
	}
	fclose(fptr);
	fclose(fp);
	return 1;
}

