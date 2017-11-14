//
//  Date.h
//  SingleEye
//
//  Created by luoxp on 3/10/14.
//
//

#ifndef __SingleEye__Date__
#define __SingleEye__Date__

#include <iostream>

using namespace std;


class Date
{
    bool leap(int);
    int dton(Date &d);
    Date ntod(int n);

public:    
    int year,month,day;
    
public:
    Date(){}    
    Date(bool bInit);
    Date(int y,int m,int d) {year=y;month=m;day=d;}//重载构造函数;
    Date(const Date &d)        //复制构造函数;
    {
        year=d.year;
        month=d.month;
        day=d.day;
    }
    
    
    Date operator+(int days)      //返回一日期加上天数得到的日期;
    {
        static Date date;
        int number=dton(*this)+days;
        date=ntod(number);
        return date;
    }
    
    Date operator-(int days)     //返回一日期减去天数得到的日期;
    {
        static Date date;
        int number=dton(*this)-days;
        date=ntod(number);
        return date;
    }
    
    int operator-(Date &b)      //返回两日期相差的天数;
    {
        int days=dton(*this)-dton(b)-1;
        return days;
    }
    
    bool operator < (const Date d)
    {
        if((year<d.year) || (year==d.year && month<d.month) || (year==d.year && month==d.month && day<d.day))
            return true;
        else
            return false;
    }
    
    bool operator == (const Date d)
    {
        if((year==d.year && month==d.month && day==d.day))
            return true;
        else
            return false;
    }
    
    bool operator > (const Date d)
    {
        if((year>d.year) || (year==d.year && month>d.month) ||(year==d.year && month==d.month && day>d.day))
            return true;
        else 
            return false;
    }
    
    void getDate()
    {
        cout<<"请输入日期:"<<endl;
        cout<<"年份:";
        cin>>year;
        cout<<"月份:";
        cin>>month;
        cout<<"日期:";
        cin>>day;
    }
    
    void display(){cout<<year<<"."<<month<<"."<<day<<endl;}
};
        
        
        
        
#endif /* defined(__SingleEye__Date__) */
