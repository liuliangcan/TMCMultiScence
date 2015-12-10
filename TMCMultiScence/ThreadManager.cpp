/* 
 * File:   ThreadManager.cpp
 * Author: LIU
 * Describe: 
 * Created on 2015年11月13日, 上午11:00
 */

#include <fstream>

#include "ThreadManager.h"

#include "tools/dir_info.h"
#include "GlobalConfiger.h"
#include "tools/log.h"


using namespace std;

ThreadManager* ThreadManager::m_instance_tm = NULL;

ThreadManager::ThreadManager()
{
}

ThreadManager::ThreadManager(const ThreadManager& orig)
{
}

ThreadManager::~ThreadManager()
{
    CPO_Dele(m_cpo_qFilelist);
}

int ThreadManager::Init()
{
    CPO_Init(m_cpo_qFilelist);
    
    GlobalConfiger *gc = GlobalConfiger::GetInstance();
    m_ithreadNum = gc->GetIThreadNum();
    gc->GetListInputDir(m_listInputDir); 
    m_logDir = gc->GetLogDir();
    
    return 0;
}

int ThreadManager::GetChildDir(char * dir, bool bFirstTimeFlag)
{
    string basedir = dir;       
        
        struct DIR_INFO *hDirInfo;
        struct FILE_INFO hFileInfo;

        char sSrcFileFullPath[1024];
        hDirInfo = F_DirOpen((char*)basedir.c_str()); 
        if (hDirInfo != NULL) {

            while (F_DirNextItem(hDirInfo, &hFileInfo) == 0) {
                if (hFileInfo.sName[0] == '.' || hFileInfo.sName[0] == ' ')
                    continue;
                  
                if(strcasecmp(hFileInfo.sName + (strlen(hFileInfo.sName) - 4), ".bin") == 0)
                {
                    sprintf(sSrcFileFullPath, "%s/%s", basedir.c_str(), hFileInfo.sName);
                    string filestr = sSrcFileFullPath;
                    m_qFilelist.push(filestr);
                    continue;
                }
                
                if(bFirstTimeFlag && strcasecmp(hFileInfo.sName + (strlen(hFileInfo.sName) - 9), DEALING_TMP_STRING) == 0)
                {
                    sprintf(sSrcFileFullPath, "%s/%s", basedir.c_str(), hFileInfo.sName);
                    char sSrcFileFullPathNew[1024];
                    strcpy(sSrcFileFullPathNew, sSrcFileFullPath);
                    sSrcFileFullPathNew[(strlen(sSrcFileFullPath) - 9)] = 0;
                    rename(sSrcFileFullPath, sSrcFileFullPathNew);
                    string filestr = sSrcFileFullPathNew;
                    m_qFilelist.push(filestr);
                    continue;
                }
//                sprintf(sSrcFileFullPath, "%s/%s", basedir.c_str(), hFileInfo.sName); 
//                if (strcasecmp(sSrcFileFullPath + (strlen(sSrcFileFullPath) - 4), ".bin") == 0) {
//                    string filestr = sSrcFileFullPath;
//                    m_qFilelist.push(filestr);
//                    sleep(0);
//                }//endif
              /*  if (hFileInfo.bDir == 0) {
                    ;
                }
                else {
                    getFileList(listFileList, sSrcFileFullPath);
                }*/
            }//end while
        }//end if
        else
        {
            LOG_ERROR("[ThreadManager]打开目录失败！%s",basedir.c_str());
        }
        F_DirClose(hDirInfo);
    return 0;
}
//获取文件列表
//定义的所有目录中的文件全部读入m_qFilelist
int ThreadManager::GetFileList()
{
    static bool bFirstTime = true;
    
    list<string>::iterator it_m_listInputDir = m_listInputDir.begin();
    list<string>::iterator it_m_listInputDirt_end = m_listInputDir.end();
    for(;it_m_listInputDir != it_m_listInputDirt_end;++it_m_listInputDir)
    {
        string basedir = *it_m_listInputDir;       
        
        struct DIR_INFO *hDirInfo;
        struct FILE_INFO hFileInfo;

        char sSrcFileFullPath[1024];
        hDirInfo = F_DirOpen((char*)basedir.c_str()); 
        if (hDirInfo != NULL) {

            while (F_DirNextItem(hDirInfo, &hFileInfo) == 0) {
                if (hFileInfo.sName[0] == '.' || hFileInfo.sName[0] == ' ')
                    continue;
                
                 if (hFileInfo.bDir == 1) 
                 {
                     int day = atoi(hFileInfo.sName);
                     if(day > 20000000)
                     {
                         sprintf(sSrcFileFullPath, "%s/%s", basedir.c_str(), hFileInfo.sName);
                         GetChildDir(sSrcFileFullPath, bFirstTime);
                     }                 
                 }
            }//end while
        }//end if
        else
        {
            LOG_ERROR("[ThreadManager]打开目录失败！%s",basedir.c_str());
        }
        F_DirClose(hDirInfo);
    }   
    bFirstTime = false;
    return 0;
}
THREAD_RETURN THREAD_PROC FileHandleProc(THREAD_PARAM param);
int ThreadManager::Start()
{    
    for(int i = 0;i < m_ithreadNum;++i)
    {
        THREAD_ID handle_FileHandleProc = NULL;	
        handle_FileHandleProc = bcreate_thread(FileHandleProc, this);
        
        if(FileHandleProc == NULL)
        {		
                LOG_ERROR("[FileHandleProc]创建线程失败");
                return -1;
        }
    }
    LOG_NOTICE("[FileHandleProc]启动%d个线程",m_ithreadNum);
    return 0;
}

    time_t lastdealtime = time(0);
    time_t emptytime = time(0);
    bool hasWriteEndFile = true;
    int bDealing = 0;
    
THREAD_RETURN THREAD_PROC FileHandleProc(THREAD_PARAM param)
{
    ThreadManager* pParam = (ThreadManager*)param;
    bool threadflag = true;
    string file;
    MultiScence ms;
    while(threadflag)
    {
        CPO_Enter(pParam->m_cpo_qFilelist);
        if(pParam->m_qFilelist.empty())
        {//检查m_qFilelist队列，如果为空，休息五秒，否则取出来一个文件进行处理
            
            pParam->GetFileList();
            if(pParam->m_qFilelist.empty())
            {
                emptytime = time(0);
                unsigned int d_time = emptytime - lastdealtime;
                if(bDealing == 0 && !hasWriteEndFile && d_time > GlobalConfiger::GetInstance()->GetUMultiScenceEndDelay())
                {
                    int batchover = (int)time(0);
                    printf("本批次处理结束[%d]\n", batchover);
                    LOG_NOTICE("[FileHandleProc]批次处理结束[%d]", batchover);
                    std::ofstream endfileout(GlobalConfiger::GetInstance()->GetUMultiScenceEndBatchFile().c_str());
                    if(!endfileout.is_open())
                    {
                        LOG_ERROR("[FileHandleProc]批次处理结束标识文件打开失败");
                    }
                    endfileout << batchover;
                    if(endfileout.is_open())endfileout.close();
                    
                    hasWriteEndFile = true;
                }
            }
            CPO_Leave(pParam->m_cpo_qFilelist);
            
            sleep(5);
        }
        else
        {
            ++bDealing;
            file = pParam->m_qFilelist.front();           
            pParam->m_qFilelist.pop();
            string tmpname = file + DEALING_TMP_STRING;
            rename(file.c_str(), tmpname.c_str());
            
            lastdealtime = time(0);
            hasWriteEndFile = false;
            CPO_Leave(pParam->m_cpo_qFilelist);
            sleep(0);
            
            LOG_NOTICE("[MultiScence]开始处理文件:%s",file.c_str());
            ms.SetSFile(file);
            ms.Process();
            
            CPO_Enter(pParam->m_cpo_qFilelist);
            --bDealing;
            CPO_Leave(pParam->m_cpo_qFilelist);
        }
    }
    
    return (THREAD_RETURN)0;
}