/*
 * @Description: 
 * @Version: 1.0
 * @Autor: yuanshuai 446968454@qq.com
 * @Date: 2020-12-24 22:12:04
 * @LastEditors: yuanshuai 446968454@qq.com
 * @LastEditTime: 2020-12-24 22:19:01
 */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
void*ThreadEntry(void*arg)
{
    (void)arg;
    printf("I am thread\n");
}

int main()
{
    pthread_t tid;
    int ret=0;
    ret=pthread_create(&tid,NULL,ThreadEntry,NULL);
    if(ret!=0)
    {
        perror("pthread_create");
        exit(1);
    }
    printf("I am main\n");
    sleep(1);
 //让主线程睡眠1秒，是为了不让此线程跑的太快使整个进程结束
 //如果主线程跑的太快，新创建的线程还没来的及执行，整个进程就已经结束了
    return 0;
}
