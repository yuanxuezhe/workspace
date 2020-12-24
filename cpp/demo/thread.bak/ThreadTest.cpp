/*
 * @Description: 
 * @Version: 1.0
 * @Autor: yuanshuai 446968454@qq.com
 * @Date: 2020-12-25 00:48:19
 * @LastEditors: yuanshuai 446968454@qq.com
 * @LastEditTime: 2020-12-25 00:48:27
 */
#include "Thread.h"
#include <time.h>

class CJobThread:public CThread
{
public:
    CJobThread() : CThread(){};
    CJobThread(bool createsuspended,bool detach): CThread(createsuspended,detach){};
    ~CJobThread() {};

    void Execute()
    {
	printf("Thread %d print\n",Self());
    }

    void Run()
    {
	printf("Thread %d print\n",Self());
    }
  private:
   unsigned int id;
};

main()
{
    for(int i=0;i<10;i++)
    {
	CJobThread* thr = new CJobThread();
	thr->Start();
    }
}

