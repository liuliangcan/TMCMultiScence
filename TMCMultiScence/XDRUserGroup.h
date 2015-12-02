/* 
 * File:   XDRUserGroup.h
 * Author: LIU
 * Describe: xdr按用户组分组
 * Created on 2015年11月16日, 下午5:10
 */

#ifndef XDRUSERGROUP_H
#define	XDRUSERGROUP_H

#include "XDRScenceGroup.h"

class MultiScence;

class XDRUserGroup {
public:
    
    void Init(MultiScence * ms);
    void set(unsigned long long m_iGroupImsi, st_XDR_INFO* GroupStartPtr, unsigned int size);
    
    void readOne(st_XDR_INFO* pXDR);
    
    void calc();
    
    XDRUserGroup();
    XDRUserGroup(const XDRUserGroup& orig);
    virtual ~XDRUserGroup();
    void SetSize(unsigned int size);
    unsigned int GetSize() const;
    void SetGroupStartPtr(st_XDR_INFO* GroupStartPtr);
    st_XDR_INFO* GetGroupStartPtr() const;
    void setIGroupImsi(unsigned long long iGroupImsi);
    unsigned long long getIGroupImsi() const;
    void SetPms(MultiScence* pms);
    MultiScence* GetPms() const;
private:
    //此map储存每个两分钟分组第一条xdr的starttime
    std::MAP<unsigned int, unsigned long long> m_firstTimeOfTimeSpan; 
    
    //调用本类的MultiScence实例的指针
    MultiScence * pms;
    //场景组变量
    XDRScenceGroup xdrSG;
    
    unsigned long long m_iGroupImsi;       //本组imsi
    
    st_XDR_INFO* m_GroupStartPtr; //本组开始指针
    
    unsigned int m_size;          //本组xdr条数
    
    
    //處理組
    int processGroup(XDRScenceGroup* pXDRSG);
    
    //修正
    int fix();

};

#endif	/* XDRUSERGROUP_H */

