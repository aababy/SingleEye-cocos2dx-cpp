//获取运行环境数据
#ifndef __LOCAL_CONDITION_H__
#define __LOCAL_CONDITION_H__

typedef enum
{
    LOCAL_ID_EN = 0,        //美元
    LOCAL_ID_TAIWAN,        //台湾：新台币NT$
    LOCAL_ID_XIANGGANG,     //香港：港币HK$
    LOCAL_ID_AOMEN,         //澳门：帕塔卡MOP$
    LOCAL_ID_XINJIAPO,      //新加坡：新加坡元S$
    LOCAL_ID_ZHONGGUO,      //中国大陆：人民币
    LOCAL_ID_LIMIT
}ELocalId;

class CLocalCondition
{
public:
    CLocalCondition(){}
    ~CLocalCondition(){}
    
    ELocalId getLocalCondition();
    
    static void toPingLun();
};

#endif // __LOCAL_CONDITION_H__
