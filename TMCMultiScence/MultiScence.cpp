/* 
 * File:   MultiScence.cpp
 * Author: LIU
 * Describle: 多场景
 * Created on 2015年11月13日, 上午10:56
 */

#include "MultiScence.h"
#include "tools/log.h"
#include <stdio.h>
#include <string.h>


using namespace std;

#define ONE_POS_FLAG_DISTANCE 200  //同一个位置点判别标准:200m

#include "XDRUserGroup.h"


MultiScence::MultiScence()
{
    pXDR = NULL;
    xdrUG.Init(this);
}

MultiScence::MultiScence(const MultiScence& orig)
{
}

MultiScence::~MultiScence()
{
}

void MultiScence::SetSFile(std::string sFile)
{
    this->sFile = sFile;
    pXDR = NULL;
}

int MultiScence::GetXdr_size() const
{
    return xdr_size;
}

st_XDR_INFO* MultiScence::GetXDR() const
{
    return pXDR;
}

//对一个文件的处理过程
int MultiScence::Process()
{
    //文件读入内存
    bfr.setSrcFilename(sFile);
    xdr_size = bfr.file2struct(pXDR);//此过程申请内存数组pXDR，无需释放，由bfr析构时自己管理
    
    if(xdr_size <= 0)
    {
        LOG_ERROR("[MultiScence]文件处理失败！%s",sFile.c_str());
        printf("[MultiScence]文件处理失败！%s\n",sFile.c_str());
        
        return -1;
    }
    

    //遍历每条xdr
    for(int i = 0;i < xdr_size; ++i)
    {
        st_XDR_INFO* xdr = pXDR + i;   
      
        if(i == 0)
        {
            xdrUG.set(xdr->public_part.llImsi, xdr, 0);
            xdrUG.readOne(xdr);
            continue;
        }
        if(xdr->public_part.llImsi == xdrUG.getIGroupImsi())//如果用户相同，则推入本用户组；否则处理本组，清空组，改变用户号。
        {
            xdrUG.readOne(xdr);
        }
        else
        {               
            //处理组
            processUserGroup(&xdrUG);
            
            //重置并新开组
            xdrUG.set(xdr->public_part.llImsi, xdr, 0);
            xdrUG.readOne(xdr);
        }
    }    
     processUserGroup(&xdrUG);
    //输出
    int ret = bfr.struct2file();
    time_t t;
   char timebuf[50] = "";
  time(&t);
   ctime_r(&t, timebuf);
   timebuf[strlen(timebuf) - 1] = '\0';
    if(ret < 0)
    {
        
        printf("%s:[MultiScence]失败处理文件:%s\n",timebuf,sFile.c_str());
        LOG_ERROR("[MultiScence]失败处理文件:%s",sFile.c_str());
    }
    else
    {
        printf("%s:[MultiScence]成功处理文件:%s\n",timebuf,sFile.c_str());
        LOG_NOTICE("[MultiScence]成功处理文件:%s",sFile.c_str());
    }
    
    return 0;
}

//处理组
int MultiScence::processUserGroup(XDRUserGroup* pXDRUG)
{
    pXDRUG->calc();
    return 0;
}
