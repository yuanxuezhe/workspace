/*
 * @Description: 
 * @Version: 1.0
 * @Autor: yuanshuai 446968454@qq.com
 * @Date: 2020-12-25 00:37:25
 * @LastEditors: yuanshuai 446968454@qq.com
 * @LastEditTime: 2020-12-25 00:46:39
 */
#ifndef _WORKERTHREAD_H_
#define _WORKERTHREAD_H_

#include "Thread.h"
#include "ThreadPool.h"
#include "Job.h"
#include "sync.h"
/**************************************************************
 *For class CThread is an abstract class,it can't be instantiated
 *class CWorkerThread is the actual thread that run the job,but
 *
 **************************************************************/

//class CThreadPool;

class CWorkerThread:public CThread
{
private:
    CThreadPool*  m_ThreadPool;
    
    //the job that associate with the workthread
    CJob*	 m_Job;
    void*	 m_JobData;	
    
    CThreadMutex m_VarMutex;
    bool 	 m_IsEnd;
protected:
public:
    CCondition   m_JobCond;
    CThreadMutex m_WorkMutex;
  
    CWorkerThread();
    virtual ~CWorkerThread();
    
    void Run();
     
    void    SetJob(CJob* job,void* jobdata); 
    CJob*   GetJob(void){return m_Job;}

    void    SetThreadPool(CThreadPool* thrpool);
    CThreadPool* GetThreadPool(void){return m_ThreadPool;}
    bool    IsWorking(void){return m_Job != NULL;} 

    void    Wakeup(CJob* job,void* jobdata);
};

#endif
