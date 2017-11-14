//
//  Timer.h
//  SingleEye
//
//  Created by luoxp on 4/3/14.
//
//

#ifndef __SingleEye__Timer__
#define __SingleEye__Timer__


#include "GameInclude.h"


#define xTimer          Timer::getInstance()


class Timer : public CCNode {        

public:
    static Timer * getInstance();
    bool init();
    
private:
    Timer();    
    ~Timer();    
    void checkIsTomorrow(float dt);

};



#endif /* defined(__SingleEye__Timer__) */
