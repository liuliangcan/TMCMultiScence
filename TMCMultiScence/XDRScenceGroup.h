/* 
 * File:   XDRScenceGroup.h
 * Author: LIU
 * Descibe: 场景组
 * Created on 2015年11月14日, 下午4:26
 */

#ifndef XDRSCENCEGROUP_H
#define	XDRSCENCEGROUP_H

#include "DataDefine.h"
#include "ScenceEnum.h"

#if __cplusplus < 201103L
#include <tr1/unordered_map>
#include <tr1/unordered_set>
#define MAP tr1::unordered_map
#define SET tr1::unordered_set
#else

#include <unordered_map>
#include <unordered_set>
#define MAP unordered_map
#define SET unordered_set
#endif


#define TWEENTY_MINUTE 1200000.0
#define TWEENTY_KM_PER_HOUR 20.0
#define SIXTY_KM_PER_HOUR 60.0
#define TWO_HUNDRED_METERS 200.0
#define TWO_MINUTE 120000

class XDRUserGroup;

class XDRScenceGroup {
public:
    XDRScenceGroup();
    XDRScenceGroup(const XDRScenceGroup& orig);
    virtual ~XDRScenceGroup();
    
    void Init(XDRUserGroup* xdrug);
    void set(unsigned int GroupID, st_XDR_INFO* GroupStartPtr, unsigned int size);
    
    void readOne(st_XDR_INFO* pXDR);
    
    void calc(std::MAP<unsigned int, unsigned long long> &firstTimeOfTimeSpan );
    
    //室外用户修复完后再判场景并写入xdr
    void outdoorScienceAfterFix();
    
    void setSize(unsigned int size);
    unsigned int getSize() const;
    void setGroupStartPtr(st_XDR_INFO* GroupStartPtr);
    st_XDR_INFO* getGroupStartPtr() const;
    void setIGroupID(unsigned int iGroupID);
    unsigned int getIGroupID() const;
    
private:
    
    std::MAP<unsigned int, unsigned long long> *m_firstTimeOfTimeSpan; 
    
    //调用本类的XDRUserGroup实例指针
    XDRUserGroup* pxdrug;
    
    //室分流程
    int Indoor();
    //室外流程
    int Outdoor();
    
    //设置全部室内外属性
    int SetAllInOutdoor();
    
    //设置全部覆盖子属性
    int SetAllCoveringChildProperties();
    
    //设置全部场景属性
    int SetAllScenceProperties();
    
    //设置所有属性    
    int SetAllProperties();
    
    //判场景
    int AnalysisScence();
    
    float nSpeed;
    CoveringProperties ioPro;           //室内外属性
    CoveringChildProperties childPro;   //子属性
    unsigned int scPro;             //场景属性
    
    CoveringChildProperties m_lastChildPro; //上一组的子属性，用来进行速度修正，如果本组速度小于上一组，则修正为上一组的速度。（这里就开始修正是因为：如果延后修正，需要重新计算场景）
    unsigned long long m_lastLaterTime;//上一组最后一条xdr的时间组号，用来进行速度修正
    
    unsigned int m_iGroupID;       //本组组号
    
    st_XDR_INFO* m_GroupStartPtr; //本组开始指针
    
    unsigned int m_size;          //本组xdr条数
        
    float m_fTotalDistance;   //本组总路程长度
    
    unsigned int m_uTotalTime; //本组xdr总业务时长
    
    unsigned int m_uCellIndoor; //本组有室分小区业务时长
    
    unsigned int m_uCellOutdoor; //本组无室分小区业务时长
    
 //   unsigned int m_uUnknownCell; //本组未知场景小区业务时长
    
//    unsigned int m_uSchoolCell; //本组校园小区业务时长
//    
//    unsigned int m_uLivingCell; //本组居民楼场景小区业务时长
//    
//    unsigned int m_uBussinessCell; //本组商业区场景小区业务时长
//    
//    unsigned int m_uScenicCell; //本组风景区场景小区业务时长
//    
//    unsigned int m_uSubwayCell; //本组地铁场景小区业务时长
//    
//    unsigned int m_uCityRoadCell; //本组城区道路场景小区业务时长
//    
//    unsigned int m_uRoadHighSpeedCell; //本组高速公路场景小区业务时长
//    
//    unsigned int m_uHighSpeedCell; //本组高铁场景小区业务时长
    unsigned int * m_uCustomScence;//本组自定义场景小区业务时长
    unsigned int  m_uCustomScenceSize;//本组自定义场景小区业务时长
};

#endif	/* XDRSCENCEGROUP_H */

