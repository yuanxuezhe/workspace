/*
 * @Description: 
 * @Version: 1.0
 * @Autor: yuanshuai 446968454@qq.com
 * @Date: 2020-12-25 00:38:03
 * @LastEditors: yuanshuai 446968454@qq.com
 * @LastEditTime: 2020-12-25 00:54:20
 */
#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_

#include "Thread.h"
#include "WorkerThread.h"
#include "Job.h"
#include "sync.h"
#include <vector>

class CWorkerThread;

class CThreadPool
{
friend class CWorkerThread;

private:
    unsigned int	m_MaxNum;	//the max thread num that can create at the same time
    unsigned int	m_ActiveNum;	//the normal thread num that keep alive
    unsigned int	m_MinNum;	//the min thread num that should keep alive
    unsigned int	m_BusyNum;	//The busy thread num
    unsigned int 	m_IdleNum;	//The idle thread num
    unsigned int	m_AliveNum;	//The actual alive thread,as m_BusyNum+m_IdleNum;
					//which is not certainly equal to m_ActiveNum;
					//m_ActiveNum is the value we set,but m_AliveNum
					//is the actual num
protected:

    CWorkerThread* GetIdleThread(void);
    CJob*   GetIdleJob(void);
    bool    DeleteJob(CJob* job);
    void    AppendToIdleList(CWorkerThread* jobthread);
    void    MoveToBusyList(CWorkerThread* idlethread);
    void    MoveToIdleList(CWorkerThread* busythread);

public:
    CThreadMutex	m_BusyMutex;	//when visit busy list,use m_BusyMutex to lock and unlock
    CThreadMutex	m_IdleMutex;	//when visit idle list,use m_IdleMutex to lock and unlock
    CThreadMutex	m_JobMutex;	//when visit job list,use m_JobMutex to lock and unlock

    CCondition		m_BusyCond;	//m_BusyCond is used to sync busy thread list
    CCondition		m_IdleCond;	//m_IdleCond is used to sync idle thread list
    CCondition		m_IdleJobCond;	//m_JobCond is used to sync job list


    std::vector<CWorkerThread*>	m_ThreadList;
    std::vector<CWorkerThread*>	m_BusyList;     //Thread List
    std::vector<CWorkerThread*>	m_IdleList;	//Idle List
    std::vector<CJob*>		m_JobList;      //Job list

    CThreadPool();
    CThreadPool(int maxnum,int minnum,int initnum);
    CThreadPool(int initnum);
    virtual ~CThreadPool();

    void    SetMaxNum(int maxnum){m_MaxNum = maxnum;}
    int     GetMaxNum(void){return m_MaxNum;}
    
    void    SetMinNum(int minnum){m_MinNum = minnum;}
    int     GetMinNum(void){return m_MinNum;}
   
    void    SetActiveNum(int activenum){m_ActiveNum = activenum;}
    int     GetActiveNum(void){return m_ActiveNum;}

    int     GetBusyNum(void){return m_BusyNum;}
    int     GetIdleNum(void){return m_IdleNum;} 
    int     GetAliveNum(void){return m_AliveNum;}

    void    PostJob(CJob* job,void* jobdata);
    void    Run();
    void    TerminateAll(void);
};


#endif
