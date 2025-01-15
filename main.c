#include"view.h"

OperationType check_operation_type(char *argv[])
{
	if(strcmp(argv[1],"-v")==0)
		return view;
	else if(strcmp(argv[1],"-e")==0)
		return edit;
//	else if(argv[1] == NULL)
//		return e_unsupported;
}
Status read_and_validate(char* str)
{
	printf("read and validate entered\n");
	if(strcmp(strchr(str,'.'),".mp3") == 0)
	{
		FILE *fp = fopen(str,"r");
		if(fp == NULL)
		{
			printf("file doesn't exists\n");
		}
		printf("file not NULL\n");
		unsigned char arr[3];
		fread(arr,1,3,fp);
		if(arr[0] == 0x49 && arr[1] == 0x44 && arr[2] == 0x33)
		{
			printf("%s\n",arr);
			printf("ID3 version\n");
		}
		return e_success;
	}
	return e_failure;
}	

int main(int argc,char *argv[])
{
	if(argc == 1)
	{
		printf("insufficient data\n");
		return 0;
	}
	int res = check_operation_type(argv);
	if(res == view)
	{
		printf("view option\n");
		if(argc =3)
		{
			int res = read_and_validate(argv[2]);
			if(res == e_failure)
			{
				printf("read and validation failed\n");
				return 0;
			}
			else
			{
				printf("read and validate successful\n");
			}
			if(view_mp3(argv[2]) == e_success)
				{
					printf("view successful\n");
		}
		}
	}
		else if(res == edit)
		{
			printf("Edit option\n");
			edit_opr(argv);
		}
		else
			printf("invalid\n");
	
}
