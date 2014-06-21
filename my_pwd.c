/*************************************************************************
	> File Name: my_pwd.c
	> Author: Comst
	> Mail:750145240@qq.com 
	> Created Time: Fri 20 Jun 2014 09:19:03 AM CST
 ************************************************************************/
#include <unistd.h>
#include<stdio.h>
int main(int argc, char* argv[])
{
	printf("current directory: %s \n", getcwd(NULL, 0));
	return 0 ;
}

