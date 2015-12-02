/* 
 * File:   BinaryFileReader.h
 * Author: LIU
 * Describle：二进制文件读取类，按预定义格式读。
 * Created on 2015年11月13日, 上午11:11
 */

#ifndef BINARYFILEREADER_H
#define	BINARYFILEREADER_H

#include "DataDefine.h"
#include <string>


#define DEALING_TMP_STRING "__MSTMP__"
#define OUTPUT_TMP_STRING "__MSOUTPUTTMP__"

class BinaryFileReader {
public:
    BinaryFileReader();
    BinaryFileReader(std::string srcFilename);
    BinaryFileReader(const BinaryFileReader& orig);
    virtual ~BinaryFileReader();
    
    //读取此二进制文件，复制到内存数组pXDR出，*注意此函数new内存，无需外部释放，析构函数时进行释放
    int file2struct(st_XDR_INFO* &pXDR);
    void set(std::string srcFilename, st_XDR_INFO* pXDR);
    //把处理好的结构体数组写入文件；将源文件mv到备份文件夹。
    int struct2file();
    void setSrcFilename(std::string srcFilename);
private:
    //获取目标文件完整路径
    void getTargetAndBackupFileName();
    
    void BackFile();
    
   
    
    //源文件完整路径
    std::string srcFilename;
    //文件上层目录代表的日期(当日的0点时间戳)
    std::string m_day;
    //文件大小
    unsigned long long m_file_size;
     //输出文件完整路径
    std::string tarFilename;
     //备份文件完整路径
    std::string backupFilename;
     //输出目录
    std::string tarDir;
     //备份目录
    std::string backupDir;
    //结构头指针
    st_XDR_INFO* pXDR;
    //结构体数组
    unsigned int m_XDRNum;
    //现在申请的内存能够处理的结构体数组大小
    unsigned int m_iAvailableXDRNum ;
};

#endif	/* BINARYFILEREADER_H */

