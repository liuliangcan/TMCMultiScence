/* 
 * File:   MultiScence.h
 * Author: LIU
 * Describle: 多场景算法类
 * Created on 2015年11月13日, 上午10:56
 */

#ifndef MULTISCENCE_H
#define	MULTISCENCE_H

#include <string>
#include "DataDefine.h"
#include "XDRUserGroup.h"
#include "BinaryFileReader.h"

class MultiScence {
public:
    //初始化函数
    //int Init();
    
    //运算函数
    int Process();
    
    MultiScence();
    MultiScence(const MultiScence& orig);
    virtual ~MultiScence();
    	
    void SetSFile(std::string sFile);
    int GetXdr_size() const;
    st_XDR_INFO* GetXDR() const;
private:
    
    //处理组
    int processUserGroup(XDRUserGroup* pXDRUG);
    
    //要处理的文件路径
    std::string sFile;
    //文件读入内存的xdr数组首地址
    st_XDR_INFO* pXDR;
    //xdr数组大小
    int xdr_size;
    
    //用户组变量
    XDRUserGroup xdrUG;
    
    //文件处理类
    BinaryFileReader bfr;
    
    
};

#endif	/* MULTISCENCE_H */

