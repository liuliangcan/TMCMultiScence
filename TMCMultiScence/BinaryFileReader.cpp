/* 
 * File:   BinaryFileReader.cpp
 * Author: LIU
 * 
 * Created on 2015年11月13日, 上午11:11
 */

#include "BinaryFileReader.h"
#include "GlobalConfiger.h"
#include "tools/log.h"
#include "tools/dir_info.h"
#include "tools/toolkit.h"
#include <unistd.h>

#include <fstream>

using namespace std;


BinaryFileReader::BinaryFileReader()
{
    m_iAvailableXDRNum = 0;
    GlobalConfiger *gc = GlobalConfiger::GetInstance();
    tarDir = gc->GetOutputDir();
    backupDir = gc->GetBackupDir();
    this->pXDR = NULL;
}
BinaryFileReader::BinaryFileReader(std::string srcFilename)
{
    this->srcFilename = srcFilename;
}
BinaryFileReader::BinaryFileReader(const BinaryFileReader& orig)
{
}

BinaryFileReader::~BinaryFileReader()
{
    if(pXDR != NULL)delete []pXDR;
}

//读取此二进制文件，复制到内存数组pXDR出，*注意此函数new内存，需要释放*pXDR = new st_XDR_INFO[return_xdr_size];
//无需外部释放，析构函数时进行释放
//返回结构体个数
int BinaryFileReader::file2struct(st_XDR_INFO* &pXDR)
{
    pXDR = this->pXDR;
    ifstream infile((srcFilename + DEALING_TMP_STRING).c_str(),  ios::in|ios::binary); 
    if(!infile.is_open())
    {
        LOG_ERROR("[BinaryFileReader]文件打开失败[%sDEALING_TMP_STRING]",srcFilename.c_str());
        return -1;
    }
    unsigned int struct_size = sizeof(st_XDR_INFO);
    //获取目标文件名和备份文件名
    getTargetAndBackupFileName();
    
    //获取文件大小
    unsigned long long start, end;
    start = infile.tellg();
    infile.seekg(0, ios::end);
    end = infile.tellg();
    //本文件中数据个数为文件大小除以结构体大小
    m_file_size = end - start;
    
    //如果文件大小错误
    if(m_file_size < struct_size  || m_file_size % struct_size != 0)
    {
        LOG_ERROR("[BinaryFileReader]文件格式错误%s[%lld byte]", srcFilename.c_str(), m_file_size);
        //将源文件转移
        BackFile();
    if(infile.is_open())
    {
        infile.close();
    }
        return -1;
    }
    m_XDRNum = (m_file_size) / struct_size;
    
    if(m_XDRNum > m_iAvailableXDRNum)
    {
        m_iAvailableXDRNum = m_XDRNum;
        if(pXDR != NULL)delete []pXDR;
        
        short retry = 3;
        //申请内存，重试三次
        pXDR = new(nothrow) st_XDR_INFO[m_iAvailableXDRNum];//不需要函数外部释放
        while(pXDR == NULL && retry--)
        {
            sleep(1);
            pXDR = new(nothrow) st_XDR_INFO[m_iAvailableXDRNum];
        }
        if(pXDR == NULL && retry == 0)
        {
            LOG_ERROR("[BinaryFileReader]内存申请错误%s[%lld byte]", srcFilename.c_str(), m_file_size);
            m_iAvailableXDRNum = 0;
            this->pXDR = pXDR;
    if(infile.is_open())
    {
        infile.close();
    }
            return -1;
        }
        this->pXDR = pXDR;
    }
    
    
    infile.seekg(ios::beg);    
     infile.read( (char *)pXDR, m_file_size);
    /*for(int i = 0;i < return_xdr_size && !infile.eof();++i)
    {
        //char * pChar = (char *)(pXDR + i);
        infile.read( (char *)(pXDR + i), struct_size);
    }
     * */
    if(infile.is_open())
    {
        infile.close();
    }
    
    return m_XDRNum;
}

int BinaryFileReader::struct2file()
{
     if(!F_DirIsExist((char*)(tarDir + "/" + m_day).c_str()))
    {
         int ret = F_MakeDirs((char*)(tarDir + "/" + m_day).c_str(), 1);
         if(ret != 0)
         {
             LOG_ERROR("[BinaryFileReader]文件输出目录创建失败%s", (tarDir + "/" + m_day).c_str());
             return -1;
         }
    }
    string tarFilenameTmp = tarFilename + OUTPUT_TMP_STRING;
    ofstream outfile(tarFilenameTmp.c_str(), ios::out|ios::binary);
    if(!outfile.is_open())
    {
        LOG_ERROR("[BinaryFileReader]文件打开失败[%s]",tarFilenameTmp.c_str());
        return -1;
    }
    outfile.write((char*)pXDR, m_file_size);
//    unsigned int struct_size = sizeof(st_XDR_INFO);
//    for(int i = 0;i < m_XDRNum; ++i)
//    {
//        outfile.write((char*)(pXDR + i), struct_size);
//    }
    if(outfile.is_open())
    {
        outfile.close();
    }
    rename(tarFilenameTmp.c_str(), tarFilename.c_str());
    
    //将源文件转移
    BackFile();
    return 0;
}

void BinaryFileReader::setSrcFilename(std::string srcFilename)
{
    this->srcFilename = srcFilename;
}

void BinaryFileReader::getTargetAndBackupFileName()
{    
    string filename = "";
    string dir = "";
    
    std::size_t backslashIndex = srcFilename.find_last_of('/');
    if(backslashIndex == string::npos)
    {
        filename =  srcFilename;
    }
    else
    {
        filename = srcFilename.substr(backslashIndex + 1);
        dir = srcFilename.substr(0, backslashIndex);
        std::size_t backslashIndex2 = dir.find_last_of('/');
        m_day = dir.substr(backslashIndex2 + 1);
    }
    tarFilename =  tarDir + "/" + m_day + "/sce_" + filename;
    backupFilename = backupDir + "/" + m_day + "/"+ filename;
}

void BinaryFileReader::BackFile()
{
    if(!F_DirIsExist((char*)(backupDir + "/" + m_day).c_str()))
    {
         int ret = F_MakeDirs((char*)(backupDir + "/" + m_day).c_str(), 1);
         if(ret != 0)
         {
             LOG_ERROR("[BinaryFileReader]文件备份目录创建失败%s", (backupDir + "/" + m_day).c_str());
             return;
         }
    }
    rename((srcFilename + DEALING_TMP_STRING).c_str(), backupFilename.c_str());
}