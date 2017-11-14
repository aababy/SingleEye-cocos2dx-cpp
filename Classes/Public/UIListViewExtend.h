//
//  UIListViewExtend.h
//  SingleEye
//
//  Created by gwsoft on 1/6/14.
//
//

#ifndef __SingleEye__UIListViewExtendX__
#define __SingleEye__UIListViewExtendX__

#include "GameInclude.h"


USING_NS_CC;
USING_NS_CC_EXT;


class TableViewExtend
{
    class CAccessor : public CCTableView
    {
        friend class TableViewExtend;
    };
    
public:
    int iStartTag;
    CAccessor  *me;
    
    TableViewExtend(CCTableView *m_pList, int iStartTag)
    {
        this->iStartTag = iStartTag;
        me = (CAccessor*)m_pList;
    }

    
    float getChildrenPosition()
    {
        return me->getContentOffset().y;
    }
    
};

//
//class UIListViewExtend
//{
//    class CAccessor1 : public UIListView
//    {
//        friend class UIListViewExtend;
//    };
//    
//public:
//    int iStartTag;
//    CAccessor1  *me;
//    
//    UIListViewExtend(UIListView *m_pList, int iStartTag)
//    {
//        this->iStartTag = iStartTag;
//        me = (CAccessor1*)m_pList;
//    }
//    
//    bool scrollChildrenEx(float touchOffset)
//    {
//        me->m_eMoveDirection = LISTVIEW_MOVE_DIR_DOWN;
//        return me->scrollChildren(touchOffset);
//    }
//    
//    
//    float getChildrenPosition()
//    {
//        //ccArray* arrayChildren = me->m_children->data;
//        UIPanel* child = getChildPositive();
//        
//        int iBetweenCount = child->getTag() - iStartTag;
//        //实际顶部Children的Y值 - 列表的高度 - 行高 = 0 (初始情况)
//        float fVirtualChildren0_Y = child->getPosition().y + me->m_fDisBetweenChild - me->getSize().height;
//        
//        fVirtualChildren0_Y += iBetweenCount * me->m_fDisBetweenChild;
//        
//        CCLOG("**********abc %f %d", fVirtualChildren0_Y, iBetweenCount);
//        
//        return fVirtualChildren0_Y;
//    }
//    
//    
//    UIPanel* getChildPositive()
//    {
//        UIPanel* child = NULL;
//        ccArray* arrayChildren = me->m_children->data;
//       
//        for(int i = 0; i < arrayChildren->num; i++)
//        {
//            child = static_cast<UIPanel*>(arrayChildren->arr[i]);
//
//            if(child->getPosition().y >= 0)
//            {
//                break;
//            }
//            else
//            {
//                child = NULL;
//            }
//        }
//        
//        
//        return child;
//    }
//};


#endif /* defined(__SingleEye__UIListViewExtend__) */
