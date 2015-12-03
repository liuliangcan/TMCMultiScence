/* 
 * File:   XDRScenceGroup.cpp
 * Author: LIU
 * Descibe: 场景组
 * Created on 2015年11月14日, 下午4:26
 */

#include "XDRScenceGroup.h"
#include <stdio.h>
#include <map>
#include "tools/log.h"
#include "XDRUserGroup.h"
#include "MultiScence.h"
#include "GlobalConfiger.h"


XDRScenceGroup::XDRScenceGroup()
{
    m_lastChildPro = INDOOR_HOME;
    m_lastLaterTime = 0;
    m_uCustomScenceSize = GlobalConfiger::GetInstance()->GetUCustomScenceSize();
    m_uCustomScence = new unsigned int[m_uCustomScenceSize];
    
}

XDRScenceGroup::XDRScenceGroup(const XDRScenceGroup& orig)
{
    
}

XDRScenceGroup::~XDRScenceGroup()
{
    if(m_uCustomScence != NULL) delete[]m_uCustomScence;
}
void XDRScenceGroup::Init(XDRUserGroup* xdrug)
{
    pxdrug = xdrug;
    
}

void XDRScenceGroup::set(unsigned int GroupID, st_XDR_INFO* GroupStartPtr, unsigned int size)
{
    m_GroupStartPtr = GroupStartPtr;
    m_size = size;
    m_iGroupID = GroupID;
    
    nSpeed = 0.0;
    ioPro = USER_INDOOR;
    childPro = INDOOR_HOME;
    scPro = 0;
    m_uCellIndoor = 0;
    m_uCellOutdoor = 0;
    
    for(int i = 0;i < m_uCustomScenceSize;++i)
    {
        m_uCustomScence[i] = 0;
    }
//    m_uUnknownCell = 0;
//    m_uSchoolCell = 0;
//    m_uLivingCell = 0;
//    m_uBussinessCell = 0;
//    m_uScenicCell = 0;
//    m_uSubwayCell = 0;
//    m_uHighSpeedCell = 0;    
//    m_uCityRoadCell = 0;    
//    m_uRoadHighSpeedCell = 0; 
    m_uTotalTime = 0;
    m_fTotalDistance = 0;
}

//读入一条记录
void XDRScenceGroup::readOne(st_XDR_INFO* pXDR)
{
    ++m_size;
 //   printf("ID:%d ,%d\n",this->getIGroupID(),m_size);
    
    m_uTotalTime += pXDR->additional_part.nTimeLength;
    m_fTotalDistance += pXDR->additional_part.fSpliteLength;
    //统计xdr小区室内属性时长
    switch (pXDR->additional_part.cCellIndoorProperties)
    {
        case CELL_INDOOR:
        {
            m_uCellIndoor += pXDR->additional_part.nTimeLength;
            break;
        }
        case CELL_OUTDOOR:
        {
            m_uCellOutdoor += pXDR->additional_part.nTimeLength;
            break;
        }
        default:
        {
            break;
        }
    }
    //统计场景属性时长
    for(int i = 1; i < m_uCustomScenceSize;++i)
    {
        if(pXDR->additional_part.cCellScenceProperties == i)
        {
            m_uCustomScence[i] +=pXDR->additional_part.nTimeLength;
            break;
        }
    }
}

void XDRScenceGroup::calc(std::MAP<unsigned int, unsigned long long> &firstTimeOfTimeSpan)
{
    if(m_size <= 0)return;
    
    m_firstTimeOfTimeSpan = &firstTimeOfTimeSpan;
    //一组中最后一条时间小于20min或者一组中最后一条到基准点距离>200m，则为室外
    //修改：使用最后一条的时间+下一条的时间 ，判断小于20min
    unsigned int time_last = (m_GroupStartPtr + m_size - 1)->additional_part.nTimespan;
    //如果不是本文件最后一条，也不是本用户最后一条
    if(m_GroupStartPtr + m_size - 1 != this->pxdrug->GetPms()->GetXDR() + this->pxdrug->GetPms()->GetXdr_size() - 1
       && (m_GroupStartPtr + m_size)->public_part.llImsi == (m_GroupStartPtr + m_size - 1)->public_part.llImsi
       )
    {
        time_last = time_last + (m_GroupStartPtr + m_size)->additional_part.nTimespan;
    }
    if(time_last <= TWEENTY_MINUTE||(m_GroupStartPtr + m_size - 1)->additional_part.fLengthspan > TWO_HUNDRED_METERS)
    {
        
   // printf("msize:%d shiwai %d %f\n",m_size,(m_GroupStartPtr + m_size - 1)->additional_part.nTimespan, (m_GroupStartPtr + m_size - 1)->additional_part.fLengthspan);
        Outdoor();
    }
    else
    {
        
 //   printf("msize:%d shinei %d %f\n",m_size,(m_GroupStartPtr + m_size - 1)->additional_part.nTimespan, (m_GroupStartPtr + m_size - 1)->additional_part.fLengthspan);
        
        Indoor();
    }
    //覆盖属性分布在室内外内部单独流程中
    
    //判场景
    //AnalysisScence();
    //现改为只有室内才判场景，因此写到Indoor()中
    
    //写入xdr
    SetAllProperties();   
    
    m_lastChildPro = childPro;
    m_lastLaterTime = (m_GroupStartPtr + m_size - 1)->additional_part.nTimeGroup;
}
    //室分流程
int XDRScenceGroup::Indoor()
{
    //设置覆盖属性为室内
    //SetAllInOutdoor(CoveringProperties.USER_INDOOR);
    ioPro = USER_INDOOR;
     unsigned int uFlag = m_uTotalTime * 80;
    if(m_uCellIndoor *100 > uFlag)
    {
        //稳定的有室分
        //SetAllCoveringChildProperties(CoveringChildProperties.INDOOR_HOME);
        childPro = INDOOR_HOME;
    }
    else if(m_uCellOutdoor * 100 > uFlag)
    {
        //无室分
        //SetAllCoveringChildProperties(CoveringChildProperties.INDOOR_OUT);
        childPro = INDOOR_OUT;
    }
    else
    {
        //室分+宏站
        //SetAllCoveringChildProperties(CoveringChildProperties.INDOOR_HOME_OUT);
        childPro = INDOOR_HOME_OUT;
    }
     AnalysisScence();
    return 0;
}
   //室外流程
int XDRScenceGroup::Outdoor()
{
    //设置覆盖属性为室外
    //SetAllInOutdoor(CoveringProperties.USER_OUTDOOR);
    ioPro = USER_OUTDOOR;
    //计算平均速度(km/h)
    unsigned int timespan = ((m_GroupStartPtr + m_size - 1)->additional_part.nStartTime - (m_GroupStartPtr + m_size - 1)->additional_part.nTimespan) / TWO_MINUTE;
    std::MAP<unsigned int, unsigned long long>::iterator it_map = m_firstTimeOfTimeSpan->find(timespan);
    if(it_map == m_firstTimeOfTimeSpan->end())
    {
        LOG_WARN("[XDRScenceGroup]速度计算时两分钟分组时间修正读取失败");  
        timespan = (m_GroupStartPtr + m_size - 1)->additional_part.nTimespan;
    }
    else
    {
        timespan = (m_GroupStartPtr + m_size - 1)->additional_part.nStartTime - it_map->second;
    }
    if(timespan == 0)
    {
        nSpeed == 0.0;
 //       printf( "m_GroupStartPtr + m_size - 1)->additional_part.nTimespan == 0,%f,%d,%lld,%d\n",nSpeed,(m_GroupStartPtr + m_size - 1)->additional_part.nGroupID, (m_GroupStartPtr + m_size - 1)->additional_part.nStartTime,m_size);
    }
    else
    {
        nSpeed = m_fTotalDistance * 1000 / timespan;
        
 //       printf( "m_fTotalDistance:%f ,nSpeed:%f\n",m_fTotalDistance,nSpeed);
    }
    float averageV = nSpeed * 3.6;
    if(0 <= averageV && averageV <= TWEENTY_KM_PER_HOUR)
    {
        //低速用户
        //SetAllCoveringChildProperties(CoveringChildProperties.OUTDOOR_SLOW);
        childPro = OUTDOOR_SLOW;
    }
    else if(TWEENTY_KM_PER_HOUR < averageV && averageV <= SIXTY_KM_PER_HOUR)
    {
        //中速用户
        //SetAllCoveringChildProperties(CoveringChildProperties.OUTDOOR_MID);
        childPro = OUTDOOR_MID;
    }
    else if(SIXTY_KM_PER_HOUR < averageV)
    {
        //高速用户
        //SetAllCoveringChildProperties(CoveringChildProperties.OUTDOOR_QUICK);
        childPro = OUTDOOR_QUICK;
    }
    else
    {
        //速度小于0？
    }
    
    if(childPro < m_lastChildPro && m_lastLaterTime == m_GroupStartPtr->additional_part.nTimeGroup)
    {
        childPro = m_lastChildPro;
    }
    
    return 0;
}
void XDRScenceGroup::outdoorScienceAfterFix()
{
    if(m_size == 0)return;
    ioPro = (CoveringProperties)m_GroupStartPtr->additional_part.cCoveringProperties;
    childPro = (CoveringChildProperties)m_GroupStartPtr->additional_part.cCoveringChildProperties;
    AnalysisScence();
    SetAllScenceProperties();
}

    //设置全部室内外属性
int XDRScenceGroup::SetAllInOutdoor()
{
    for(int i = 0 ;i < m_size;++i)
    {
        (m_GroupStartPtr + i)->additional_part.cCoveringProperties = ioPro;
    }
    return 0;
}

//设置全部覆盖子属性
int XDRScenceGroup::SetAllCoveringChildProperties()
{
    for(int i = 0 ;i < m_size;++i)
    {
        (m_GroupStartPtr + i)->additional_part.cCoveringChildProperties = childPro;
    }
    return 0;
}

//设置全部场景属性
int XDRScenceGroup::SetAllScenceProperties()
{
    for(int i = 0 ;i < m_size;++i)
    {
        //由于目前只有室外场景判断(补全时)用到这个函数，考虑到室外+室内修正:如果是室外遍历到室内 跳出
        if((m_GroupStartPtr + i)->additional_part.cCoveringProperties == USER_INDOOR)
        {
            break;
        }
        (m_GroupStartPtr + i)->additional_part.cScenceProperties = scPro;
    }
    return 0;
}
int XDRScenceGroup::SetAllProperties()
{
    for(int i = 0 ;i < m_size;++i)
    {
        (m_GroupStartPtr + i)->additional_part.cCoveringProperties = ioPro;
        (m_GroupStartPtr + i)->additional_part.cCoveringChildProperties=childPro;
        (m_GroupStartPtr + i)->additional_part.cScenceProperties = scPro;
        (m_GroupStartPtr + i)->additional_part.nSpeed = (int)this->nSpeed;
    }
    return 0;
}

//判场景
int XDRScenceGroup::AnalysisScence()
{
    unsigned int uFlag = m_uTotalTime * 80;
    scPro = 0;
    for(int i = 0;i < m_uCustomScenceSize;++i)
    {
        if(m_uCustomScence[i] * 100 > uFlag
           && GlobalConfiger::GetInstance()->GetUCustomScence()[i] & childPro)
        {
            scPro = i;
            break;
        }
    }
    return 0;
}
void XDRScenceGroup::setSize(unsigned int size)
{
    m_size = size;
}

unsigned int XDRScenceGroup::getSize() const
{
    return m_size;
}

void XDRScenceGroup::setGroupStartPtr(st_XDR_INFO* GroupStartPtr)
{
    m_GroupStartPtr = GroupStartPtr;
}

st_XDR_INFO* XDRScenceGroup::getGroupStartPtr() const
{
    return m_GroupStartPtr;
}

void XDRScenceGroup::setIGroupID(unsigned int iGroupID)
{
    m_iGroupID = iGroupID;
}

unsigned int XDRScenceGroup::getIGroupID() const
{
    return m_iGroupID;
}