/*************************************************************************
	> File Name: common.c
	> Author:Yj_Z 
	> Mail: 
	> Created Time: Tue 23 Mar 2021 09:04:38 PM CST
 ************************************************************************/
#include "head.h"
int make_nonblock(int fd) {
    int flag;
    if ((flag = fcntl(fd, F_GETFL)) < 0) {
        perror("fcntl");
        return -1;
    }
    flag |= O_NONBLOCK;
    return fcntl(fd, F_SETFL, flag);
}   
int make_block(int fd) {
    int flag;
    if ((flag = fcntl(fd, F_GETFL)) <0) {
        perror("fcntl");
        return -1;
    }
    flag &= ~O_NONBLOCK; 
    return fcntl(fd, F_SETFL, flag);
}

int socket_create(int port) {
    // 1.创建一个快递员：ipv4、子节流、TCP【socket属于系统调用】
    int sockfd;
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        // 若文件打开数量有限，可能出错
        return -1;      // 返回给上层
    }
    // 2-1 填写表单给bind
    addr.sin_family = AF_INET;                    // ipv4
    addr.sin_port = htons(port);                  // htons: 主机->网络 端口：输入参数  
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");  // 0.0.0.0代表可接收对于本机任意IP地址的连接
    // 2-2 绑定IP和端口：0～1023号端口需要权限【周知端口】
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        return -1;                                // 没有权限，端口被占用
    }
    // 3、设为监听状态【主动变为被动】 完成队列的长度为20【两边都处于ESTABLISHED状态】
    if (listen(sockfd, 20) < 0) {
        return -1;
    }
    return sockfd;
}

int socket_connect(const char *ip, int port) {
    // 创建
    int sockfd;
    struct sockaddr_in server;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket()");
        return -1;
    }
    // 填表
    server.sin_family = AF_INET;
    server.sin_port = htons(port);          
    server.sin_addr.s_addr = inet_addr(ip);
    // 主动连接指定IP和端口
    if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect");
        return -1;
    }
    return sockfd;
}
