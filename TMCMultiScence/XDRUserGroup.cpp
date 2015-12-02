/* 
 * File:   XDRUserGroup.cpp
 * Author: LIU
 * 
 * Created on 2015年11月16日, 下午5:10
 */

#include "XDRUserGroup.h"
#include "XDRScenceGroup.h"


void XDRUserGroup::Init(MultiScence * ms)
{
    this->pms = ms;
    xdrSG.Init(this);
}
    
void XDRUserGroup::set(unsigned long long m_iGroupImsi, st_XDR_INFO* GroupStartPtr, unsigned int size)
{
    this->m_iGroupImsi = m_iGroupImsi;
    this->m_size = size;
    this->m_GroupStartPtr = GroupStartPtr;
    
    m_firstTimeOfTimeSpan.clear();
    xdrSG.set(0, GroupStartPtr, 0);
}

void XDRUserGroup::readOne(st_XDR_INFO* pXDR)
{    
     ++m_size;
     //如果本两分钟分组还没数据，则当前读入为第一条
     if(m_firstTimeOfTimeSpan.count(pXDR->additional_part.nStartTime / TWO_MINUTE) == 0)
     {
         m_firstTimeOfTimeSpan[pXDR->additional_part.nStartTime / TWO_MINUTE] = pXDR->additional_part.nStartTime;
     }
    if(m_size == 1)
    {
        xdrSG.set(pXDR->additional_part.nGroupID, pXDR, 0);
        xdrSG.readOne(pXDR);
        return ;
    }
    if (pXDR->additional_part.nGroupID == xdrSG.getIGroupID() )//如果组号相同，则推入本组；否则处理本组，清空组，改变组号。
    {
        xdrSG.readOne(pXDR);
    }
    else 
    {        
        //处理组
        processGroup(&xdrSG);
        //重置并新开组
        xdrSG.set(pXDR->additional_part.nGroupID, pXDR, 0);
        xdrSG.readOne(pXDR);
    }
    
}

void XDRUserGroup::calc()
{
    //处理组
    processGroup(&xdrSG);
    
    //修正
    if(m_size > 0)
        fix();
}

int XDRUserGroup::processGroup(XDRScenceGroup* pXDRSG)
{
    pXDRSG->calc(m_firstTimeOfTimeSpan);
    return 0;
}

XDRUserGroup::XDRUserGroup()
{
}

//修正

int XDRUserGroup::fix()
{
    if(m_size == 0)return -1;
    st_XDR_INFO* p = m_GroupStartPtr + m_size - 1;//
    st_XDR_INFO* pLastXDR = p;//前一条xdr
    
    //如果是室外用户，需要再进入分组判场景
    if(p->additional_part.cCoveringProperties == USER_OUTDOOR)
    {
        xdrSG.set(p->additional_part.nGroupID, p, 0);
        xdrSG.readOne(p);
    }
    while(p != m_GroupStartPtr)
    {
        --p;
        //读到新分组xdr
        if(p->additional_part.nGroupID != pLastXDR->additional_part.nGroupID)
        {
            //如果上个分组是室外，计算上个分组的场景。
            if(pLastXDR->additional_part.cCoveringProperties == USER_OUTDOOR)
            {
                xdrSG.setGroupStartPtr(pLastXDR);
                xdrSG.outdoorScienceAfterFix(); 
            }
            //如果本场景是室外，开新组。
            if(p->additional_part.cCoveringProperties == USER_OUTDOOR)
            {
                xdrSG.set(p->additional_part.nGroupID, p, 0);
                xdrSG.readOne(p);
            }
            //相邻的两个室外场景，并且有相同2min分组的xdr,并且比读到的上一条速度的小，修正速度
            if(p->additional_part.cCoveringProperties == USER_OUTDOOR
               && pLastXDR->additional_part.cCoveringProperties == USER_OUTDOOR
               && p->additional_part.nTimeGroup == pLastXDR->additional_part.nTimeGroup
               && p->additional_part.cCoveringChildProperties < pLastXDR->additional_part.cCoveringChildProperties)
            {
                p->additional_part.cCoveringChildProperties = pLastXDR->additional_part.cCoveringChildProperties;
                pLastXDR = p;
                while(p != m_GroupStartPtr)
                {
                    --p;
                    
                    if(p->additional_part.nGroupID == pLastXDR->additional_part.nGroupID)
                    {
                        xdrSG.readOne(p);
                        p->additional_part.cCoveringChildProperties = pLastXDR->additional_part.cCoveringChildProperties;
                    }
                    else
                    {                        
                        ++p;
                        break;
                    }                    
                }
            }
            //如果室外+室内同时间分组的,前室外后室内, 全判断为室内, 后者场景填充至前面；
            //临界的判定为同经纬度,全判断为室内, 后者场景填充至前面
            else if(p->additional_part.cCoveringProperties == USER_OUTDOOR
                    && pLastXDR->additional_part.cCoveringProperties == USER_INDOOR
                    && p->additional_part.dLatitude == pLastXDR->additional_part.dLatitude
                    && p->additional_part.dLongitude == pLastXDR->additional_part.dLongitude)
            {
                      //
                p->additional_part.cCoveringProperties = pLastXDR->additional_part.cCoveringProperties;
                p->additional_part.cCoveringChildProperties = pLastXDR->additional_part.cCoveringChildProperties;
                p->additional_part.cScenceProperties = pLastXDR->additional_part.cScenceProperties;

                pLastXDR = p;
                while(p != m_GroupStartPtr)
                {
                    --p;
                    
                    if(p->additional_part.nGroupID == xdrSG.getIGroupID())
                    {
                        xdrSG.readOne(p);
                    }
                    else
                    {
                        xdrSG.setGroupStartPtr(pLastXDR);
                        xdrSG.outdoorScienceAfterFix(); 
                         //如果本场景是室外，开新组。
                        if(p->additional_part.cCoveringProperties == USER_OUTDOOR)
                        {
                            xdrSG.set(p->additional_part.nGroupID, p, 0);
                            xdrSG.readOne(p);
                        }
                    }
                   
                    if(p->additional_part.dLatitude == pLastXDR->additional_part.dLatitude
                       && p->additional_part.dLongitude == pLastXDR->additional_part.dLongitude)
                    {
                        p->additional_part.cCoveringProperties = pLastXDR->additional_part.cCoveringProperties;
                        p->additional_part.cCoveringChildProperties = pLastXDR->additional_part.cCoveringChildProperties;
                        p->additional_part.cScenceProperties = pLastXDR->additional_part.cScenceProperties;
                    }
                    else
                    {
                        ++p;
                        break;
                    }

                    pLastXDR = p;
                }        
            }
        }
        else//不是新分组
        {
            //如果是室外用户，需要再进入分组判场景
            if(p->additional_part.cCoveringProperties == USER_OUTDOOR 
               && p->additional_part.nGroupID == xdrSG.getIGroupID())
            {
                xdrSG.readOne(p);
                //xdrSG.setGroupStartPtr(p);
            }
        }
        pLastXDR = p;
    }
     if(pLastXDR->additional_part.cCoveringProperties == USER_OUTDOOR)
    {
        xdrSG.setGroupStartPtr(pLastXDR);
        xdrSG.outdoorScienceAfterFix(); 
    }
    
    
    //修正：两个相邻室外场景，同2min的，场景全部修正为前边
    p = m_GroupStartPtr;
    pLastXDR = p;
    for(unsigned int i = 0;i < m_size;++i)
    {
        p = m_GroupStartPtr + i;
        if(p->additional_part.cCoveringProperties == USER_OUTDOOR
                && pLastXDR->additional_part.cCoveringProperties == USER_OUTDOOR
                && p->additional_part.nTimeGroup == pLastXDR->additional_part.nTimeGroup)
        {
            p->additional_part.cScenceProperties = pLastXDR->additional_part.cScenceProperties;
        }
        pLastXDR = p;
    }
    return 0;
}



XDRUserGroup::XDRUserGroup(const XDRUserGroup& orig)
{
}

XDRUserGroup::~XDRUserGroup()
{
}

void XDRUserGroup::SetSize(unsigned int size)
{

    m_size = size;
}

unsigned int XDRUserGroup::GetSize() const
{

    return m_size;
}

void XDRUserGroup::SetGroupStartPtr(st_XDR_INFO* GroupStartPtr)
{

    m_GroupStartPtr = GroupStartPtr;
}

st_XDR_INFO* XDRUserGroup::GetGroupStartPtr() const
{

    return m_GroupStartPtr;
}

void XDRUserGroup::setIGroupImsi(unsigned long long iGroupImsi)
{
    m_iGroupImsi = iGroupImsi;
}

unsigned long long XDRUserGroup::getIGroupImsi() const
{
    return m_iGroupImsi;
}

void XDRUserGroup::SetPms(MultiScence* pms)
{
    this->pms = pms;
}

MultiScence* XDRUserGroup::GetPms() const
{
    return pms;
}


