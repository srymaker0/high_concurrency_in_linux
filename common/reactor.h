/*************************************************************************
	> File Name: reactor.h
	> Author:Yj_Z 
	> Mail: 
	> Created Time: Sun 18 Jul 2021 03:51:51 PM CST
 ************************************************************************/

#ifndef _REACTOR_H
#define _REACTOR_H

// 客户的请求
struct logRequest {
    char name[20];
    int sex;          // 性别 1 男 0 女
};

// 服务端给客户的回应
struct logResponse {
    int flag;         // 1 正确连接，0 出错
    char msg[512];    // 回应消息
};

struct User {
    int fd;
    char name[20];
    char ip[20];
    int online;
    int epollfd;
};
#endif
