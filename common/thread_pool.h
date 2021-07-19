/*************************************************************************
	> File Name: thread_pool.h
	> Author:Yj_Z 
	> Mail: 
	> Created Time: Fri 07 May 2021 09:28:05 AM CST
 ************************************************************************/

#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H
struct task_queue {
    int size;
    int total;
    int head;
    int tail;
    void **data;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};
void task_queue_init(struct task_queue *taskQueue, int size);
void task_queue_push(struct task_queue *taskQueue, char *str);
char *task_queue_pop(struct task_queue *task_queue);
#endif
