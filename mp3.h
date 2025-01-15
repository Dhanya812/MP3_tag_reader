#ifndef MP3_H
#define MP3_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>


#define SUCCESS 1
#define FAILURE 0

typedef enum
{
    edit,
    view,
    e_unsupported
} OperationType;

typedef enum
{
	e_success,
	e_failure
}Status;
//int edit_opr(char *argv);
int to_LES(void *,int );
#endif
