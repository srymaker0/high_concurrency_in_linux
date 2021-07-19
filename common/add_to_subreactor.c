/*************************************************************************
	> File Name: add_to_subreactor.c
	> Author:Yj_Z 
	> Mail: 
	> Created Time: Sun 18 Jul 2021 03:49:31 PM CST
 ************************************************************************/

#include "head.h"

void add_event_ptr(int epollfd, struct User *user) {
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.ptr = (void *)user;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, user->fd, &ev);
}

void add_to_subreactor(struct User *user) {
    add_event_ptr(user->epollfd, user);
}
