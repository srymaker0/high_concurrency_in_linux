/*************************************************************************
	> File Name: 1.server.c
	> Author:Yj_Z 
	> Mail: 
	> Created Time: Mon 19 Jul 2021 02:26:14 PM CST
 ************************************************************************/

#include "head.h"
#define MAXUSER 100
#define EPOLL_SIZE 10

int main(int argc, char **argv) {
    int opt, epollfd, server_listen, port;
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
    epollfd = epoll_create(1);
    if (epollfd == -1) {
        perror("epollfd_create");
        exit(1);
    }
    
    struct epoll_event ev, events[EPOLL_SIZE];
    ev.events = EPOLLIN;
    ev.data.fd = server_listen;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, server_listen, &ev) < 0) {
        perror("epoll_ctl");
        exit(1);
    }
    while (1) {
        int nfds = epoll_wait(epollfd, events, EPOLL_SIZE, -1);
        if (nfds < 0) {
            perror("epoll_wait");
            exit(1);
        }
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == server_listen) {
                struct sockaddr_in client;
                int new_fd;
                socklen_t len = sizeof(client);
                if ((new_fd = accept(server_listen, (struct sockaddr *)&client, &len)) < 0) {
                    perror("accept");
                    exit(1);
                }
                
                //make_nonblock(new_fd);
                printf("<Accept> %s:%d!\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
                
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = new_fd;
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, new_fd, &ev) < 0) {
                    perror("epoll_ctl");
                    exit(1);
                }
                // 回显
                int fd = events[i].data.fd;
                char buff[4096] = {0};
                int rsize = recv(new_fd, buff, sizeof(buff), 0);
                printf("<The Msg>: %s\n", buff);
                if (rsize <= 0) {
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL);
                    close(fd);
                } else {
                    for (int i = 0; buff[i]; i++) {
                        if (buff[i] <= 'z' && buff[i] >= 'a') {
                            buff[i] -= 'a' - 'A';
                        } else if (buff[i] >= 'A' && buff[i] <= 'Z') {
                            buff[i] += 'a' - 'A';
                        }
                    }
                    send(new_fd, buff, strlen(buff), 0);   
                }
            }
        }
    }
    return 0;
}