/* 
 * File:   GlobalConfiger.h
 * Author: LIU
 * Describe: 配置类
 * Created on 2015年11月18日, 下午12:41
 */

#ifndef GLOBALCONFIGER_H
#define	GLOBALCONFIGER_H


#include <string>
#include <list>

class GlobalConfiger {
public:
    
    int Init(char * confXML);
    
    
    GlobalConfiger();
    GlobalConfiger(const GlobalConfiger& orig);
    virtual ~GlobalConfiger();
    //单例
    static GlobalConfiger *GetInstance()
    {
            if(m_instance_gc == NULL)
            {
                    m_instance_gc = new GlobalConfiger();
            }
            return m_instance_gc;
    }
    void SetIThreadNum(int iThreadNum);
    int GetIThreadNum() const;
    void SetBackupDir(std::string backupDir);
    std::string GetBackupDir() const;
    void SetLogDir(std::string logDir);
    std::string GetLogDir() const;
    void SetListInputDir(std::list<std::string> listInputDir);
    void GetListInputDir(std::list<std::string>& listInputDir) const;
    void SetOutputDir(std::string outputDir);
    std::string GetOutputDir() const;
    unsigned int GetUCustomScenceSize() const;
    unsigned int* GetUCustomScence() const;
private:
    //输入目录
    std::list<std::string> m_listInputDir;
    //输出目录
    std::string m_outputDir;
    //日志输出目录
    std::string m_logDir;
    //备份目录
    std::string m_backupDir;
    //线程数
    int m_iThreadNum;
    
    //自定义场景数组指针
    unsigned int* m_uCustomScence;
    //自定义场景数组大小
    unsigned int m_uCustomScenceSize;
    
    int check();
    //单例
    static GlobalConfiger *m_instance_gc;
};

#endif	/* GLOBALCONFIGER_H */

