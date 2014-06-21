/*************************************************************************
	> File Name: my_rmdir.c
	> Author: Comst
	> Mail:750145240@qq.com 
	> Created Time: Fri 20 Jun 2014 09:49:34 AM CST
 ************************************************************************/

#include<stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc, char* argv[])
{
	int iret ;
    if(argc == 1)
	{
		printf("too few arg \n");
		exit(-1);
	}	else 
	{
	   iret = rmdir(argv[1]);
	   if(iret != 0)
	   {
			perror("rmdir fail: "); 
	   }

	}
}
