//
//  ScrollText.h
//  SingleEye
//
//  Created by luoxp on 1/28/14.
//
//

#ifndef __SingleEye__ScrollText__
#define __SingleEye__ScrollText__

#include "cocos-ext.h"


USING_NS_CC;
USING_NS_CC_EXT;


class ScrollText : public UIWidget {
public:
    static ScrollText* create(const char *text, CCSize sizeView, const CCPoint &pos);
    void setText(const char * text);
    void setFontSize(int size);
    
private:    
    bool init(const char *text, const CCPoint &pos);
    void setPositionEx();
    
    CCSize  m_sizeView;
    
    UIScrollView*   m_pScroll;    
    UITextArea*     m_uiText;    
};




#endif /* defined(__SingleEye__ScrollText__) */
