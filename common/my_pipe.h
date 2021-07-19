/*************************************************************************
	> File Name: my_pipe.h
	> Author:Yj_Z 
	> Mail: 
	> Created Time: Fri 23 Apr 2021 10:40:30 AM CST
 ************************************************************************/

#ifndef _MY_PIPE_H
#define _MY_PIPE_H
FILE *my_popen(const char *command, const char *type);
int my_pclose(FILE *fp);
#endif
