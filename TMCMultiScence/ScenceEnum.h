/* 
 * File:   ScenceEnum.h
 * Author: LIU
 *
 * Created on 2015年12月2日, 下午5:55
 */

#ifndef SCENCEENUM_H
#define	SCENCEENUM_H


enum CoveringProperties
{
    USER_INDOOR = 0,     //室内用户
    USER_OUTDOOR = 1     //室外用户
};

//小区覆盖子属性
enum CoveringChildProperties
{
    INDOOR_HOME = 0,            //占用室分的室内稳定用户（有室分）
    INDOOR_OUT = 1,             //占用宏站的室内稳定用户（无室分）
    INDOOR_HOME_OUT = 2,        //室内稳定用户（占用宏站和室分）
    OUTDOOR_SLOW = 3,           //室外低速用户（速度在20公里/小时）
    OUTDOOR_MID = 4,            //室外中速用户（20~60公里/小时）
    OUTDOOR_QUICK = 5           //室外高速用户（20~60公里/小时）
};

//小区场景属性
enum ScenceProperties
{
    UNKNOWN_ECI = 0,            //未知类型
    SCHOOL_ECI = 1,             //校园小区
    LIVING_ECI = 2,             //居民楼小区
    BUSSINESS_ECI = 3,          //商业区
    SCENIC_ECI = 4,             //风景区
    SUBWAY_ECI = 5,             //地铁
    HIGHSPEED_ECI = 6,           //高铁
    CITYROAD_ECI = 7,           //城区道路
    ROADHIGHSPEED_ECI = 8 //高速公路
};

//小区位置属性
enum CellSiteProperties {
    CELL_INDOOR  = 0,                //室内小区
    CELL_OUTDOOR = 1,                //室外小区
    CELL_UNKNOWN = 2                //未知
} ;

#endif	/* SCENCEENUM_H */

