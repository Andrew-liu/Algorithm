/*************************************************************************
	> File Name: my_mkdir.c
	> Author: Comst
	> Mail:750145240@qq.com 
	> Created Time: Fri 20 Jun 2014 09:38:34 AM CST
 ************************************************************************/
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char* argv[])
{
	int iret ;
	if(argc == 1)
	{
		printf("too few arg ! \n");
        exit(-1);
	}else 
	{
		iret = mkdir(argv[1],0777);
	}
}

