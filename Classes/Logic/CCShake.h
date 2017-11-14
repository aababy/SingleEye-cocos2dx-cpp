//
//  CCShake.h
//  SingleEye
//
//  Created by qinfen on 14-1-21.
//
//

#ifndef __SingleEye__CCShake__
#define __SingleEye__CCShake__

#include "GameInclude.h"
USING_NS_CC;


class CCShake : public cocos2d::CCActionInterval
{
public:
    CCShake();
    
    // Create the action with a time and a strength (same in x and y)
    static CCShake *create(float d, float strength );
    // Create the action with a time and strengths (different in x and y)
    static CCShake *createWithStrength(float d, float strength_x, float strength_y );
    bool initWithDuration(float d, float strength_x, float strength_y );
    
protected:
    
    void startWithTarget(cocos2d::CCNode *pTarget);
    void update(float time);
    void stop(void);
    
    CCPoint m_StartPosition;
    
    
    // Strength of the action
    float m_strength_x, m_strength_y;
};

#endif /* defined(__SingleEye__CCShake__) */
