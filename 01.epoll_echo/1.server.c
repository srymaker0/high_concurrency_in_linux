/*************************************************************************
	> File Name: 1.server.c
	> Author:Yj_Z 
	> Mail: 
	> Created Time: Mon 19 Jul 2021 02:26:14 PM CST
 ************************************************************************/

#include "head.h"
#define MAXUSER 100
#define EPOLL_SIZE 10

void msg_handle(char *buff) {
    for (int i = 0; buff[i] != '\n'; i++) {
        if (buff[i] <= 'z' && buff[i] >= 'a') {
           buff[i] -= 'a' - 'A';
        } else if (buff[i] >= 'A' && buff[i] <= 'Z') {
           buff[i] += 'a' - 'A';
        }
    }
    return ;
}

int main(int argc, char **argv) {
    int opt, epollfd, server_listen, port;
    int client_fd;
    int boy[MAXUSER + 5] = {0};
    if (argc != 3) {
        fprintf(stderr, "Usage : %s -p port", argv[0]);
        exit(1);
    }
    while ((opt = getopt(argc, argv, "p:")) != -1) {
        switch (opt) {
            case 'p':
                port = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage : %s -p port", argv[0]);
                exit(1);    
        }
    }
    if ((server_listen = socket_create(port)) < 0) {
        perror("socket_create");
        exit(1);
    }

    boy[server_listen] = server_listen;
    // 创建实例
    epollfd = epoll_create(1);
    if (epollfd == -1) {
        perror("epollfd_create");
        exit(1);
    }
    
    struct epoll_event ev, events[EPOLL_SIZE];
    ev.events = EPOLLIN;
    // 用户态设置
    ev.data.fd = server_listen;
    // 内核态设置
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, server_listen, &ev) < 0) {
        perror("epoll_ctl");
        exit(1);
    }
    //make_nonblock(server_listen);
    while (1) {
        // 感兴趣的事件放入到events数组中，是否用到了mmap？？是在内核态还是在用户态？？？
        int nfds = epoll_wait(epollfd, events, EPOLL_SIZE, -1);
        if (nfds < 0) {
            perror("epoll_wait");
            exit(1);
        }
        for (int i = 0; i < nfds; i++) {
            int fd = events[i].data.fd;
            if (fd == server_listen) {
                struct sockaddr_in client;
                struct epoll_event ev;
                socklen_t len = sizeof(client);
                if ((client_fd = accept(server_listen, (struct sockaddr *)&client, &len)) < 0) {
                    perror("accept");
                    exit(1);
                }
                
                boy[client_fd] = client_fd;
                printf("<Accept> %s:%d!\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
                
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = boy[client_fd];
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, boy[client_fd], &ev) < 0) {
                    perror("epoll_ctl");
                    exit(1);
                }
                //make_nonblock(boy[client_fd]);
            } else {
                char buff[4096] = {0};
                if (events[i].events & (EPOLLIN | EPOLLERR | EPOLLHUP)) {
                    int rsize = recv(fd, buff, sizeof(buff), 0);
                    if (rsize <= 0) {
                        epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL);
                        close(fd);
                        continue;
                    } else {
                        msg_handle(buff);
                        send(fd, buff, strlen(buff), 0);
                    }
                }
            }
        }
    }
    //close(server_listen);
    //close(epollfd);
    return 0;
}
