//
//  SceneManager.h
//  SingleEye
//
//  Created by gwsoft on 1/21/14.
//
//

#ifndef __SingleEye__SceneManager__
#define __SingleEye__SceneManager__


#include "GameInclude.h"
#include "Block.h"
#include <vector>

USING_NS_CC;
USING_NS_CC_EXT;


#define xSM     (SceneManager::getInstance())




class SceneManager : public CCNode{
    
public:
    static SceneManager* getInstance();
    
public:
    ~SceneManager();    
    void forward(SceneTag aTag);
    void forward(SceneTag aTag, int iData, void* pData);    //这个pData必须独立于场景, 一直存在, 最好是全局的. 不然会出错, 有特殊需求请给我讲.(luoxp)    
    
    void back();
    void back(SceneTag aTag);       //回退到某个指定的场景, 如Home, 如果没有, 将报错.
    SceneTag getRunningSceneTag();
    SceneTag getLastSceneTag();         //这个函数暂时只能在外面用
    
    //暂时放这里, 以后有可能放到 ResourceManager 里面
    void loadUIPublic();
    void enableTransit();
    void backFromTrans(SceneData &data);        

    void dumpAll();
        
private:
    SceneManager();    
    bool isOldScene();

    void switchToTrans(SceneData &data);
    void switchToLoading(SceneData &data);    
    void replaceScene(SceneData &data); //不要用这个函数, 暂时写这里
    void switchToScene(float dt);
    

    vector<SceneData>   m_vData;
    SceneTag            m_eCurScene;
    bool                m_bTransEnabled;    
    CCScene*            m_pOldScene;
    SceneData           m_eBackup;
    
    Block*              m_block;
};


#endif /* defined(__SingleEye__SceneManager__) */
