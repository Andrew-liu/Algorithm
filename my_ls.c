/*************************************************************************
	> File Name: my_ls.c
	> Author: Comst
	> Mail:750145240@qq.com 
	> Created Time: Fri 20 Jun 2014 11:21:39 AM CST
 ************************************************************************/

#include<stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
int main(int argc, char* argv[])
{
	DIR* pDir ;
	struct dirent* myent;
	int col = 0 ;
	if(argc == 1)
	{
		pDir = opendir(".");
	}else 
	{
		pDir = opendir(argv[1]);
	}
	if(pDir == NULL)
	{
		perror("open dir fail: ");
		exit(-1);
	}
	while( (myent = readdir(pDir) ) != NULL )
	{
		if(strncmp(myent ->d_name, ".",1) !=0 && strncmp(myent->d_name,"..",2)!=0)
		{
			printf("%-10s",myent->d_name);
			col ++ ;
			if(col % 5 == 0) 
			{
				printf("\n");
			}
		}
	}
	printf("\n");
	return  0 ;
}
