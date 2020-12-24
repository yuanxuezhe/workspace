/*
 * @Description: 
 * @Version: 1.0
 * @Autor: yuanshuai 446968454@qq.com
 * @Date: 2020-12-24 23:07:10
 * @LastEditors: yuanshuai 446968454@qq.com
 * @LastEditTime: 2020-12-24 23:07:47
 */
#include "threadpool.h"

class CThreadManage
{
private:
    CThreadPool*    m_Pool;
    int          m_NumOfThread;
protected:
public:
    void     SetParallelNum(int num);
    CThreadManage();
    CThreadManage(int num);
    virtual ~CThreadManage();

    void    Run(CJob* job,void* jobdata);
    void    TerminateAll(void);
};