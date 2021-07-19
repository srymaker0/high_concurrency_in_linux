/*************************************************************************
	> File Name: 2.client.c
	> Author:Yj_Z 
	> Mail: 
	> Created Time: Mon 19 Jul 2021 03:45:56 PM CST
 ************************************************************************/

#include "head.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage : %s ip port", argv[0]);
        exit(1);
    }
    int sockfd;
    if ((sockfd = socket_connect(argv[1], atoi(argv[2]))) < 0) {
        perror("socket_connect");
        exit(1);
    }
    while (1) {
        char buff[4096] = {0};
        scanf("%[^\n]s", buff);
        getchar();
        if (strlen(buff)) {
            send(sockfd, buff, strlen(buff), 0);
        }
    }
    return 0;
}
