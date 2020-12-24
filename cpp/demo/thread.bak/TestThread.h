/*
 * @Description: 
 * @Version: 1.0
 * @Autor: yuanshuai 446968454@qq.com
 * @Date: 2020-12-25 00:45:35
 * @LastEditors: yuanshuai 446968454@qq.com
 * @LastEditTime: 2020-12-25 00:45:35
 */
#ifndef _WORKERTHREAD_H_
#define _WORKERTHREAD_H_

#include "Thread.h"
#include "Job.h"
#include "sync.h"
/**************************************************************
 *For class CThread is an abstract class,it can't be instantiated
 *class CWorkerThread is the actual thread that run the job,but
 *
 **************************************************************/


class CTestThread:public CThread
{
protected:
public:
    CCondition   m_WorkCond;
    CThreadMutex m_WorkMutex;
  
    CTestThread();
    virtual ~CTestThread();
    
    void Run();
     
};

#endif
