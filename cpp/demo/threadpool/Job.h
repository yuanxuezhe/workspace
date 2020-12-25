/*
 * @Description: 
 * @Version: 1.0
 * @Autor: yuanshuai 446968454@qq.com
 * @Date: 2020-12-25 22:22:03
 * @LastEditors: yuanshuai 446968454@qq.com
 * @LastEditTime: 2020-12-25 23:18:55
 */

#ifndef _JOB_H_
#define _JOB_H_

/*****************************************************************
* The class is an abstract class,which define the abstract job    *
* All job should inherit from it  and implement the abstract     *
* method RunJob()                                                *
*****************************************************************/

#include "Thread.h"

class CJob
{
private:
    int		m_JobNo;		//The num was assigned to the job
    char*	m_JobName;		//The job name 
    CThread	*m_pWorkThread;		//The thread associated with the job
        
public:
    CJob( void );
    virtual ~CJob();
        
    int		GetJobNo(void) const { return m_JobNo; }
    void	SetJobNo(int jobno){ m_JobNo = jobno;}

    char* 	GetJobName(void) const { return m_JobName; }
    void	SetJobName(char* jobname);

    CThread *GetWorkThread(void){ return m_pWorkThread; }
    void SetWorkThread (CThread *pWorkThread){
	    m_pWorkThread = pWorkThread;
    }

    bool	GetTerminated(void);
    virtual void Run ( void *ptr ) = 0;
};

#endif
