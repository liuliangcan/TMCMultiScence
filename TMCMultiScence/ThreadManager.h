/* 
 * File:   ThreadManager.h
 * Author: LIU
 * Describe: 多线程管理类
 * Created on 2015年11月13日, 上午11:00
 */

#ifndef THREADMANAGER_H
#define	THREADMANAGER_H

#include <string>
#include <queue>
#include <list>
#include "tools/macro.h"
#include "MultiScence.h"

class ThreadManager {
public:
    
    //初始化函数
    int Init();
    
    //启动函数
    int Start();
    
    ThreadManager();
    ThreadManager(const ThreadManager& orig);
    virtual ~ThreadManager();
    
    //单例
    static ThreadManager *GetInstance()
    {
            if(m_instance_tm == NULL)
            {
                    m_instance_tm = new ThreadManager();
            }
            return m_instance_tm;
    }
private:
    friend THREAD_RETURN THREAD_PROC FileHandleProc(THREAD_PARAM param);
    //启动的处理线程数
    int m_ithreadNum;
    
    //输入目录
    std::list<std::string> m_listInputDir;
    //日志输出目录
    std::string m_logDir;
    
    //读取到的文件列表
    std::queue<std::string> m_qFilelist;
    
    //文件列表锁
    CPO m_cpo_qFilelist;
    
    //读取文件列表
    int GetFileList();
    int GetChildDir(char * dir, bool bFirstTimeFlag);
    
    //单例
    static ThreadManager *m_instance_tm;
};

#endif	/* THREADMANAGER_H */

