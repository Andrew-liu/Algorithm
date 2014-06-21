/*************************************************************************
	> File Name: my_cd.c
	> Author: Comst
	> Mail:750145240@qq.com 
	> Created Time: Fri 20 Jun 2014 09:25:03 AM CST
 ************************************************************************/

#include<stdio.h>
#include <unistd.h>
int main(int argc, char* argv[])
{
	int iret ;
	if(argc == 1)
	{
		iret = chdir("~");
	}else 
	{
		iret = chdir(argv[1]);
		printf("cwd: %s \n", getcwd(NULL, 0));
		getchar();
	}
	if(iret != 0)
	{
		perror("chdir fail: ");
	}
	return 0 ;
}
