#ifndef VIEW_H
#define VIEW_H

#include"mp3.h"
typedef struct tag_info
{
	char* version;
	char* title;
	char* artist;
	char* album;
	int year;
	char* music;
	char* comment;
}tag;

void version_id(FILE *fp);
Status view_mp3(char* str);
 OperationType check_operation_type(char *argv[]);
int edit_opr(char * argv[]);
#endif
