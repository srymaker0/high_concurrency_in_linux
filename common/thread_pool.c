/*************************************************************************
	> File Name: 3.thread_pool.c
	> Author:Yj_Z 
	> Mail: 
	> Created Time: Fri 07 May 2021 09:27:23 AM CST
 ************************************************************************/

#include "head.h"

void task_queue_init(struct task_queue *taskQueue, int size) {
    taskQueue->size = size;
    taskQueue->total = taskQueue->head = taskQueue->tail = 0;
    pthread_mutex_init(&taskQueue->mutex, NULL);
    pthread_cond_init(&taskQueue->cond, NULL);
    taskQueue->data = calloc(size, sizeof(void *));
    return ;
}

void task_queue_push(struct task_queue *taskQueue, char *str) {
    pthread_mutex_lock(&taskQueue->mutex);     // 【加锁】
    // 若任务队列满了
    if (taskQueue->total == taskQueue->size) {
        printf("taskQueue is full!\n");
        pthread_mutex_unlock(&taskQueue->mutex);
        return ;
    }
    // 入队
    printf("<Push> : %s\n", str);
    taskQueue->data[taskQueue->tail] = str;
    taskQueue->total++;
    // 超出队尾
    if (++taskQueue->tail == taskQueue->size) {
        printf("taskQueue tail reached end!\n");
        taskQueue->tail = 0;
    }
    pthread_cond_signal(&taskQueue->cond);      // 发信号
    pthread_mutex_unlock(&taskQueue->mutex);    // 解锁
}

char *task_queue_pop(struct task_queue *taskQueue) {
    pthread_mutex_lock(&taskQueue->mutex);
    // 若任务队列为空，则等待任务到来
    if (taskQueue->total == 0) {
        printf("taskQueue is empty!\n");
        pthread_cond_wait(&taskQueue->cond, &taskQueue->mutex);
    }
    // 否则，取走任务
    char *str = taskQueue->data[taskQueue->head];
    printf("<Pop> : %s\n", str);
    taskQueue->total--;
    // 若超出队列尾部
    if (++taskQueue->head == taskQueue->size) {
        printf("taskQueue head reached end!\n");
        taskQueue->head = 0;
    }
    pthread_mutex_unlock(&taskQueue->mutex);
    return str;
}
