//
//  ScrollText.cpp
//  SingleEye
//
//  Created by luoxp on 1/28/14.
//
//

#include "ScrollText.h"


ScrollText* ScrollText::create(const char *text, CCSize sizeView, const CCPoint &pos)
{
    ScrollText* widget = new ScrollText();
    widget->m_sizeView = sizeView;
    
    if (widget && widget->init(text, pos))
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return NULL;
}


bool ScrollText::init(const char *text, const CCPoint &pos)
{
    UIWidget::init();    
        
    //创建文本区
    m_uiText = UITextArea::create();
    
    m_uiText->setTextAreaSize(CCSizeMake(m_sizeView.width, 0)); //高度一定要设置为0
    m_uiText->setFontSize(40);
    m_uiText->setText(text);
    m_uiText->setTextHorizontalAlignment(kCCTextAlignmentCenter);
    m_uiText->setPosition(ccp(m_uiText->getContentSize().width/2, m_uiText->getContentSize().height/2));
    
    //设置滚动层
    m_pScroll = UIScrollView::create();
    m_pScroll->setSize(m_sizeView);
    m_pScroll->setInnerContainerSize(m_uiText->getContentSize());
    m_pScroll->setTouchEnable(true);
    m_pScroll->setAnchorPoint(ccp(0.5f, 0.5f));    

    m_pScroll->addChild(m_uiText);

    addChild(m_pScroll);
    setPosition(pos); 
    setPositionEx();
        
    return true;
}


void ScrollText::setText(const char * txt)
{
    m_uiText->setText(txt);
    
    m_uiText->setPosition(ccp(m_uiText->getContentSize().width/2, m_uiText->getContentSize().height/2));    
    m_pScroll->setInnerContainerSize(m_uiText->getContentSize());
    setPositionEx();
}


void ScrollText::setFontSize(int size)
{
    m_uiText->setFontSize(size);
    
    m_uiText->setPosition(ccp(m_uiText->getContentSize().width/2, m_uiText->getContentSize().height/2));       
    m_pScroll->setInnerContainerSize(m_uiText->getContentSize());
    setPositionEx();    
}


void ScrollText::setPositionEx()
{       
    //默认SCROLLVIEW_DIR_VERTICAL
    if (m_sizeView.equals(m_pScroll->getInnerContainerSize())) {
               //居中对齐
        CCSize sizeText = m_uiText->getContentSize(); 
        m_pScroll->setPosition(ccp(0, (m_sizeView.height - sizeText.height)/2));
    }
    else
    {
        //设置old 坐标
        m_pScroll->setPosition(CCPointZero);
    }
}


