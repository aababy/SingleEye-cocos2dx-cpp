//
//  Enemy.cpp
//  SingleEye
//
//  Created by qinfen on 13-11-11.
//
//

#include "Enemy.h"
Enemy::Enemy()
{
    CCLog("enemy!");
}

Enemy::Enemy(Enemy_Data *pData, CCLayer *layer,bool pve)
{
    initData(pData);
    int _atk = pData->mAtk2+DATAPOOL->getEnemyRecord(m_nType)->m_nLevel*pData->mAtkAdd;
    int _skillatk = pData->mAtk2*2+ pData->mSkillAtkAdd*DATAPOOL->getEnemyRecord(m_nType)->m_nLevel;
    m_nAtk = _atk;
    m_nSkillAtk = _skillatk;
    m_pParentLayer =(FightLayer*)layer;
    if(!pve)
    {
        m_nHp*=2;
        m_nCurrentHp*=2;
        m_nAtk*=2;
    }
}

Enemy::Enemy(Enemy_Data *pData,CCLayer *layer,int add,int level)
{
    initData(pData);
    if(isBoss())
    {
        m_nAtk = pData->mAtk2 + pData->mAtkAdd*level;
        m_nSkillAtk = pData->mAtk2*2 + pData->mSkillAtkAdd*level;
        m_nCurrentHp = m_nHp*=add;
        
    }
    else
    {
        m_nAtk = pData->mAtk*add;
        m_nSkillAtk = 0;
        m_nCurrentHp = m_nHp*=add;
    }
    m_pParentLayer =(FightLayer*)layer;
    m_fAccelerateY = 2;
}

Enemy::Enemy(Enemy_Data *pData,SnatchLayer *layer)
{
    initData(pData);
    m_fVel = pData->mVel/100;
    m_bQiangBao = true;
    m_pQiangBaoLayer = layer;
    m_nAtk = 0;
    m_nSkillAtk = 0;
    
    m_gameTime.mTotalTime = 0;
    m_atkFrequency.mTotalTime = 0;
    m_cdTime.mTotalTime = 0;
    m_fAccelerateX = 2;
    
    if(arc4random()%100<80)  //礼品
    {
        m_bLipin = true;
        CCArmature *linp = CCArmature::create("qiangbaotubiao");
        linp->getAnimation()->play("jinbi");
        linp->setPosition(ccp(m_pSprite->getPositionX(), m_pSprite->getPositionY()+50));
        addChild(linp,1,Effect_Flag);
    }
    else  //炸弹
    {
        m_bLipin = false;
        CCArmature *linp =  CCArmature::create("qiangbaotubiao");
        linp->getAnimation()->play("zhadan");
        linp->setPosition(ccp(m_pSprite->getPositionX(), m_pSprite->getPositionY()+50));
        addChild(linp,1,Effect_Flag);
    }
}

Enemy::Enemy(Enemy_Data *pData, CCLayer *layer,int allhp,int lefthp,int defenseAdd)  //defenseAdd 名字为防御加成实则为弹射方的防御加成 这里做成怪方的攻击减成
{
    initData(pData);
    m_nHp = allhp;
    m_nCurrentHp = lefthp;
    int _atk = pData->mAtk2+DATAPOOL->getEnemyRecord(m_nType)->m_nLevel*pData->mAtkAdd;
    int _skillatk = pData->mAtk2*2+ pData->mSkillAtkAdd*DATAPOOL->getEnemyRecord(m_nType)->m_nLevel;
    m_nAtk = _atk;
    m_nSkillAtk = _skillatk;
    m_nAtk *= ((float)(100-defenseAdd))/100;
    m_nSkillAtk *= ((float)(100-defenseAdd))/100;
    m_pParentLayer =(FightLayer*)layer;
    
    m_atkFrequency.mTotalTime = ((pData->mAtkFrequency)/2)/1000.0;
    m_atkFrequency.mTotalTime+= CCRANDOM_0_1();
    
    m_cdTime.mTotalTime = ((pData->mCD)/2)/1000.0;
    m_cdTime.mTotalTime += CCRANDOM_0_1();
    m_fAccelerateY = 2;
}

void Enemy::initData(Enemy_Data *pData)
{
    m_pSprite = NULL;
    m_nType = pData->mType;
    m_nHp = pData->mHp;
    m_nCurrentHp = pData->mHp;
    m_fVel = pData->mVel;
    m_nGold = pData->mGold;
    m_nDistance = pData->mDistance;
    m_nState = 0;
    m_fAccelerateY = 2;
    m_bUseSkill = false;
    m_bRange = pData->mRange;
    lastHitMe = 0;
    m_fAward = pData->mFrequency;
    isEat = false;
    isJiaCheng = false;
    isFuHuo = false;
    m_nFuHuoHp = pData->mHp;
    bossFirstSkill = true;
    m_bHuDun = false;
    m_bJiaCheng = false;
    m_bMoveFree = false;
    m_bQiangBao = false;
    m_pQiangBaoLayer = NULL;
    m_bLipin = false;
    m_nHeroAtk = 0;
    m_bMoveFreeComplete = true;
    m_bMoveFreeDirection = 0;
    m_bUseCDSkill = false;
    m_bUseAtkSkill = false;
    m_bMoveUDDirection = 0;
    m_bMoveLRBack = true;
    m_bMoveLRArrival = false;
    m_fLRPosX = 0;
    m_fOriginalPosX = 0;
    m_fUDPosY = 0;
    m_bMoveUDArrival = false;
    m_nCurrentPosIndex = 1;
    m_fOriginalPosY = 0;
    m_bRecordPos = false;
    m_bRebound = false;
    m_bIsSlowAction = true;
    m_bHurtCount = 0;
    m_bPowerAtk = false;
    m_bMuBeiHit = false;
    m_pCurrentState = NULL;
    m_bBoom = false;
    m_pData = pData;
    
    sprintf(m_strHead,"%s",pData->mSourceName);
    CCLOG("m_strHead:%s",m_strHead);
    m_pSprite = cocos2d::extension::CCArmature::create(m_strHead);
    if(m_nType == Enemy_Type_iBoss)
    {
        m_pSprite->getAnimation()->play("daiji_1");
    }
    else if(m_nType == Enemy_Type_jBoss)
    {
        m_pSprite->getAnimation()->play("daiji_2");
    }
    else
    {
        m_pSprite->getAnimation()->play("daiji");
    }
    addChild(m_pSprite);
    
    m_gameTime.mCurTime = 0.0f;
    m_gameTime.mTotalTime = m_fVel;
    
    m_atkFrequency.mCurTime = 0;
    m_atkFrequency.mTotalTime = (pData->mAtkFrequency)/1000.0;
    m_atkFrequency.mTotalTime+= CCRANDOM_0_1();
    
    m_moveLRTime.mCurTime = 0;
    m_moveLRTime.mTotalTime = 0;
    
    m_cdTime.mCurTime = 0;
    m_cdTime.mTotalTime = (pData->mCD)/1000.0;
    m_cdTime.mTotalTime += CCRANDOM_0_1();
    
    m_SlowStopTime.mCurTime = 0;
    m_SlowStopTime.mTotalTime = 0;
    
    m_comboTime.mCurTime = 0;
    m_comboTime.mTotalTime = COMBO_TIME;
    
    if (m_nType==Enemy_Type_XueYuGangXie)  //螃蟹
    {
        m_fAccelerateX = 2;
        
        if(arc4random()%2)
            m_fAccelerateX *=-1;
    }
    else if(m_nType == Enemy_Type_ShuiJingZhangYu)  //章鱼
    {
        m_nDistance += (arc4random()%101 - 50);
        m_fAccelerateX = 2;
    }
    else
    {
        m_fAccelerateX = 2;
    }
    
    if(isBoss())
    {
        int dy = BLOOD_DY;
        if(m_nType==Enemy_Type_bBoss)
        {
            dy -= 20;
        }
        CCSprite *progressHp = CCSprite::createWithSpriteFrameName(Hp_Boss_Frame);
        progressHp->setPosition(ccp(m_pSprite->getPositionX(),m_pSprite->getPositionY() +m_pSprite->getContentSize().height +dy));
        addChild(progressHp,1,Effect_Hp_Progress);
        
        
        CCProgressTimer *barHp = CCProgressTimer::create(CCSprite::createWithSpriteFrameName(Hp_Boss_Bar));
        barHp->setPosition(ccp(m_pSprite->getPositionX(),m_pSprite->getPositionY() +m_pSprite->getContentSize().height +dy));
        barHp->setType(kCCProgressTimerTypeBar);
        barHp->setMidpoint(ccp(0, 0));
        barHp->setBarChangeRate(ccp(1, 0));
        addChild(barHp,1,Effect_Hp_Bar);
        
        float percent = (float)m_nCurrentHp / (float)m_nHp;
        barHp->setPercentage(percent*100);
        m_fAccelerateX = 4;
        m_fAccelerateY = 4;
    }
    if(m_nType == Enemy_Type_HeiTangEMo)
        m_bRebound = true;
    CreateFsmSys();
}

void Enemy::onEnter()
{
    CCNode::onEnter();
}

void Enemy::onExit()
{
    removeAllChildrenWithCleanup(true);
    CCNode::onExit();
}


Enemy::~Enemy()
{
    unscheduleAllSelectors();
    m_pParentLayer = NULL;
    m_pQiangBaoLayer = NULL;
    CC_SAFE_DELETE(m_pFSMSystem);
    m_pCurrentState = NULL;
    m_pData = NULL;
    lastHitMe = 0;
}

bool Enemy::CreateFsmSys()
{
    m_pFSMSystem = new FSMSystem<Enemy>(this);
    if (m_pFSMSystem)
    {
        scheduleUpdate();
        return true;
    }
    return false;
}

void Enemy::update(float delta)
{
    if (m_pFSMSystem) {
        m_pFSMSystem->Update(delta);
    }
    
    if(!m_pQiangBaoLayer&&m_pParentLayer->isShowTeachLayer())return;
    
    if(m_nType==Enemy_Type_ShaMoFeiLong&&m_nState!=Enemy_State_Stop&&m_nState!=Enemy_State_SlowDown)
    {
        CCArray* bullets =  m_pParentLayer->getBulletArray();
        for (int i = 0; i<bullets->count(); ++i)
        {
            if (searchTarget((CCNode*)(bullets->objectAtIndex(i))))
            {
                EnemyDodgeState *_instance = EnemyDodgeState::getInstance();
                _instance->setLeft(false);
                if(((Hero*)bullets)->getAccelerateX()>0)
                {
                    _instance->setLeft(true);
                }
                GetFSMSystem()->ChangeState(_instance);
            }
        }
    }
    
    enemyCDUpdate(delta);
    enemyAtkUpdata(delta);
    
    if(m_bJiaCheng)
        jiaChengUpdate(delta);
    
    if(m_bHurtCount>0)
    {
        m_comboTime.mCurTime+=delta;
        if(m_comboTime.mCurTime>=m_comboTime.mTotalTime)
        {
            m_comboTime.mCurTime = 0;
            m_bHurtCount = 0;
        }
    }
}

bool Enemy::searchTarget(cocos2d::CCNode *target)
{
    CCPoint enemyPos = this->getPosition();
    CCPoint targetPos = target->getPosition();
    if(ccpDistance(targetPos, enemyPos)<150)
        return true;
    return false;
}

void Enemy::moveUpdate(float dt)
{
    if(!m_pQiangBaoLayer&&m_pParentLayer->isShowTeachLayer())return;
    if(m_nState==Enemy_State_Die)return;
    m_gameTime.mCurTime+=dt;
    if(!m_bMoveFreeComplete)m_moveLRTime.mCurTime+=dt;
    switch (m_nType)
    {
        case Enemy_Type_FengXiYi:
        {
            if (m_gameTime.mCurTime>=m_gameTime.mTotalTime/1000)
            {
                m_gameTime.mCurTime = 0.0;
                EnemyJumpState *_instance = EnemyJumpState::getInstance();
                GetFSMSystem()->ChangeState(_instance);
            }
        }
            break;
        case Enemy_Type_GeBuLin:
        case Enemy_Type_JuXingGeBuLin:
        case Enemy_Type_RuanNiGuai:
        case Enemy_Type_XueYuGangXie:
        case Enemy_Type_ShuiJingZhangYu:
        case Enemy_Type_ChangMaoXueGuai:
        case Enemy_Type_BingBaoHeTun:
        case Enemy_Type_ShaMoJiaChong:
        case Enemy_Type_HuangMoGeBuLin:
        case Enemy_Type_ShaMoFeiLong:
        case Enemy_Type_ShaBaoJuRen:
        case Enemy_Type_XiaoGui:
        case Enemy_Type_DiYuMoMeng:
        case Enemy_Type_DiYuFengQuan:
        case Enemy_Type_LingHunShouGeZhe:
        case Enemy_Type_JiaoTangKuiLei:
        case Enemy_Type_YouEMo:
        case Enemy_Type_TangXinMo:
        case Enemy_Type_HeiTangEMo:
        {
            if (m_gameTime.mCurTime>=m_gameTime.mTotalTime/1000)
            {
                m_gameTime.mCurTime = 0.0;
                float _x = this->getPositionX();
                float _y = this->getPositionY();
                if(!m_bMoveFree)
                {
                    if(_y- m_fAccelerateY>Barrier_Y+(m_nDistance*DATAPOOL->zoom))
                    {
                     _y -= m_fAccelerateY;
                    }
                    else
                    {
                        if(m_pQiangBaoLayer)
                        {
                            _y -= m_fAccelerateY;
                        }
                        else
                        {
                            
                            _y -= (_y-Barrier_Y-(m_nDistance*DATAPOOL->zoom));
                        }
                    }
                    _x += getMoveLRAccelerateX();
                    this->setPosition(ccp(_x, _y));
                }
                else
                {
                    if(m_bMoveFreeComplete)
                    {
                        m_moveLRTime.mTotalTime = arc4random()&4+2;
                        m_moveLRTime.mCurTime = 0;
                        m_bMoveFreeComplete = false;
                        do {
                            m_bMoveFreeDirection = arc4random()%3;
                            m_bMoveFreeDirection--;
                            m_bMoveUDDirection = arc4random()%3;
                            m_bMoveUDDirection--;
                            if(MAX_POS_Y-_x<=20&&arc4random()%3>0)
                            {
                                m_bMoveUDDirection = -1;
                            }
                        } while (0==m_bMoveFreeDirection);
                    }
                    else
                    {
                        if(m_moveLRTime.mCurTime<m_moveLRTime.mTotalTime)
                        {
                            if(_x+m_fAccelerateX*m_bMoveFreeDirection>=MIN_POS_X&&_x+m_fAccelerateX*m_bMoveFreeDirection<=MAX_POS_X)
                            {
                                _x+=m_fAccelerateX*m_bMoveFreeDirection;
                            }
                            
                            if(_y+m_fAccelerateY*m_bMoveUDDirection>MIN_POS_Y&&_y+m_fAccelerateY*m_bMoveUDDirection<MAX_POS_Y)
                            {
                                _y+=m_fAccelerateY*m_bMoveUDDirection;
                            }
                            
                            if(_x<MIN_POS_X)_x = MIN_POS_X;
                            if(_x>MAX_POS_X)_x = MAX_POS_X;
                            if(_y<MIN_POS_Y)_y = MIN_POS_Y;
                            if(_y>MAX_POS_Y)_y = MAX_POS_Y;
                            this->setPosition(ccp(_x, _y));
                        }
                        else
                        {
                            m_bMoveFreeComplete = true;
                        }
                    }
                }
            }
        }
            break;
        case Enemy_Type_aBoss:
        case Enemy_Type_bBoss:
        case Enemy_Type_cBoss:
        case Enemy_Type_dBoss:
        case Enemy_Type_eBoss:
        case Enemy_Type_fBoss:
        case Enemy_Type_gBoss:
        case Enemy_Type_hBoss:
        case Enemy_Type_iBoss:
        case Enemy_Type_jBoss:
        {
            if (m_gameTime.mCurTime>=m_gameTime.mTotalTime/1000)
            {
                m_gameTime.mCurTime = 0.0;
                float _x = this->getPositionX();
                float _y = this->getPositionY();
                if(!m_bMoveFree)
                {
                    _y -= m_fAccelerateY;
                    this->setPosition(ccp(_x, _y));
                }
                else
                {
                    if(m_bMoveUDArrival&&m_bMoveLRArrival)
                    {
                        getNextPos();
                        m_bMoveUDArrival = false;
                        m_bMoveLRArrival = false;
                    }
                    else
                    {
                        _x += getMoveLRAccelerateXBoss();
                        _y += getMoveLRAccelerateYBOss();
                        this->setPosition(ccp(_x, _y));
                    }
                }
            }
        }
        break;
        default:
            break;
    }
    
    if(m_pQiangBaoLayer&&this->getPositionY()<0)
    {
        if(m_bLipin)m_pQiangBaoLayer->addEscape();
        this->removeFromParentAndCleanup(true);
        m_pQiangBaoLayer->removeEnemyFromArray(this);
    }
        
}

float Enemy::getMoveLRAccelerateXBoss()
{
    if(m_bMoveLRArrival)return 0;
    float _x = this->getPositionX();
    if(_x <= m_fLRPosX)  //向右移动
    {
        if(_x + m_fAccelerateX >= m_fLRPosX)
        {
            m_bMoveLRArrival = true;
            return (m_fLRPosX - _x);
        }
        else
        {
            return m_fAccelerateX;
        }
    }
    else
    {
        if(_x - m_fAccelerateX <= m_fLRPosX)
        {
            m_bMoveLRArrival = true;
            return (_x - m_fLRPosX);
        }
        else
        {
            return - m_fAccelerateX;
        }
    }
}

float Enemy::getMoveLRAccelerateYBOss()
{
    if(m_bMoveUDArrival)return 0;
    float _y = this->getPositionY();
    if(_y <= m_fUDPosY)  //向上移动
    {
        if(_y + m_fAccelerateY >= m_fUDPosY)
        {
            m_bMoveUDArrival = true;
            return (m_fUDPosY - _y);
        }
        else
        {
            return m_fAccelerateY;
        }
    }
    else
    {
        if(_y - m_fAccelerateY <= m_fUDPosY)
        {
            m_bMoveUDArrival = true;
            return (_y - m_fUDPosY);
        }
        else
        {
            return -m_fAccelerateY;
        }
    }
}

void Enemy::getNextPos()
{
    int index = 4;
    switch (m_nCurrentPosIndex)
    {
        case 1:
        {
            do
            {
                index = arc4random()%7;
                index++;
            } while (index==5||index==7);
        }
            break;
        case 2:
        {
            do
            {
                index = arc4random()%7;
                index++;
            } while (index==4||index==8);
        }
            break;
        case 3:
        {
            do
            {
                index = arc4random()%7;
                index++;
            } while (index==6||index==8||index == 7);
        }
            break;
        case 4:
        {
            do
            {
                index = arc4random()%7;
                index++;
            } while (index==2||index==8||index==6||index == 7);
        }
            break;
        case 5:
        {
            do
            {
                index = arc4random()%7;
                index++;
            } while (index==1||index==8||index==6||index==7);
        }
            break;
        case 6:
        {
            do
            {
                index = arc4random()%7;
                index++;
            } while (index==3||index==4||index==5);
        }
            break;
        case 7:
        {
            do
            {
                index = arc4random()%7;
                index++;
            } while (index==1||index==3||index==4||index==5);
        }
            break;
        case 8:
        {
            do
            {
                index = arc4random()%7;
                index++;
            } while (index==2||index==3||index==4||index==5);
        }
            break;
        default:
            break;
    }
    
    switch (index) {
        case 1:
        {
            m_fLRPosX = m_fOriginalPosX;
            m_fUDPosY = m_fOriginalPosY;
        }
            break;
        case 2:
        {
            m_fLRPosX = m_fOriginalPosX;
            float pos = m_fOriginalPosY - MOVE_DELTA_Y;
            m_fUDPosY = pos>MIN_POS_Y&&pos<MAX_POS_Y?pos:m_fUDPosY;
        }
            break;
        case 3:
        {
            float posx = m_fOriginalPosX - MOVE_DELTA_X/2;
            float posy = m_fOriginalPosY - MOVE_DELTA_Y/2;
            m_fLRPosX = posx>MIN_POS_X&&posx<MAX_POS_X?posx:m_fLRPosX;
            m_fUDPosY = posy>MIN_POS_Y&&posy<MAX_POS_Y?posy:m_fUDPosY;
        }
            break;
        case 4:
        {
            float posx = m_fOriginalPosX - MOVE_DELTA_X;
            m_fLRPosX = posx>MIN_POS_X&&posx<MAX_POS_X?posx:m_fLRPosX;
            m_fUDPosY = m_fOriginalPosY;
        }
            break;
        case 5:
        {
            float posx = m_fOriginalPosX - MOVE_DELTA_X;
            float posy = m_fOriginalPosY - MOVE_DELTA_Y;
            m_fLRPosX = posx>MIN_POS_X&&posx<MAX_POS_X?posx:m_fLRPosX;
            m_fUDPosY = posy>MIN_POS_Y&&posy<MAX_POS_Y?posy:m_fUDPosY;
        }
            break;
        case 6:
        {
            float posx = m_fOriginalPosX + MOVE_DELTA_X/2;
            float posy = m_fOriginalPosY - MOVE_DELTA_Y/2;
            m_fLRPosX = posx>MIN_POS_X&&posx<MAX_POS_X?posx:m_fLRPosX;
            m_fUDPosY = posy>MIN_POS_Y&&posy<MAX_POS_Y?posy:m_fUDPosY;
        }
            break;
        case 7:
        {
            float posx = m_fOriginalPosX + MOVE_DELTA_X;
            float posy = m_fOriginalPosY - MOVE_DELTA_Y;
            m_fLRPosX = posx>MIN_POS_X&&posx<MAX_POS_X?posx:m_fLRPosX;
            m_fUDPosY = posy>MIN_POS_Y&&posy<MAX_POS_Y?posy:m_fUDPosY;
        }
            break;
        case 8:
        {
            float posx = m_fOriginalPosX + MOVE_DELTA_X;
            m_fLRPosX = posx>MIN_POS_X&&posx<MAX_POS_X?posx:m_fLRPosX;
            m_fUDPosY = m_fOriginalPosY;
        }
            break;
        default:
            break;
    }
    m_nCurrentPosIndex = index;
}

float Enemy::getMoveLRAccelerateX()
{
    if(m_nType == Enemy_Type_HeiTangEMo || m_nType == Enemy_Type_LingHunShouGeZhe||m_nType==Enemy_Type_ShaMoFeiLong)
        return 0;
    if(!m_bRange)
        return 0;
    if(m_bMoveLRBack)
    {
        do {
            int _dx = arc4random()%150;
            _dx -= 75;
            m_fLRPosX = m_fOriginalPosX + _dx;
        } while (m_fLRPosX<MIN_POS_X && m_fLRPosX>MAX_POS_X);
        CCLOG("%d %f",MIN_POS_X,MAX_POS_X);
        
        m_bMoveLRBack = false;
    }
    float _x = this->getPositionX();
    if(!m_bMoveLRArrival)
    {
        
        if(m_fLRPosX>m_fOriginalPosX)  //向右移动
        {
          if(_x+ m_fAccelerateX >=m_fLRPosX)
          {
              m_bMoveLRArrival = true;
              return (m_fLRPosX - _x);
          }
          else
          {
              return m_fAccelerateX;
          }
        }
        else
        {
            if(_x - m_fAccelerateX <= m_fLRPosX)
            {
                m_bMoveLRArrival = true;
                return  (m_fLRPosX - _x);
            }
            else
            {
                return -m_fAccelerateX;
            }
        }
    }
    else
    {
        if(m_fLRPosX > m_fOriginalPosX) //向右移动
        {
            if(_x - m_fAccelerateX <= m_fOriginalPosX)
            {
                m_bMoveLRArrival = false;
                m_bMoveLRBack = true;
                return (m_fOriginalPosX - _x);
            }
            else
            {
                return -m_fAccelerateX;
            }
        }
        else
        {
            if(_x + m_fAccelerateX >= m_fOriginalPosX)
            {
                m_bMoveLRBack = true;
                m_bMoveLRArrival = false;
                return (_x - m_fOriginalPosX);
            }
            else
            {
                return m_fAccelerateX;
            }
        }
    }
}

void Enemy::atkTimeUpdate(float dt)
{
    
}

void Enemy::addPoints(Fight_Wave_Element *pData)
{
    int _x = pData->mArrayPoint.x;
    int _y = pData->mArrayPoint.y;
    m_fOriginalPosX = (float)_x;
    _y = _y * DATAPOOL->zoom;
    this->setPosition(ccp(_x,_y));
    if(isCanAtk()&&m_bRange)
        m_bMoveFree = true;
}

void Enemy::addPoint(CCPoint pos)
{
    int _x = pos.x;
    int _y = pos.y;
    m_fOriginalPosX = (float)_x;

    _y = _y * DATAPOOL->zoom;
    this->setPosition(ccp(_x,_y));
    if(isCanAtk()&&m_bRange)
        m_bMoveFree = true;
}

void Enemy::enemyAtk()
{
   m_pParentLayer->minusBarrierHp(this->getAtk());
   m_pParentLayer->playBarrierUnderAtkAni();
    if(m_nType == Enemy_Type_BingBaoHeTun || m_nType == Enemy_Type_YouEMo)
    {
        CCParticleSystem *emitter = NULL;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        emitter =  CCParticleSystemQuad::create("explode.plist");
#else
        emitter = CCParticleSystemQuad::create("particle/explode.plist");
#endif
        emitter->setAutoRemoveOnFinish(true);
        emitter->setPosition(m_pSprite->getPosition());
        addChild(emitter,100);
    }
}

void Enemy::enemyHurt(bool isHurt)
{
    if(m_nState == Enemy_State_MuBei)return;
    if (m_nCurrentHp<=0)
    {
        this->setState(Enemy_State_Die);
        this->stopUpdate();
        this->stopEffectAnimation();
        this->closeDlg();
        this->removeMofalun();
        if(!m_bQiangBao)
        {
            if(m_nType != Enemy_Type_LingHunShouGeZhe||isFuHuo||m_pParentLayer->isMaxMuBei())
            {
                m_pParentLayer->removeEnemyFromArray(this);
            }
            if(m_nType == Enemy_Type_XiaoGui)
                removeJiaCheng();
            m_pParentLayer->changeGoldNum(m_nGold);
        }
        else
        {
            m_pQiangBaoLayer->removeEnemyFromArray(this);
            
            CCSprite *spr = (CCSprite*)getChildByTag(Effect_Flag);
            spr->removeFromParentAndCleanup(true);
            m_pQiangBaoLayer->addAward(m_bLipin,this->getPosition());
        }
        this->stopAllActions();
        this->setZOrder(Fight_Zorder_Up);
        m_bJiaCheng = false;
        //爆金币或者道具
        {
            int num = m_fAward*100;
            CCLOG("暴奖率:%d",num);
            if(arc4random()%100<=num)
            {
                CCLOG("暴奖");
                CCPoint pos = this->getPosition();
                if(!m_bQiangBao)
                {
                    m_pParentLayer->awardGold(pos);
                }
            }
        }
        
        if(m_nType == Enemy_Type_LingHunShouGeZhe&&!isFuHuo&&!(m_pParentLayer->isMaxMuBei()))
        {
            this->setState(Enemy_State_MuBei);
            m_pParentLayer->retainMuBeiCount();
            m_pSprite->getAnimation()->play("siwang");
            CCSequence *fuhuoAction = CCSequence::create(CCDelayTime::create(15),
                                                         CCCallFuncN::create(this, callfuncN_selector(Enemy::fuhuoCallBack)),
                                                         NULL);
            this->setZOrder(Fight_Zorder_Enemy);
            this->runAction(fuhuoAction);
            return;
        }
        else if(!m_bBoom&&m_nType == Enemy_Type_ShaMoJiaChong&&!(m_pParentLayer->isMaxMuBei()))
        {
            m_pParentLayer->addMuBei(this->getPosition());
            goto die;
        }
        
        if(isBoss())  //移除血条
        {
            CCSprite *progressHp = (CCSprite*)getChildByTag(Effect_Hp_Progress);
            if(progressHp)
            {
                progressHp->removeFromParentAndCleanup(true);
            }
            CCProgressTimer *barHp = (CCProgressTimer*)getChildByTag(Effect_Hp_Bar);
            if(barHp)
            {
                barHp->removeFromParentAndCleanup(true);
            }
        }
        
        if(!m_bPowerAtk)
        {
        die:
            m_pSprite->getAnimation()->stop();
            m_pSprite->setVisible(false);
            if(isBoss())
            {
                char buf[30] = {0};
                sprintf(buf, "Sound/%d_die.ogg",m_nType);
                DATAPOOL->playGameEffect(buf);
            }
            else
            {
                DATAPOOL->playGameEffect(Die2);
            }
            
            DATAPOOL->addEffectCache(XiaoShiRes);
            CCArmature *arm = CCArmature::create("xiaoshi");
            arm->getAnimation()->play("xiaoshi");
            arm->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(Enemy::animationEvent));
            addChild(arm,2);
        }
        else
        {
            if(isBoss())
            {
                char buf[30] = {0};
                sprintf(buf, "Sound/%d_die.ogg",m_nType);
                DATAPOOL->playGameEffect(buf);
            }
            else
            {
                DATAPOOL->playGameEffect(Die1);
            }
            m_pSprite->getAnimation()->stop();
            float angle = arc4random()%360;
            m_fAccelerateY =  HelpMethod::getAngleAccelerateY(angle, Enemy_Accelerate);
            m_fAccelerateX =  HelpMethod::getAngleAccelerateX(angle, Enemy_Accelerate);
            schedule(schedule_selector(Enemy::disapperUpdate), 1/60);
            CCFadeOut *fadeOut = CCFadeOut::create(1);
            CCRotateBy *rotateBy = CCRotateBy::create(0.3, 360);
            CCSequence *seq = CCSequence::create(fadeOut,
                                                 CCCallFunc::create(this, callfunc_selector(Enemy::disapperCallBack)),
                                                 NULL);
            CCRepeatForever *repeat = CCRepeatForever::create(rotateBy);
            m_pSprite->setAnchorPoint(ccp(0.5, 0.5));
            m_pSprite->runAction(seq);
            m_pSprite->runAction(repeat);
        }

    }
    else
    {
        m_bBoom = false;
        if(!isHurt)return;
        if (isCanAtk())
        {
            EnemyStayState *_instance = EnemyStayState::getInstance();
            GetFSMSystem()->ChangeState(_instance);
        }
        else
        {
            State<Enemy> *state = getPreviousState();
            if(state==NULL)
            {
                EnemyStayState *_instance = EnemyStayState::getInstance();
                GetFSMSystem()->ChangeState(_instance);
            }
            else
            {
                GetFSMSystem()->ChangeState(state);
            }
        }
    }
    
}

void Enemy::enemySlowDown()
{
    this->unschedule(schedule_selector(Enemy::moveUpdate));
}

void Enemy::enemyStop()
{
    
}

bool Enemy::checkCollision(Hero *pHero)
{
    if(
       ((m_nType == Enemy_Type_BingBaoHeTun||m_nType==Enemy_Type_RuanNiGuai)&&m_nState==Enemy_State_DrillStay)
       ||m_nState == Enemy_State_Jump||m_nState==Enemy_State_Invisibility||m_nState == Enemy_State_Die
       )return false;
    if(pHero->getType()==Hero_Type_BoLuo&&pHero->isPowerState())return false;
    
    if(lastHitMe == pHero->getHeroReference())  //对于攻击后不消失的英雄 此作用使其只能攻击一次怪物。
    {
        CCLOG("lastHitMe!");
        return false;
    }
    if(!(pHero->getAtkEnemy()))
    {
        return false;
    }
    CCPoint _point = convertToNodeSpace(pHero->getPosition());
    float width = pHero->getSprite()->getContentSize().width;
    float height = pHero->getSprite()->getContentSize().height;
    if (m_pSprite->boundingBox().intersectsRect(CCRectMake(_point.x-width/2 , _point.y-height/2,width, height)))  //大小调试
    {
        if(pHero->isPowerState())
        {
            m_bPowerAtk = true;
        }
        else
        {
            m_bPowerAtk = false;
        }
        lastHitMe = 0;
        lastHitMe = pHero->getHeroReference();
        return true;
    }
    return  false;
}

bool Enemy::checkCollision(cocos2d::CCNode *object)
{
    if((m_nType == Enemy_Type_BingBaoHeTun&&m_nState==Enemy_State_DrillStay)||
       m_nState == Enemy_State_Invisibility||m_nState==Enemy_State_Die)
        return false;
    CCPoint _point = convertToNodeSpace(object->getPosition());
    float width = object->getContentSize().width;
    float height = object->getContentSize().height;
    if (m_pSprite->boundingBox().intersectsRect(CCRectMake(_point.x-width/2, _point.y-height/2, width, height)))
    {
        return true;
    }
    return  false;
}

bool Enemy::checkCollisionLayer(cocos2d::CCLayer *layer)
{
    if((m_nType == Enemy_Type_BingBaoHeTun&&m_nState==Enemy_State_DrillStay)||
       m_nState == Enemy_State_Invisibility||m_nState==Enemy_State_Die)
        return false;
    CCPoint _point = convertToNodeSpace(layer->getPosition());
    float width = layer->getContentSize().width;
    float height = layer->getContentSize().height;
    if (m_pSprite->boundingBox().intersectsRect(CCRectMake(_point.x, _point.y, width, height)))
    {
        return true;
    }
    return  false;
}

bool Enemy::checkCollisionArm(cocos2d::CCNode *object,int dt)
{
    if(
       ((m_nType == Enemy_Type_BingBaoHeTun||m_nType==Enemy_Type_RuanNiGuai)&&m_nState==Enemy_State_DrillStay)||
       m_nState == Enemy_State_Invisibility||m_nState==Enemy_State_Die)
        return false;
    CCPoint _point = convertToNodeSpace(object->getPosition());
    float width = object->getContentSize().width/dt;
    float height = object->getContentSize().height/dt;
    if (m_pSprite->boundingBox().intersectsRect(CCRectMake(_point.x-width/2, _point.y-height/2, width, height)))
    {
        return true;
    }
    return  false;
}

bool Enemy::checkCollision(cocos2d::CCPoint pos)
{
    CCPoint _point = convertToNodeSpace(pos);
    if (m_pSprite->boundingBox().containsPoint(_point))
    {
        return true;
    }
    return  false;
}

bool Enemy::isInvincible()
{
   if(m_nState == Enemy_State_DrillStay||m_nState == Enemy_State_Shield||m_bHuDun)
            return true;
    return false;
}

bool Enemy::isHitInInvincible()
{
    if(m_nType==Enemy_Type_XueYuGangXie||m_nType==Enemy_Type_HuangMoGeBuLin||m_nType == Enemy_Type_hBoss)
    {
        return true;
    }
    return false;
}

void Enemy::setCurrentState(State<Enemy> *pState)
{
    m_pCurrentState = pState;
}

void Enemy::animationEvent(cocos2d::extension::CCArmature *armature, cocos2d::extension::MovementEventType movementType, const char *movementID)
{
    string id = movementID;
    if(movementType == cocos2d::extension::LOOP_COMPLETE)
    {
        if (id.compare("gongji")==0)
        {
            m_bUseAtkSkill = false;
            m_bUseCDSkill = false;
//            enemyHurt(false);
            if(m_nType==Enemy_Type_FengXiYi)
            {
            EnemyJumpState *_instance = EnemyJumpState::getInstance();
            GetFSMSystem()->ChangeState(_instance);
            }
            else if(m_nType==Enemy_Type_BingBaoHeTun||m_nType == Enemy_Type_YouEMo)  //河豚
            {
                this->stopUpdate();
                this->stopSkillUpdate();
                this->stopAllActions();
                m_pParentLayer->removeEnemyFromArray(this);
                this->removeFromParentAndCleanup(true);
            }
            else
            {
                EnemyStayState *_instance = EnemyStayState::getInstance();
                GetFSMSystem()->ChangeState(_instance);
            }
        }
        else if(id.compare("gongji_1")==0)
        {
            m_bUseAtkSkill = false;
            m_bUseCDSkill = false;
//            enemyHurt(false);
            EnemyStayState *_instance = EnemyStayState::getInstance();
            GetFSMSystem()->ChangeState(_instance);
        }
        else if(id.compare("gongji_2")==0)
        {
            m_bUseAtkSkill = false;
            m_bUseCDSkill = false;
//            enemyHurt(false);
            EnemyStayState *_instance = EnemyStayState::getInstance();
            GetFSMSystem()->ChangeState(_instance);
        }
        else if(id.compare("daiji")==0)
        {
//            enemyHurt(false);
            if(m_bQiangBao)
                return;
            
            if(!isCanAtk()&&m_nState!=Enemy_State_Stop)
            {
                EnemyMoveState *_instance = EnemyMoveState::getInstance();
                GetFSMSystem()->ChangeState(_instance);
            }
            else if(isCanAtk()&&m_nState!=Enemy_State_Stop)
            {
                if(m_bRange)
                {
                    m_bMoveFree = true;
                    if(!m_bRecordPos)
                    {
                        m_fOriginalPosX = this->getPositionX();
                        m_fOriginalPosY = this->getPositionY();
                        m_nCurrentPosIndex = 1;
                        m_bMoveUDArrival = true;
                        m_bMoveLRArrival = true;
                        m_bRecordPos = true;
                    }
                    EnemyMoveState *_instance = EnemyMoveState::getInstance();
                    GetFSMSystem()->ChangeState(_instance);
                }
            }
        }
        else if(id.compare("daiji_1")==0)
        {
//            enemyHurt(false);
            if(!isCanAtk()&&m_nState!=Enemy_State_Stop)
            {
                EnemyMoveState *_instance = EnemyMoveState::getInstance();
                GetFSMSystem()->ChangeState(_instance);
            }
            else if(isCanAtk())
            {
                if(m_bRange)
                {
                    m_bMoveFree = true;
                    if(!m_bRecordPos)
                    {
                        m_fOriginalPosX = this->getPositionX();
                        m_fOriginalPosY = this->getPositionY();
                        m_nCurrentPosIndex = 1;
                        m_bMoveUDArrival = true;
                        m_bMoveLRArrival = true;
                        m_bRecordPos = true;
                    }
                    EnemyMoveState *_instance = EnemyMoveState::getInstance();
                    GetFSMSystem()->ChangeState(_instance);
                }
            }

        }
        else if(id.compare("daiji_2")==0)
        {
//            enemyHurt(false);
            if(!isCanAtk()&&m_nState!=Enemy_State_Stop)
            {
                EnemyMoveState *_instance = EnemyMoveState::getInstance();
                GetFSMSystem()->ChangeState(_instance);
            }
            else if(isCanAtk())
            {
                if(m_bRange)
                {
                    m_bMoveFree = true;
                    if(!m_bRecordPos)
                    {
                        m_fOriginalPosX = this->getPositionX();
                        m_fOriginalPosY = this->getPositionY();
                        m_nCurrentPosIndex = 1;
                        m_bMoveUDArrival = true;
                        m_bMoveLRArrival = true;
                        m_bRecordPos = true;
                    }
                    EnemyMoveState *_instance = EnemyMoveState::getInstance();
                    GetFSMSystem()->ChangeState(_instance);
                }
            }

        }
        else if(id.compare("shoushang")==0)
        {
            enemyHurt(true);
        }
        else if(id.compare("shoushang_1")==0)
        {
            enemyHurt(true);
        }
        else if(id.compare("shoushang_2")==0)
        {
            enemyHurt(true);
        }
        else if (id.compare("yidong")==0)
        {
//            enemyHurt(false);
            if(m_bQiangBao)
                return;
            if (isCanAtk())
            {
                if(m_nType == Enemy_Type_ShuiJingZhangYu)
                {
                    EnemyInkJetState *_instance = EnemyInkJetState::getInstance();
                    GetFSMSystem()->ChangeState(_instance);
                }
                else
                {
                    EnemyStayState *_instance = EnemyStayState::getInstance();
                    GetFSMSystem()->ChangeState(_instance);
                }
            }
        }
        else if (id.compare("yidong_1")==0)
        {
//            enemyHurt(false);
            if (isCanAtk())
            {
                EnemyStayState *_instance = EnemyStayState::getInstance();
                GetFSMSystem()->ChangeState(_instance);
            }
        }
        else if (id.compare("yidong_2")==0)
        {
//            enemyHurt(false);
            if (isCanAtk())
            {
                EnemyStayState *_instance = EnemyStayState::getInstance();
                GetFSMSystem()->ChangeState(_instance);
            }
        }
        else if(id.compare("zuanjin")==0)
        {
//            enemyHurt(false);
            if(m_nType==Enemy_Type_XueYuGangXie||m_nType == Enemy_Type_HuangMoGeBuLin)
                m_bRebound = true;
            EnemyDrillStayState *_instance = EnemyDrillStayState::getInstance();
            GetFSMSystem()->ChangeState(_instance);
        }
        else if(id.compare("zuanchu")==0)
        {
            m_bUseAtkSkill = false;
            m_bUseCDSkill = false;
            if(m_nType==Enemy_Type_XueYuGangXie||m_nType == Enemy_Type_HuangMoGeBuLin)
                m_bRebound = false;
            
            if(m_nType == Enemy_Type_hBoss)
                m_cdTime.mCurTime = 0;
            
            if (isCanAtk())
            {
                EnemyStayState *_instance = EnemyStayState::getInstance();
                GetFSMSystem()->ChangeState(_instance);
            }
            else
            {
                EnemyMoveState *_instance = EnemyMoveState::getInstance();
                GetFSMSystem()->ChangeState(_instance);
            }
        }
        else if(id.compare("penmo")==0)
        {
            m_bUseAtkSkill = false;
            m_bUseCDSkill = false;
            EnemyStayState *_instance = EnemyStayState::getInstance();
            GetFSMSystem()->ChangeState(_instance);
        }
        else if(id.compare("chongci")==0)
        {
            m_bUseAtkSkill = false;
            m_bUseCDSkill = false;
            if (isCanAtk())
            {
                EnemyStayState *_instance = EnemyStayState::getInstance();
                GetFSMSystem()->ChangeState(_instance);
            }
        }
        else if(id.compare("yao")==0)
        {
            armature->getAnimation()->play("chi");
        }
        else if(id.compare("chi")==0)
        {
            EnemyStayState *_instance = EnemyStayState::getInstance();
            GetFSMSystem()->ChangeState(_instance);
        }
        else if(id.compare("fuhuo")==0)
        {
            this->setState(Enemy_State_Stay);
            m_pParentLayer->minusMuBeiCount();
            this->setZOrder(Fight_Zorder_Enemy);
            EnemyMoveState *_instance = EnemyMoveState::getInstance();
            GetFSMSystem()->ChangeState(_instance);
        }
        else if(id.compare("tiaochu")==0)
        {
            armature->getAnimation()->play("fuzhi");
        }
        else if(id.compare("fuzhi")==0)
        {
            EnemyMoveState *_instance = EnemyMoveState::getInstance();
            GetFSMSystem()->ChangeState(_instance);
        }
    }
    else if (movementType == extension::COMPLETE)
    {
        if(id.compare("tiaoyue")==0)
        {
            this->setZOrder(Fight_Zorder_Enemy);
            CCLOG("distance:%d",m_nDistance);
            if (isCanAtk())
            {
                EnemyAtkState *_instance = EnemyAtkState::getInstance();
                GetFSMSystem()->ChangeState(_instance);
            }
            else
            {
                schedule(schedule_selector(Enemy::moveUpdate), m_fVel/1000.0);
                EnemyStayState *_instance = EnemyStayState::getInstance();
                GetFSMSystem()->ChangeState(_instance);
            }
        }
        else if(id.compare("xiaoshi")==0)
        {
            this->removeFromParentAndCleanup(true);
            if(!m_bQiangBao)
                m_pParentLayer->enemyDie(isBoss());
            else
            {
            }
        }
        else if(id.compare("shanxian")==0)
        {
            m_bUseAtkSkill = false;
            m_bUseCDSkill = false;
            armature->removeFromParentAndCleanup(true);
            DATAPOOL->removeEffectCache(ShanXianRes);
        }
        else if(id.compare("luoshi")==0)
        {
            armature->removeFromParentAndCleanup(true);
            DATAPOOL->removeEffectCache(LuoShiRes);
            m_pParentLayer->minusBarrierHp(this->getSkillAtk());
            m_pParentLayer->playBarrierUnderAtkAni();
        }
    }
}

void Enemy::frameEvent(Bone *bone, const char *evt, int origiFrameIndex, int currentFrameIndex)
{
    DATAPOOL->playGameEffect(Sound_Atk);
    if(strcmp(evt, "gongjiwancheng")==0)
    {
        if(!m_bRange)  // 近程攻击
        {
            enemyAtk();
        }
        else    //远程
        {
            switch (m_nType)
            {
                case Enemy_Type_HuangMoGeBuLin:
                {
                    createBullet(BoneRes);
                }
                    break;
                case Enemy_Type_FengXiYi:
                {
                    createBullet(BoneRes);
                }
                    break;
                case Enemy_Type_ShuiJingZhangYu:
                {
                    createBullet(ZiDanRes);
                }
                    break;
                case Enemy_Type_DiYuMoMeng:
                {
                    createBullet(BoneRes);
                }
                    break;
                case Enemy_Type_TangXinMo:
                {
                    createFlash();
                }
                    break;
                case Enemy_Type_aBoss:
                {
                    if(bossFirstSkill)
                    {
                        m_bUseSkill = true;
                        this->createTransfer();
                    }
                    else
                    {
                        for(int i=0;i<2;i++)
                        {
                            CCPoint pos;
                            createPos(pos);
                            CCLOG("pos%d x:%f y:%f",i+1,pos.x,pos.y);
                            m_pParentLayer->createOneEnemy(Enemy_Type_GeBuLin, pos);
                        }
                    }
                }
                    break;
                case Enemy_Type_bBoss:
                {
                    if(bossFirstSkill)
                    {
                        createShip();
                    }
                    else
                    {
                        for(int i=0;i<2;i++)
                        {
                            CCPoint pos;
                            createPos(pos);
                            CCLOG("pos%d x:%f y:%f",i+1,pos.x,pos.y);
                            m_pParentLayer->createOneEnemy(Enemy_Type_GeBuLin, pos);
                        }
                    }
                }
                    break;
                case Enemy_Type_cBoss:
                {
                    createTornado();
                }
                    break;
                case Enemy_Type_dBoss:
                {
                    if(bossFirstSkill)
                    {
                        createIceWall();
                    }
                    else
                    {
                        createIceBerg();
                    }
                }
                    break;
                case Enemy_Type_eBoss:
                {
                    if(bossFirstSkill)
                    {
                        createStoneWalling();
                    }
                    else
                    {
                        createBulletAni(HuDunBRes, "hudun_b",false);
                    }
                }
                    break;
                case Enemy_Type_fBoss:
                {
                    if(bossFirstSkill)
                    {
                        createSelf();
                    }
                    else
                    {
                        m_pParentLayer->retainHuoQiuCount();
                        createBulletAni(HuoQiuRes,"huoqiu",true);
                    }
                }
                    break;
                case Enemy_Type_gBoss:
                {
                    if(bossFirstSkill)
                    {
                        createLockHero();
                    }
                    else
                    {
                        createFallAni(LuoShiRes,"luoshi");
                    }
                }
                    break;
                case Enemy_Type_hBoss:
                {
                    if(!bossFirstSkill)
                    {
                        createBullet(JianQiRes);
                    }
                }
                    break;
                case Enemy_Type_iBoss:
                {
                    if(bossFirstSkill)
                    {
                        CCLOG("createFog");
                        createFog();
                    }
                    else
                    {
                        CCLOG("createFlash");
                        createFlash();
                    }
                }
                    break;
                case Enemy_Type_jBoss:
                {
                    if(bossFirstSkill)
                    {
                        createFallAni(LuoShiRes,"luoshi");
                    }
                    else
                    {
                        m_pParentLayer->retainHuoQiuCount();
                        createBulletAni(HuoQiuRes, "huoqiu",true);
                    }
                }
                    break;
                default:
                    break;
            }
        }
    }
    else if(strcmp(evt, "penmowancheng")==0)
    {
        CCSprite *ink = CCSprite::createWithSpriteFrameName(MoShuiRes);
        int dx = arc4random()%301 - 150;
        int dy = arc4random()%301 - 150;
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        ink->setPosition(ccp(size.width/2+dx, size.height/2+dy));
        m_pParentLayer->addInk(ink);
        m_pParentLayer->addChild(ink,Fight_Zorder_Top);
    }
    else if(strcmp(evt, "zuanchuwancheng")==0)
    {
        setState(Enemy_State_DrillOut);
    }
}

void Enemy::createPos(cocos2d::CCPoint &pos)
{
    int dy = arc4random()%50;
    pos.y = this->getPositionY() - dy;
    int dx = arc4random()%100;
    dx -= 50;
    pos.x = this->getPositionX() + dx;
}

void Enemy::minusBlood(int atk,bool powerState,int atkAdd)
{
    if(atk<=0)return;
    if(m_nState == Enemy_State_MuBei)
        return;
    if(!m_pQiangBaoLayer&&m_pParentLayer->isEnergy()&&powerState)
        atk *= 1.2;
    float delay = 0;
    if(powerState)
    {
        int crit = arc4random()%100;
        if(crit>=0&&crit<2)
        {
            delay = 1;
            atk *= 3;
            playBaojiAction();
        }
        else if(crit>=2&&crit<5)
        {
            delay = 1;
            atk *= 2;
            playBaojiAction();
        }
    }
    else
    {
        int delta = arc4random()%4;
        delta -= 2;
        atk += delta;   //+-2的浮动
    }
    atk*= ((float)(100+atkAdd))/100.0f;
    if(atk<1)atk = 1;
    m_nCurrentHp -= atk;
    if(isBoss())
    {
        m_pParentLayer->addBossHurtHp(atk);
    }

    if(!m_pQiangBaoLayer)
    {
        char buf[5] = {0};
        sprintf(buf,"-%d",atk);
        CCLabelBMFont *_numAtk = NULL;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        _numAtk = CCLabelBMFont::create(buf, "num_blood.fnt");
#else
        _numAtk = CCLabelBMFont::create(buf, "fonts/num_blood.fnt");
#endif
        _numAtk->setPosition(ccp(m_pSprite->getPositionX(), m_pSprite->getPositionY()+m_pSprite->getContentSize().height-20));
        addChild(_numAtk);
        CCSequence *seq = CCSequence::create(CCDelayTime::create(delay),
                                             CCSpawn::create(CCMoveBy::create(0.2, ccp(0, 20)),
                                                             CCScaleTo::create(0.2, 1.5),
                                                             NULL),
                                             CCFadeOut::create(0.1),
                                             CCCallFuncN::create(this, callfuncN_selector(Enemy::removeHpNumCallBack)),
                                             NULL);
        _numAtk->runAction(seq);
    }
    
    CCSprite *dlg = NULL;
    dlg = (CCSprite*)getChildByTag(Effect_Dlg);
    if(!dlg&&!m_pQiangBaoLayer)
    {
        float currentHp = (float)m_nCurrentHp;
        float hp = (float)m_nHp;
        if(currentHp<(hp*0.1)&&currentHp>0)   //血量少于10%
        {
            dlg = CCSprite::createWithSpriteFrameName("Dlg_Passive_2_1.png");
            dlg->setPosition(ccp(m_pSprite->getPositionX(), m_pSprite->getPositionY()+m_pSprite->getContentSize().height));
            dlg->setOpacity(0);
            CCSequence *seq = CCSequence::create(CCFadeIn::create(0.1),
                                                 CCDelayTime::create(3),
                                                 CCCallFuncN::create(this, callfuncN_selector(Enemy::removeMissBaojiCallBack)),
                                                 NULL);
            dlg->runAction(seq);
            addChild(dlg,10,Effect_Dlg);
        }
    }
    m_bHurtCount++;
    int tmp = arc4random()%100;
    if(!m_pQiangBaoLayer&&!dlg&&m_bHurtCount>=3&&tmp>80)
    {
        m_bHurtCount = 0;
        int index = arc4random()%3;
        index++;
        char buf[30] = {0};
        sprintf(buf, "Dlg_Passive_1_%d.png",index);
        dlg = CCSprite::createWithSpriteFrameName(buf);
        dlg->setPosition(ccp(m_pSprite->getPositionX(), m_pSprite->getPositionY()+m_pSprite->getContentSize().height));
        dlg->setOpacity(0);
        CCSequence *seq = CCSequence::create(CCFadeIn::create(0.1),
                                             CCDelayTime::create(3),
                                             CCCallFuncN::create(this, callfuncN_selector(Enemy::removeMissBaojiCallBack)),
                                             NULL);
        dlg->runAction(seq);
        addChild(dlg,10,Effect_Dlg);
    }

    if(isBoss())
    {
        updateHpBar();
    }
    enemyHurt(false);
}

void Enemy::speakInitiative()
{
    if(m_pQiangBaoLayer)
        return;
    CCSprite *dlg = NULL;
    dlg = (CCSprite*)getChildByTag(Effect_Dlg);
    if(!dlg)
    {
        char buf[30] = {0};
        int index = arc4random()%4;
        index++;
        sprintf(buf, "Dlg_Initiative_%d.png",index);
        dlg = CCSprite::createWithSpriteFrameName(buf);
        dlg->setPosition(ccp(m_pSprite->getPositionX(), m_pSprite->getPositionY()+m_pSprite->getContentSize().height));
        dlg->setOpacity(0);
        CCSequence *seq = CCSequence::create(CCFadeIn::create(0.1),
                                             CCDelayTime::create(3),
                                             CCCallFuncN::create(this, callfuncN_selector(Enemy::removeMissBaojiCallBack)),
                                             NULL);
        dlg->runAction(seq);
        addChild(dlg,10,Effect_Dlg);
    }
}

void Enemy::speakSkill()
{
    if(m_pQiangBaoLayer)
        return;
    char buf[30] = {0};
    int index = 0;
    int delayTime = 0;
    switch (m_nType)
    {
        case Enemy_Type_JuXingGeBuLin:
        {
            delayTime = 1;
            index = 1;
        }
            break;
        case Enemy_Type_XueYuGangXie:
        case Enemy_Type_HuangMoGeBuLin:
        {
            index = 2;
        }
            break;
        case Enemy_Type_HeiTangEMo:
        {
            index = 1;
        }
            break;
        default:
        {
            index = 1;
        }
            break;
    }
    CCSprite *dlg = NULL;
    dlg = (CCSprite*)getChildByTag(Effect_Dlg);
    if(!dlg)
    {
        sprintf(buf, "Dlg_Passive_3_%d.png",index);
        dlg = CCSprite::createWithSpriteFrameName(buf);
        dlg->setPosition(ccp(m_pSprite->getPositionX(), m_pSprite->getPositionY()+m_pSprite->getContentSize().height));
        dlg->setOpacity(0);
        CCSequence *seq = CCSequence::create(CCDelayTime::create(delayTime),
                                             CCFadeIn::create(0.1),
                                             CCDelayTime::create(3),
                                             CCCallFuncN::create(this, callfuncN_selector(Enemy::removeMissBaojiCallBack)),
                                             NULL);
        dlg->runAction(seq);
        addChild(dlg,10,Effect_Dlg);
    }
}

void Enemy::playBaojiAction()
{
    CCSprite *_miss = CCSprite::createWithSpriteFrameName(Text_BaoJi);
    _miss->setPosition(ccp(m_pSprite->getPositionX(), m_pSprite->getPositionY()+m_pSprite->getContentSize().height/2));
    addChild(_miss,1);
    CCSequence *seq = CCSequence::create(CCSpawn::create(CCMoveBy::create(0.2, ccp(0, 20)),
                                                         CCScaleTo::create(0.2, 2.5),
                                                         NULL),
                                         CCDelayTime::create(0.3),
                                         CCFadeOut::create(0.1),
                                         CCCallFuncN::create(this, callfuncN_selector(Enemy::removeMissBaojiCallBack)),
                                         NULL);
    _miss->runAction(seq);
}

void Enemy::removeHpNumCallBack(cocos2d::CCNode *pSender)
{
    CCLabelBMFont *_numAtk = (CCLabelBMFont*)pSender;
    if(_numAtk)
    {
        _numAtk->removeFromParentAndCleanup(true);
    }
}

void Enemy::removeMissBaojiCallBack(cocos2d::CCNode *pSender)
{
    CCSprite *spr = (CCSprite*)pSender;
    if(spr)
    {
        spr->removeFromParentAndCleanup(true);
    }
}

void Enemy::updateHpBar()
{
    CCProgressTimer *barHp = (CCProgressTimer*)getChildByTag(Effect_Hp_Bar);
    if(barHp)
    {
        float percent = (float)m_nCurrentHp/(float)m_nHp;
        if(percent*100>0&&percent*100<=5)
            percent = 0.05f;
        barHp->setPercentage(percent*100);
    }
}

void Enemy::stopUpdate()
{
    m_gameTime.mCurTime = 0.0;
    unschedule(schedule_selector(Enemy::moveUpdate));
}

void Enemy::disapperUpdate(float dt)
{
    float _posX = this->getPositionX();
    float _posY = this->getPositionY();
    
    _posX += m_fAccelerateX;
    _posY += m_fAccelerateY;
    CCSize _size = CCDirector::sharedDirector()->getWinSize();
    if (_posX < this->getContentSize().width/2)
    {
        _posX = this->getContentSize().width/2;
        m_fAccelerateX*=-1;
    }
    else if (_posX > (_size.width - this->getContentSize().width/2))
    {
        _posX = _size.width - this->getContentSize().width/2;
        m_fAccelerateX*=-1;
    }
    else if(_posY<Barrier_Y+Atk_Barrier_DY+this->getContentSize().height/2)
    {
        _posY = Barrier_Y+Atk_Barrier_DY+this->getContentSize().height/2;
        m_fAccelerateY*=-1;
    }
    else if(_posY > _size.height - this->getContentSize().height/2)
    {
        _posY = _size.height - this->getContentSize().height/2;
        m_fAccelerateY*=-1;
    }
    this->setPosition(ccp(_posX, _posY));
}

void Enemy::disapperCallBack()
{
    this->removeFromParentAndCleanup(true);
    if(!m_bQiangBao)
       m_pParentLayer->enemyDie(isBoss());
}

void Enemy::replicateCallBack(cocos2d::CCObject *pSender)
{
    m_bUseCDSkill = false;
    Enemy *enemy = (Enemy*)pSender;
    EnemyStayState *_instance = EnemyStayState::getInstance();
    enemy->GetFSMSystem()->ChangeState(_instance);
    enemy->stopAllActions();
    Enemy_Data* data = getData();
    Enemy *enemy1 = new Enemy(data,m_pParentLayer,true);
    enemy1->autorelease();
    enemy1->setPosition(enemy->getPosition());
    enemy1->setUseSkill(true);
    m_pParentLayer->addEnemy(enemy1,true);
    m_pParentLayer->addChild(enemy1, Fight_Zorder_Enemy);
    int dx = 100;
    float x = enemy->getPositionX();
    if(x-dx>m_pSprite->getContentSize().width/2)
        x = x - dx;
    CCMoveTo *moveLeft = CCMoveTo::create(0.5, ccp(x, enemy->getPositionY()));
    
    x = enemy1->getPositionX();
    if(x + dx <= m_pSprite->getContentSize().width/2)
    x = x + dx;
    CCMoveTo *moveRight = CCMoveTo::create(0.5, ccp(x, enemy1->getPositionY()));
    
    enemy->runAction(
                     CCSequence::create(
                                        moveLeft,
                                        CCCallFuncN::create(enemy, callfuncN_selector(Enemy::replicatedCallBack)),
                                        NULL));
    
    enemy1->runAction(
                      CCSequence::create(
                                         moveRight,
                                         CCCallFuncN::create(enemy1, callfuncN_selector(Enemy::replicatedCallBack)),
                                         NULL));
    data = NULL;
}

void Enemy::replicatedCallBack(cocos2d::CCObject *pSender)
{
    Enemy* enemy = (Enemy*)pSender;
    EnemyMoveState *_instance = EnemyMoveState::getInstance();
    enemy->GetFSMSystem()->ChangeState(_instance);
}

Enemy_Data* Enemy::getData()     ////
{
    return  m_pData;
}

void Enemy::playShieldAnimation()
{
    DATAPOOL->addEffectCache(HuDunARes);
    CCArmature *arm = CCArmature::create("hudun_a");
    arm->getAnimation()->play("hudun_a");
    arm->setPosition(m_pSprite->getPosition());
    addChild(arm,1,Effect_HuDun);
}

void Enemy::stopEffectAnimation()
{
    CCSprite *bing = (CCSprite*)getChildByTag(Effect_BingKuai);
    if (bing)
    {
        bing->removeFromParentAndCleanup(true);
    }
    
    CCArmature *tengman = (CCArmature*)getChildByTag(Effect_TengMan);
    {
        if (tengman)
        {
            tengman->removeFromParentAndCleanup(true);
            DATAPOOL->removeEffectCache(TengManRes);
        }
    }
    
    CCSprite *sleep = (CCSprite*)getChildByTag(Effect_Sleep);
    if(sleep)
    {
        sleep->removeFromParentAndCleanup(true);
    }
    
    CCArmature *hudun = (CCArmature*)getChildByTag(Effect_HuDun);
    {
        if(hudun)
        {
            hudun->removeFromParentAndCleanup(true);
            DATAPOOL->removeEffectCache(HuDunARes);
        }
    }
    
    CCArmature *bingxuli = (CCArmature*)getChildByTag(Effect_BingXuLi);
    {
        if(bingxuli)
        {
            bingxuli->removeFromParentAndCleanup(true);
            DATAPOOL->removeEffectCache(BingBRes);
        }
    }
    
    CCArmature *star = (CCArmature*)getChildByTag(Effect_Star);
    {
        if(star)
        {
            star->removeFromParentAndCleanup(true);
            DATAPOOL->removeEffectCache(XingXingRes);
        }
    }
    
    CCParticleSmoke *smoke = (CCParticleSmoke*)getChildByTag(Effect_Particle);
    {
        if(smoke)
        {
            smoke->removeFromParentAndCleanup(true);
        }
    }
}

void Enemy::closeDlg()
{
    CCSprite *dlg = (CCSprite*)getChildByTag(Effect_Dlg);
    {
        if(dlg)
        {
            dlg->removeFromParentAndCleanup(true);
        }
    }
}

void Enemy::playBondageEffectAnimation()
{
    DATAPOOL->addEffectCache(TengManRes);
    CCArmature *arm = NULL;
    arm = (CCArmature*)getChildByTag(Effect_TengMan);
    if(!arm)
    {
        arm = CCArmature::create("tengman");
        arm->getAnimation()->play("tengman");
        arm->setPosition(m_pSprite->getPosition());
        addChild(arm,1,Effect_TengMan);
    }
}

void Enemy::playFrozenEffectAnimation()
{
    CCSprite *spr = NULL;
    spr = (CCSprite*)getChildByTag(Effect_BingKuai);
    if(!spr)
    {
        if(isBoss())
        {
            spr = CCSprite::createWithSpriteFrameName(BingKuaiResB);
        }
        else
        {
            spr = CCSprite::createWithSpriteFrameName(BingKuaiResA);
        }
        spr->setAnchorPoint(ccp(0.5, 0));
        spr->setPosition(ccp(m_pSprite->getPositionX(),m_pSprite->getPositionY()-10));
        addChild(spr,1,Effect_BingKuai);
    }
    m_pSprite->getAnimation()->stop();
    
}

void Enemy::playSleepEffectAnimation()
{
    CCSprite *spr = NULL;
    spr = (CCSprite*)getChildByTag(Effect_Sleep);
    if(!spr)
    {
        spr = CCSprite::createWithSpriteFrameName(SleepRes);
        spr->setPosition(ccp(m_pSprite->getPositionX(), m_pSprite->getPositionY()+m_pSprite->getContentSize().height));
        spr->setScale(0.4f);
        addChild(spr,1,Effect_Sleep);
        
        CCSequence *seq = CCSequence::create(CCSpawn::create(CCScaleTo::create(0.1, 0.5),CCMoveBy::create(0.1, ccp(9, 9)),NULL),
                                             CCSpawn::create(CCScaleTo::create(0.1, 0.6),CCMoveBy::create(0.1, ccp(8, 17)),CCFadeTo::create(0.1, 210),NULL),
                                             CCSpawn::create(CCScaleTo::create(0.1, 0.7),CCMoveBy::create(0.1, ccp(-4, 13)),CCFadeTo::create(0.1, 180),NULL),
                                             CCSpawn::create(CCScaleTo::create(0.1, 0.8),CCMoveBy::create(0.1, ccp(-14, 12)),CCFadeTo::create(0.1, 150),NULL),
                                             CCSpawn::create(CCScaleTo::create(0.1, 0.9),CCMoveBy::create(0.1, ccp(-6, 15)),CCFadeTo::create(0.1, 100),NULL),
                                             CCSpawn::create(CCScaleTo::create(0.1, 1),CCMoveBy::create(0.1, ccp(3, 12)),CCFadeTo::create(0.1, 20),NULL),
                                             CCSpawn::create(CCScaleTo::create(0.01, 0.4),CCMoveBy::create(0.01, ccp(4, -78)),CCFadeTo::create(0.01, 255),NULL),
                                             NULL);
        spr->runAction(CCRepeatForever::create(seq));
    }
}

void Enemy::createTransfer()
{
    m_pParentLayer->createTransfer(this);
}

void Enemy::createShip()
{
    m_pParentLayer->createShip(this);
}

void Enemy::createTornado()
{
    m_pParentLayer->createTornado(this);
}

void Enemy::createLockHero()
{
    m_pParentLayer->createLockHero(this);
}

void Enemy::createIceBerg()
{
//    DATAPOOL->addEffectCache(BingBRes);
//    CCArmature *arm = NULL;
//    arm = (CCArmature*)getChildByTag(Effect_BingXuLi);
//    if(arm==NULL)
//    {
//        arm = CCArmature::create("bing_b");
//    }
//    else
//    {
//        arm->removeFromParentAndCleanup(true);
//        arm = NULL;
//        arm = CCArmature::create("bing_b");
//    }
//    arm->getAnimation()->play("bing_b");
//    arm->setPosition(m_pSprite->getPosition());
//    addChild(arm,1,Effect_BingXuLi);
//    CCSequence *seq = CCSequence::create(CCDelayTime::create(3),
//                                         CCCallFuncN::create(this, callfuncN_selector(Enemy::actionCallBack)),
//                                         NULL);
//    arm->runAction(seq);
    m_pParentLayer->createIceBerg(this);
}

void Enemy::createStoneWalling()
{
    m_pParentLayer->createStoneWalling(this);
}

void Enemy::createIceWall()
{
    m_pParentLayer->createIceWall(this);
}

void Enemy::createHuDun()
{
    m_bHuDun = true;
    DATAPOOL->addEffectCache(HuDunBRes);
    CCArmature *hudun = CCArmature::create("hudun_b");
    hudun->setPosition(m_pSprite->getPosition());
    addChild(hudun, 2, Effect_BossHuDun);
    hudun->getAnimation()->play("hudun_b");
    
}

void Enemy::cancelHuDun()
{
    m_bHuDun = false;
    CCArmature *hudun = (CCArmature*)getChildByTag(Effect_BossHuDun);
    if(hudun)
    {
        hudun->removeFromParentAndCleanup(true);
        DATAPOOL->removeEffectCache(HuDunBRes);
    }
}

void Enemy::createFallAni(const char *fileName ,const char *aniName)
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    DATAPOOL->addEffectCache(fileName);
    CCArmature* arm = CCArmature::create(aniName);
    arm->setPosition(ccp(size.width/2, Barrier_Y));
    m_pParentLayer->addChild(arm,Fight_Zorder_Bullet);
    arm->getAnimation()->play(aniName);
    arm->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(Enemy::animationEvent));
}

void Enemy::createFlash()
{
    CCSprite *flash = CCSprite::createWithSpriteFrameName(ShanDianRes);
    flash->setScale(0.8);
    flash->setPosition(this->getPosition());
    m_pParentLayer->addChild(flash,Fight_Zorder_Bullet);
    
    CCMoveTo *move = CCMoveTo::create(0.3f, ccp(this->getPositionX(), Barrier_Y));
    CCSequence *seq = CCSequence::create(move,
                                         CCCallFuncND::create(m_pParentLayer, callfuncND_selector(FightLayer::remoteAtkCallBack_Sprite), this),
                                         NULL);
    flash->runAction(seq);
}

void Enemy::createSelf()
{
    m_pParentLayer->createSelf(this);
}

void Enemy::createFog()
{
    m_pParentLayer->createFog(this);
}

void Enemy::stopSkillUpdate()
{
//    this->unschedule(schedule_selector(Enemy::skillUpdate));
}

void Enemy::createBullet(const char *name)
{
    CCSprite* _bone = CCSprite::createWithSpriteFrameName(name);
    _bone->setPosition(this->getPosition());
    m_pParentLayer->addChild(_bone,Fight_Zorder_Bullet);
    CCMoveTo *move = CCMoveTo::create(0.3f, ccp(this->getPositionX(), Barrier_Y));
//    CCRotateBy *rotate = CCRotateBy::create(0.2f, 720);
    CCSpawn *spa = CCSpawn::create(move,
//                                   rotate,
                                   NULL);
    CCSequence *seq = CCSequence::create(spa,
                                         CCCallFuncND::create(m_pParentLayer, callfuncND_selector(FightLayer::remoteAtkCallBack_Sprite), this),
                                         NULL);
    _bone->runAction(seq);
}

void Enemy::createBulletAni(const char *fileName,const char *aniName,bool retain)
{
    DATAPOOL->addEffectCache(fileName);
    CCArmature* arm = CCArmature::create(aniName);
    arm->setPosition(this->getPosition());
    arm->getAnimation()->play(aniName);
    m_pParentLayer->addChild(arm,Fight_Zorder_Bullet);
    CCMoveTo *move = CCMoveTo::create(0.3f, ccp(this->getPositionX(), Barrier_Y));
    CCRotateBy *rotate = CCRotateBy::create(0.2f, 720);
    CCSpawn *spa = CCSpawn::create(move,
                                   rotate,
                                   NULL);
    CCSequence *seq = NULL;
    if(!retain)
    {
        seq  = CCSequence::create(spa,
                                   CCCallFuncND::create(m_pParentLayer, callfuncND_selector(FightLayer::remoteAtkCallBack_Armature), this),
                                   NULL);
    }
    else
    {
        seq = CCSequence::create(spa,
                                 CCCallFuncND::create(m_pParentLayer, callfuncND_selector(FightLayer::remoteAtkCallBack_Armature_HuoQiu), this),
                                 NULL);
    }
    
    arm->runAction(seq);
}

bool Enemy::isCanAtk()
{
    bool res = false;
    if(this->getPositionY()<=Barrier_Y+(m_nDistance*DATAPOOL->zoom))
        res = true;
    return  res;
}

bool Enemy::isBoss()
{
    bool res = false;
    if(m_nType == Enemy_Type_aBoss||m_nType == Enemy_Type_bBoss||m_nType == Enemy_Type_cBoss||m_nType == Enemy_Type_dBoss||m_nType == Enemy_Type_eBoss
       ||m_nType == Enemy_Type_fBoss||m_nType == Enemy_Type_gBoss||m_nType == Enemy_Type_hBoss||m_nType == Enemy_Type_iBoss||m_nType == Enemy_Type_jBoss)
        res = true;
    return res;
}

void Enemy::actionCallBack(cocos2d::CCObject *pSender)
{
    CCArmature *arm = (CCArmature*)pSender;
    if(arm)
    {
        arm->removeFromParentAndCleanup(true);
        DATAPOOL->removeEffectCache(BingBRes);
    }
}

bool Enemy::isCanEat()
{
    if(m_nType!=Enemy_Type_DiYuFengQuan)
    return false;
    
    if(!isEat)
    {
        isEat = true;
        return true;
    }
    else
    {
        return false;
    }
}

void Enemy::jiaChengUpdate(float dt)
{
    for(int i =m_pParentLayer->getEnemyArray()->count()-1 ;i>=0 ;i--)
    {
        Enemy *_enemy = (Enemy*)m_pParentLayer->getEnemyArray()->objectAtIndex(i);
        if(_enemy&&_enemy->getType()!=Enemy_Type_XiaoGui)
        {
            if(m_pParentLayer->areaAttack(this->getPosition(), _enemy, 200))
            {
                _enemy->jiaCheng();
            }
            else
            {
                _enemy->CancelJiaCheng();
            }
        }
    }
    
}

void Enemy::jiaCheng()
{
    if(!isJiaCheng)
    {
        CCLOG("JiaCheng!");
        isJiaCheng = true;
        DATAPOOL->addEffectCache(MoFaLunBRes);
        CCArmature *mofalun_b = CCArmature::create("mofalun_a");
        mofalun_b->getAnimation()->play("mofalun_a");
        mofalun_b->setPosition(ccp(m_pSprite->getPositionX(), m_pSprite->getPositionY()+20));
        addChild(mofalun_b,1,Effect_MoFaLun_B);
        m_nAtk+=10;
    }
}

void Enemy::CancelJiaCheng()
{
    if(isJiaCheng)
    {
        isJiaCheng = false;
        m_nAtk-=10;
        CCArmature *arm = (CCArmature*)getChildByTag(Effect_MoFaLun_B);
        if(arm)
        {
            this->removeChild(arm, true);
        }
    }
}

void Enemy::fuhuoCallBack(cocos2d::CCObject *pSender)
{
    isFuHuo = true;
    m_nCurrentHp = m_nFuHuoHp;
    m_pSprite->getAnimation()->play("fuhuo");
    m_pSprite->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(Enemy::animationEvent));
}

void Enemy::enemyCDUpdate(float dt)
{
    if(!m_pQiangBaoLayer&&m_pParentLayer->isShowTeachLayer())return;
    if(m_nState == Enemy_State_Stop||m_nState == Enemy_State_Die||m_nState == Enemy_State_MuBei)
        return;
    if(m_cdTime.mTotalTime == 0)
       return;
    if(m_bUseAtkSkill)return;
    m_cdTime.mCurTime+=dt;
    if(m_cdTime.mCurTime>=m_cdTime.mTotalTime)
    {
        m_cdTime.mCurTime = 0;
        m_bUseCDSkill = true;
        switch (m_nType)
        {
            case Enemy_Type_RuanNiGuai:
            case Enemy_Type_XueYuGangXie:
            case Enemy_Type_BingBaoHeTun:
            case Enemy_Type_HuangMoGeBuLin:
            {
                m_bUseSkill = true;
                EnemyDrillState *_intance = EnemyDrillState::getInstance();
                GetFSMSystem()->ChangeState(_intance);
            }
                break;
            case Enemy_Type_ShaBaoJuRen:
            {
                m_bUseSkill =  true;
                EnemySprintState *_instance = EnemySprintState::getInstance();
                GetFSMSystem()->ChangeState(_instance);
            }
                break;
            case Enemy_Type_DiYuMoMeng:
            case Enemy_Type_HeiTangEMo:
            {
                EnemyInvisibilityState *_instance = EnemyInvisibilityState::getInstance();
                GetFSMSystem()->ChangeState(_instance);
            }
                break;
            case Enemy_Type_LingHunShouGeZhe:
            {
                m_bUseSkill = true;
                EnemyInvisibilityState *_instance = EnemyInvisibilityState::getInstance();
                GetFSMSystem()->ChangeState(_instance);
            }
                break;
            case Enemy_Type_XiaoGui:
            {
                if(m_bUseSkill)
                {
                    m_bUseCDSkill = false;
                    return;
                }
                else
                {
                    m_bUseSkill = true;
                }
                m_bUseCDSkill = false;
                DATAPOOL->addEffectCache(MoFaLunARes);
                CCArmature *mofalun_a = CCArmature::create("mofalun_a");
                mofalun_a->getAnimation()->play("mofalun_a");
                mofalun_a->setPosition(ccp(m_pSprite->getPositionX(), m_pSprite->getPositionY()+20));
                addChild(mofalun_a,1,Effect_MoFaLun_A);
                DATAPOOL->addEffectCache(MoFaLunBRes);
                CCArmature *mofalun_b = CCArmature::create("mofalun_b");
                mofalun_b->getAnimation()->play("mofalun_b");
                mofalun_b->setPosition(m_pSprite->getPosition());
                addChild(mofalun_b,-1,Effect_MoFaLun_B);
                m_bJiaCheng = true;
            }
                break;
            case Enemy_Type_JiaoTangKuiLei:
            {
                if(m_bUseSkill)
                {
                    m_bUseCDSkill = false;
                    return;
                }
                else
                {
                    m_bUseSkill = true;
                }
                m_bUseCDSkill = false;
                Enemy *_enemy = new Enemy(this->getData(),m_pParentLayer,true);
                _enemy->autorelease();
                _enemy->setPosition(ccp(this->getPositionX(), this->getPositionY()-20));
                _enemy->getSprite()->getAnimation()->play("tiaochu");
                _enemy->getSprite()->getAnimation()->setMovementEventCallFunc(_enemy, movementEvent_selector(Enemy::animationEvent));
                m_pParentLayer->addChild(_enemy,Fight_Zorder_Enemy);
                m_pParentLayer->addEnemy(_enemy,false);
                _enemy->setUseSkill(true);
            }
                break;
            case Enemy_Type_aBoss:
            {
                if(m_bUseSkill)
                {
                    m_bUseCDSkill = false;
                    return;
                }
                bossFirstSkill = true;
                EnemyAtkState *_instance = EnemyAtkState::getInstance();
                this->GetFSMSystem()->ChangeState(_instance);
            }
                break;
            case Enemy_Type_bBoss:
            {
                bossFirstSkill = true;
                EnemyAtkState *_instance = EnemyAtkState::getInstance();
                GetFSMSystem()->ChangeState(_instance);
            }
                break;
            case Enemy_Type_cBoss:
            {
                if(m_bUseSkill)
                {
                    m_bUseCDSkill = false;
                    return;
                }
                else
                {
                    m_bUseSkill = true;
                }
                EnemyReplicateState *_instance = EnemyReplicateState::getInstance();
                GetFSMSystem()->ChangeState(_instance);
            }
                break;
            case Enemy_Type_dBoss:
            {
//                if(m_bUseSkill)
//                {
//                    m_bUseCDSkill = false;
//                    return;
//                }
//                else
//                {
//                    m_bUseSkill = true;
//                }
                bossFirstSkill = true;
                EnemyAtkState *_instance = EnemyAtkState::getInstance();
                GetFSMSystem()->ChangeState(_instance);
            }
                break;
            case Enemy_Type_eBoss:
            {
                bossFirstSkill = true;
                if(m_bHuDun)   //已有护盾
                    return;
                if(m_pParentLayer->isMaxShiBei())
                    return;
                if(m_bUseSkill)
                {
                    m_bUseCDSkill = false;
                    return;
                }
                EnemyAtkState *_instance = EnemyAtkState::getInstance();
                GetFSMSystem()->ChangeState(_instance);
            }
                break;
            case Enemy_Type_fBoss:
            {
                if(m_bUseSkill)
                {
                    m_bUseCDSkill = false;
                    return;
                }
                else
                {
                    m_bUseSkill = true;
                }
                bossFirstSkill = true;
                EnemyAtkState *_instance = EnemyAtkState::getInstance();
                GetFSMSystem()->ChangeState(_instance);
            }
                break;
            case Enemy_Type_gBoss:
            {
                bossFirstSkill = true;
                EnemyAtkState *_instance = EnemyAtkState::getInstance();
                GetFSMSystem()->ChangeState(_instance);
            }
                break;
            case Enemy_Type_hBoss:
            {
                if(m_bUseSkill)
                {
                    m_bUseCDSkill = false;
                    return;
                }
//                else
//                {
//                    m_bUseSkill = true;
//                }
                bossFirstSkill = true;
                EnemyDrillState *_intance = EnemyDrillState::getInstance();
                GetFSMSystem()->ChangeState(_intance);
            }
                break;
            case Enemy_Type_iBoss:
            {
                if(m_bUseSkill)
                {
                    m_bUseCDSkill = false;
                    return;
                }
//                else
//                {
//                    m_bUseSkill = true;
//                }
                bossFirstSkill = true;
                EnemyAtkState *_instance = EnemyAtkState::getInstance();
                GetFSMSystem()->ChangeState(_instance);
            }
                break;
            case Enemy_Type_jBoss:
            {
                bossFirstSkill = true;
                EnemyAtkState *_instance = EnemyAtkState::getInstance();
                GetFSMSystem()->ChangeState(_instance);
            }
                break;
            default:
                m_bUseCDSkill = false;
                break;
        }
        
        char buf[30] = {0};
        sprintf(buf, "Sound/%d_skill1.ogg",m_nType);
        DATAPOOL->playGameEffect(buf);
    }
}

void Enemy::enemyAtkUpdata(float dt)
{
    if(!m_pQiangBaoLayer&&m_pParentLayer->isShowTeachLayer())return;
    if(m_nState == Enemy_State_Stop||m_atkFrequency.mTotalTime==0||m_nState == Enemy_State_Die||m_nState == Enemy_State_MuBei)
        return;
    if (isCanAtk())
    {
        if(m_bUseCDSkill)return;
        m_atkFrequency.mCurTime+=dt;
        if(m_atkFrequency.mCurTime>m_atkFrequency.mTotalTime)
        {
            m_atkFrequency.mCurTime=0.0;
            m_bUseAtkSkill =  true;
            if(m_nType == Enemy_Type_ShuiJingZhangYu&&(arc4random()%100)<30)   //章鱼 30%的概率喷墨
            {
                if(!m_pParentLayer->isMaxInk())
                {
                    EnemyInkJetState *_instance = EnemyInkJetState::getInstance();
                    GetFSMSystem()->ChangeState(_instance);
                }
                else
                {
                    EnemyAtkState *_instance = EnemyAtkState::getInstance();
                    GetFSMSystem()->ChangeState(_instance);
                }
            }
            else
            {
                if(isBoss())
                {
                    bossFirstSkill = false;
                    char buf[30] = {0};
                    sprintf(buf, "Sound/%d_skill2.ogg",m_nType);
                    DATAPOOL->playGameEffect(buf);
                }
                CCLOG("use atk skill!");
                EnemyAtkState *_instance = EnemyAtkState::getInstance();
                GetFSMSystem()->ChangeState(_instance);
            }
        }
    }
}

void Enemy::enemyPoison(int time,int atk)
{
    m_PoisonTime.mCurTime = 0;
    m_PoisonTime.mTotalTime = time;
    m_nPoisonAtk = 0;
    m_nHeroAtk = atk;
    schedule(schedule_selector(Enemy::poisonUpdate), 1.0);
}

void Enemy::enemyBondage(int time, int atk)
{
    m_PoisonTime.mCurTime = 0;
    m_PoisonTime.mTotalTime = time;
    m_nHeroAtk = atk;
    schedule(schedule_selector(Enemy::bondageUpdate), 1.0);
}

void Enemy::poisonUpdate(float dt)
{
    m_PoisonTime.mCurTime+=dt;
    minusBlood(m_nHeroAtk,false);
    enemyHurt(false);
    if(m_PoisonTime.mCurTime>m_PoisonTime.mTotalTime)
    {
        m_PoisonTime.mCurTime= 0;
        unschedule(schedule_selector(Enemy::poisonUpdate));
    }
}

void Enemy::bondageUpdate(float dt)
{
    m_PoisonTime.mCurTime+=dt;
    if(m_PoisonTime.mCurTime>m_PoisonTime.mTotalTime)
    {
        m_PoisonTime.mCurTime= 0;
        unschedule(schedule_selector(Enemy::poisonUpdate));
        minusBlood(m_nHeroAtk,false);
        enemyHurt(false);
    }
}

void Enemy::startDisapperUpdate()
{
    schedule(schedule_selector(Enemy::disapperUpdate), 1/60);
}

void Enemy::createSlowDelayAction(float time)
{
    if(m_SlowStopTime.mTotalTime!=0)
    {
        m_SlowStopTime.mTotalTime = time;
        unschedule(schedule_selector(Enemy::slowStopUpdate));
        schedule(schedule_selector(Enemy::slowStopUpdate), 1/60);
        return;
    }
    DATAPOOL->addEffectCache(XingXingRes);
    CCArmature *arm = CCArmature::create("xingxing");
    arm->getAnimation()->play("xingxing");
    addChild(arm,1,Effect_Star);
    arm->setPosition(ccp(0, getSprite()->getContentSize().height));
    m_SlowStopTime.mCurTime = 0;
    m_SlowStopTime.mTotalTime = time;
    m_bIsSlowAction = true;
    schedule(schedule_selector(Enemy::slowStopUpdate), 1/60);
}

void Enemy::slowDownEndCallback(cocos2d::CCNode *pSender)
{
//    this->enemySlowDown();
//    if(this->getPreviousState())
//    {
//        this->GetFSMSystem()->ChangeState(this->getPreviousState());
//    }
//    else
//    {
//        EnemyMoveState *_instance = EnemyMoveState::getInstance();
//        this->GetFSMSystem()->ChangeState(_instance);
//    }
    CCArmature *arm = (CCArmature*)getChildByTag(Effect_Star);
    if(arm)
    {
        this->removeChild(arm, true);
        DATAPOOL->removeEffectCache(XingXingRes);
    }
}

void Enemy::createStopDelayAction(float time)
{

    if(m_SlowStopTime.mTotalTime!=0)
    {
        m_SlowStopTime.mTotalTime = time;
        unschedule(schedule_selector(Enemy::slowStopUpdate));
        schedule(schedule_selector(Enemy::slowStopUpdate), 1/60);
        return;
    }
    m_SlowStopTime.mCurTime = 0;
    m_SlowStopTime.mTotalTime = time;
    m_bIsSlowAction = false;
    schedule(schedule_selector(Enemy::slowStopUpdate), 1/60);
}

void Enemy::stopEndCallBack(cocos2d::CCNode *pSender, void *pData)
{
    stopEffectAnimation();
    if(this->getPositionY()<=Atk_Barrier_DY+Barrier_Y)
    {
        EnemyStayState *_instance = EnemyStayState::getInstance();
        this->GetFSMSystem()->ChangeState(_instance);
    }
    else
    {
        EnemyMoveState *_instance = EnemyMoveState::getInstance();
        this->GetFSMSystem()->ChangeState(_instance);
    }
}

void Enemy::slowStopUpdate(float dt)
{
    m_SlowStopTime.mCurTime+=dt;
    if(m_SlowStopTime.mCurTime>m_SlowStopTime.mTotalTime)
    {
        m_SlowStopTime.mCurTime = 0;
        m_SlowStopTime.mTotalTime = 0;
        enemyHurt(true);
        unschedule(schedule_selector(Enemy::slowStopUpdate));
        if(m_bIsSlowAction)
        {
            slowDownEndCallback(NULL);
        }
        else
        {
            stopEndCallBack(NULL, NULL);
        }
    }
}

void Enemy::hitMubei()
{
    if(!m_bMuBeiHit)
    {
        m_bMuBeiHit = true;
    }
    else
    {
        m_pSprite->getAnimation()->stop();
        m_pSprite->setVisible(false);
        DATAPOOL->playGameEffect(Die2);
        
        DATAPOOL->addEffectCache(XiaoShiRes);
        CCArmature *arm = CCArmature::create("xiaoshi");
        arm->getAnimation()->play("xiaoshi");
        arm->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(Enemy::animationEvent));
        addChild(arm,2);
    }
}

void Enemy::dieWithBoom()
{
    m_bBoom = true;
}

void Enemy::removeJiaCheng()
{
    for(int i =m_pParentLayer->getEnemyArray()->count()-1 ;i>=0 ;i--)
    {
        Enemy *_enemy = (Enemy*)m_pParentLayer->getEnemyArray()->objectAtIndex(i);
        if(_enemy&&_enemy->getType()!=Enemy_Type_XiaoGui)
        {
            _enemy->CancelJiaCheng();
        }
    }
}

void Enemy::removeMofalun()
{
    CCArmature *mofalun = (CCArmature*)getChildByTag(Effect_MoFaLun_B);
    if (mofalun) {
        mofalun->removeFromParentAndCleanup(true);
    }
}
