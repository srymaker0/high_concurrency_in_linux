/*************************************************************************
	> File Name: head.h
	> Author:Yj_Z 
	> Mail: 
	> Created Time: Mon 22 Mar 2021 02:23:41 PM CST
 ************************************************************************/

#ifndef _HEAD_H
#define _HEAD_H
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>   // open headfile
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>


#include "common.h"      //the head file which wrote by personally
#include "my_pipe.h"
#include "thread_pool.h"
#include "reactor.h"
#include "add_to_subreactor.h"


#endif
