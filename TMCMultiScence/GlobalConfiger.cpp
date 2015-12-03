/* 
 * File:   GlobalConfiger.cpp
 * Author: LIU
 * 
 * Created on 2015年11月18日, 下午12:41
 */

#include "GlobalConfiger.h"
#include "tools/Markup.h"
#include "tools/log.h"
#include "tools/dir_info.h"
#include "tools/toolkit.h"
#include <map>

GlobalConfiger* GlobalConfiger::m_instance_gc = NULL;
GlobalConfiger::GlobalConfiger()
{
}

GlobalConfiger::GlobalConfiger(const GlobalConfiger& orig)
{
}

GlobalConfiger::~GlobalConfiger()
{
    if(m_uCustomScence == NULL)delete[]m_uCustomScence;
}

int GlobalConfiger::Init(char * confXML)
{
    m_iThreadNum = 0;
    m_uCustomScence = NULL;
    m_uCustomScenceSize = 0;
    CMarkup cmxml;
    bool b = cmxml.Load(confXML);  
    if(!b)
    {
        fprintf(stderr, "[GlobalConfiger]配置文件读取失败！%s\n",confXML);
        return -1;
    }
     while (cmxml.FindChildElem("MultiScenceInputDir")) {  
        m_listInputDir.push_back(cmxml.GetChildAttrib("name")); 
    }
    cmxml.ResetMainPos();
    if (cmxml.FindChildElem("MultiScenceOutputDir"))
    {
         m_outputDir = cmxml.GetChildAttrib("name");
    }
    cmxml.ResetMainPos();
    if (cmxml.FindChildElem("MultiScenceLogDir"))
    {
        m_logDir = cmxml.GetChildAttrib("name");
    }
    cmxml.ResetMainPos();
    if (cmxml.FindChildElem("MultiScencebackupDir"))
    {
        m_backupDir = cmxml.GetChildAttrib("name");
    }
    cmxml.ResetMainPos();
    if (cmxml.FindChildElem("MultiScenceThreadNum"))
    {
        m_iThreadNum = atoi(cmxml.GetChildAttrib("name").c_str());        
    }
    cmxml.ResetMainPos();
    std::map<int,int> m ;
    std::map<int,int>::iterator it_m;
    m.clear();    
    while (cmxml.FindChildElem("MultiScenceCustomScence"))
    {
        int t = atoi(cmxml.GetChildAttrib("name").c_str());     
        if(t > 0) 
        {
            it_m = m.find(t);
            if(it_m == m.end())
            {
                int t2 = atoi(cmxml.GetChildAttrib("conditions").c_str());
                if(t2 < 64)
                {
                    m[t] =t2; 
                }
                else
                {
                    fprintf(stderr,"多场景自定义部分配置错误，请检查,name=%d,conditions=%d\n", t, t2);
                    return -1;
                }
            }
            else
            {
                fprintf(stderr,"多场景自定义部分配置错误，请检查,name=%d\n", t);
                return -1;
            }
        }
        else 
        {fprintf(stderr,"多场景自定义部分配置错误，请检查,name=%d\n", t);return -1;}
    }
    std::map<int,int>::iterator it_m_end = m.end() ;
    m_uCustomScenceSize = m.size() + 1;
    m_uCustomScence = new unsigned int[m_uCustomScenceSize];
    m_uCustomScence[0] = 0;
    int i = 1;
    for( it_m = m.begin();it_m != it_m_end; ++it_m,++i)
    {
        if(it_m->first != i)
        {
            fprintf(stderr,"多场景自定义部分配置错误:不连续，请配置1-n连续的不重复值，请检查\n");
            return -1;
        }
        m_uCustomScence[i] = it_m->second;
    }
    
    int ret =check();
    if(ret < 0)return -1;
    return 0;
}
int GlobalConfiger::check()
{
    std::list<std::string>::iterator it_m_listInputDir=  m_listInputDir.begin();
    std::list<std::string>::iterator it_m_listInputDir_end=  m_listInputDir.end();
    unsigned int inputNum = 0;
    for(;it_m_listInputDir != it_m_listInputDir_end;)
    {
        if(F_DirIsExist((char*)(*it_m_listInputDir).c_str()))
        {
            fprintf(stdout,"输入目录: %s\n",(*it_m_listInputDir).c_str());
            ++inputNum;
            ++it_m_listInputDir;
        }
        else
        {
            fprintf(stderr,"输入目录: %s,检查不存在， 忽略此目录\n",(*it_m_listInputDir).c_str());
            it_m_listInputDir = m_listInputDir.erase(it_m_listInputDir);
        }
    }
    if(inputNum == 0)
    {
        fprintf(stderr,"输入目录不可用， 程序退出！\n");
        return -1;
    }
    if(!F_DirIsExist((char*)m_outputDir.c_str()))
    {
        fprintf(stdout,"输出目录: %s不存在，",m_outputDir.c_str());
        int ret = F_MakeDirs((char*)m_outputDir.c_str(), 1);
        if(ret != 0)
        {
            fprintf(stderr,"创建失败， 请检查！errno = %d\n",ret);
            return -1;
        }
        else
        {
            fprintf(stdout,"创建成功。\n");
        }
    }
    else
    {
        fprintf(stdout,"输出目录: %s检测到存在。\n",m_outputDir.c_str());
    }
    if(!F_DirIsExist((char*)m_logDir.c_str())) 
    {
        fprintf(stdout,"日志目录: %s不存在，",m_logDir.c_str());
        int ret = F_MakeDirs((char*)m_logDir.c_str(), 1); 
        if(ret != 0)
        {
            fprintf(stderr,"创建失败， 请检查！errno = %d\n",ret);
            return -1;
        }
        else
        {
            fprintf(stdout,"创建成功。\n");
        }
    }
    else
    {
        fprintf(stdout,"日志目录: %s检测到存在。\n",m_logDir.c_str());
    }
    if(!F_DirIsExist((char*)m_backupDir.c_str()))
    {
        printf("备份目录备份目录备份目录备份目录%s:\n",m_backupDir.c_str());
        fprintf(stdout,"备份目录: %s不存在，",m_backupDir.c_str());
        int ret = F_MakeDirs((char*)m_backupDir.c_str(), 1); 
        if(ret != 0)
        {
            fprintf(stderr,"创建失败， 请检查！errno = %d\n",ret);
            return -1;
        }
        else
        {
            fprintf(stdout,"创建成功。\n");
        }
    }
    else
    {
        fprintf(stdout,"备份目录: %s检测到存在。\n",m_backupDir.c_str());
    }
    if(m_iThreadNum <= 0)
    {
        fprintf(stderr,"线程数目: %d, 设置错误， 请检查!\n",m_iThreadNum);
        return -1;
    }
    else
    {
        fprintf(stdout,"线程数目: %d, 成功。\n",m_iThreadNum);
    }
}

void GlobalConfiger::SetListInputDir(std::list<std::string> listInputDir)
{
    m_listInputDir = listInputDir;
}

void GlobalConfiger::GetListInputDir(std::list<std::string>& listInputDir) const
{
    listInputDir =  m_listInputDir;
}

void GlobalConfiger::SetOutputDir(std::string outputDir)
{
    m_outputDir = outputDir;
}

std::string GlobalConfiger::GetOutputDir() const
{
    return m_outputDir;
}

unsigned int GlobalConfiger::GetUCustomScenceSize() const
{
    return m_uCustomScenceSize;
}

unsigned int* GlobalConfiger::GetUCustomScence() const
{
    return m_uCustomScence;
}



void GlobalConfiger::SetIThreadNum(int iThreadNum)
{
    m_iThreadNum = iThreadNum;
}

int GlobalConfiger::GetIThreadNum() const
{
    return m_iThreadNum;
}

void GlobalConfiger::SetBackupDir(std::string backupDir)
{
    m_backupDir = backupDir;
}

std::string GlobalConfiger::GetBackupDir() const
{
    return m_backupDir;
}

void GlobalConfiger::SetLogDir(std::string logDir)
{
    m_logDir = logDir;
}

std::string GlobalConfiger::GetLogDir() const
{
    return m_logDir;
}

