/*
 * @Description: 
 * @Version: 1.0
 * @Autor: yuanshuai 446968454@qq.com
 * @Date: 2020-12-25 22:13:52
 * @LastEditors: yuanshuai 446968454@qq.com
 * @LastEditTime: 2020-12-26 01:13:55
 */
#include "Job.h"
#include "time.h"
#include "ThreadManage.h"

class CXJob:public CJob
{
private:
    int i;	
    CThreadMutex mutex;
public:
    CXJob(){i=0;}
    ~CXJob(){}
    void Run(void* jobdata)
    {
        mutex.Lock();
        i++;
        mutex.Unlock();
        printf("CXJob %d\n",i);
        sleep(2);
    }
};

class CYJob:public CJob
{
private:
    int i;
    CThreadMutex mutex;
public:
    CYJob(){i=0;}
    ~CYJob(){}
    void Run(void* jobdata)
    {
        mutex.Lock();
        printf("==========================\n");
        mutex.Unlock();
        printf("CYJob %d\n",i);
        sleep(2);
    }
};


main()
{
    CThreadManage* manage = new CThreadManage(3);
   
    int i=0;
	
    for(i=0;i<4;i++)
    {
        CXJob*   job = new CXJob();
        manage->PostJob(job,NULL);
    }
    manage->Run();

    CYJob* job = new CYJob();
    manage->PostJob(job,NULL);
    
    manage->Run();
    manage->TerminateAll();
}
