/*
 * @Description: 
 * @Version: 1.0
 * @Autor: yuanshuai 446968454@qq.com
 * @Date: 2020-12-25 00:40:10
 * @LastEditors: yuanshuai 446968454@qq.com
 * @LastEditTime: 2020-12-25 23:21:14
 */

#ifndef _THREADMANAGE_H_
#define _THREADMANAGE_H_

#include "ThreadPool.h"
#include "Thread.h"
#include "sync.h"
//#include "WorkerThread.h"
#include "Job.h"

//===============================================================
//CThreadManage is the class that interact with client
//we usually use it as following
//CThreadManage* thrmanage = new CThreadManage(10);
//CXXJob job = new CXXJob();
//thrmanage->Run(job,NULL);

class CThreadManage
{
private:
    CThreadPool*    m_Pool;
    int		    m_NumOfThread;
protected:
public:
    void	    SetParallelNum(int num);
    CThreadManage();
    CThreadManage(int num);
    virtual ~CThreadManage();
 
    void    PostJob(CJob* job,void* jobdata);
    void    Run();
    void    TerminateAll(void);
};

#endif

