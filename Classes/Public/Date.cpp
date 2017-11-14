//
//  Date.cpp
//  SingleEye
//
//  Created by luoxp on 3/10/14.
//
//

#include "Date.h"


int day_tab[2][12]={{31,28,31,30,31,30,31,31,30,31,30,31},{31,29,31,30,31,30,31,31,30,31,30,31}};

Date::Date(bool bInit)
{
    //获取当前时间
    unsigned long long timestamp = time(NULL);
    
    struct tm *tm;
    tm = localtime((time_t*)&timestamp);

    year = tm->tm_year + 1900;
    month = tm->tm_mon + 1;
    day = tm->tm_mday;
}


bool Date::leap(int year)       //判断指定的年份日否为闰年;
{
    if((year%4==0 && year%100!=0) || year%400==0)
        return true;
    else
        return false;
}


int Date::dton(Date &d)       //将指定的日期转换成从0年0月0日起的天数;
{
    int y,m,days=0;
    for(y=1;y<=d.year;y++)
        if(leap(y)) days+=366;
        else days+=365;
    for(m=0;m<d.month-1;m++)
        if(leap(d.year)) days+=day_tab[1][m];
        else days+=day_tab[0][m];
    days+=d.day;
    return days;
}


Date Date::ntod(int n)       //将指定的0年0月0日起的天数转换成对应的日期;
{
    int y=1,m=1,d,rest=n,lp;
    while(1)
    {
        if(leap(y))
        {
            if(rest<=366) break;
            else rest-=366;
        }
        else
        {
            if(rest<=365) break;
            else rest-=365;
        }
        y++;
    }
    y--;
    lp=leap(y);
    while(true)
    {
        if(lp)
        {
            if(rest>day_tab[1][m-1]) rest-=day_tab[1][m-1];
            else break;
        }
        else
        {
            if(rest>day_tab[0][m-1]) rest-=day_tab[0][m-1];
            else break;
        }
        m++;
    }
    d=rest;
    return Date(y,m,d);
}
