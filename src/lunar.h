//
//  lunar.h
//  LunarCore
//
//  Created by Xiaoxuan Tang on 2/12/15.
//  Copyright (c) 2015 liwushuo. All rights reserved.
//

#ifndef LunarCore_lunar_h
#define LunarCore_lunar_h

#include <iostream>
#include <stdio.h>

struct LunarObj {
    int32_t lunarYear, lunarMonth, lunarDay, solarYear, solarMonth, solarDay, weekNumber;
    std::string animal, lunarMonthChineseName, lunarDayChineseName, ganzhiYear, ganzhiMonth, ganzhiDay, term, weekChineseName;
    bool isToday, isLeap, isTerm;
};

class Lunar {
    
public:
    /**
     *  返回农历 year 年一整年的总天数
     *
     *  @param year(1900~2100) 农历年
     *
     *  @return 总天数
     */
    int32_t lYearDays( int32_t year );
    
    /**
     *  返回农历 year 年闰月时哪个月；若 year 年没有闰月 则返回0
     *
     *  @param year(1900~2100) 农历年
     *
     *  @return 0-12
     */
    int32_t leapMonth( int32_t year );
    
    /**
     *  返回农历 year 年 闰月的天数 若该年没有闰月则返回 0
     *
     *  @param year(1900~2100) 农历年
     *
     *  @return 0/29/30
     */
    int32_t leapDays( int32_t year );
    
    /**
     *  返回农历 year 年 month 月 (非闰月) 的总天数，计算 month 为闰月时的天数请使用 leapDays 方法
     *
     *  @param year(1900~2100)  农历年
     *  @param month(1~12) 农历月
     *
     *  @return -1/29/30
     */
    int32_t monthDays( int32_t year, int32_t month );
    
    /**
     *  返回公历年 year 年 month 月的天数
     *
     *  @param year(1900~2100)  公历年
     *  @param month(1~12) 公历月
     *
     *  @return -1/28/29/30/31
     */
    int32_t solarDays( int32_t year, int32_t month );
    
    /**
     *  传入 offset 偏移量，返回干支
     *
     *  @param offset offset 相对甲子的偏移量
     *
     *  @return 中文字付串
     */
    std::string toGanZhi( int32_t offset );
    
    /**
     *  传入公历年份 year 获得该年第 number 个节气的公历日期
     *
     *  @param year(1~24)   公历年
     *  @param number(1~24) 二十四节气中的第几个节气 ; 从 number = 1 (小寒) 算起
     *
     *  @return 一个数字，代表日期
     */
    int32_t getTerm( int32_t year, int32_t number );
    
    /**
     *  传入农历数字月份返回汉语通俗表示法
     *
     *  @param month(1~12) 农历月
     *
     *  @return 中文字符串
     */
    std::string toChinaMonth( int32_t month );
    
    /**
     *  传入农历日期数字返回汉字表示法
     *
     *  @param day(1~30) 农历日
     *
     *  @return 中文字符串
     */
    std::string toChinaDay( int32_t day );
    
    /**
     *  年份生肖转换，注意粗略转换；生肖分界线是立春
     *
     *  @param year(1900~2100) 农历年
     *
     *  @return 中文字符串
     */
    std::string getAnimal( int32_t year );
    
    int32_t deltaDaysWith19000131(int32_t year, int32_t month, int32_t day);
        
    /**
     *  传入公历年月日获得详细的公历农历信息，参数区间1900.1.31~2100.12.31
     *
     *  @param year  公历年
     *  @param month 公历月
     *  @param day   公历日
     *
     *  @return obj
     */
    LunarObj* solar2lunar( int32_t year, int32_t month, int32_t day);
    
    /**
     *  传入农历年月日以及传入的月份是否闰月获得详细的公历、农历信息，参数区间1900.1.31~2100.12.1
     *
     *  @param year         农历年
     *  @param month        农历月
     *  @param day          农历日
     *  @param isLeapMonth  是否是闰月
     *
     *  @return obj
     */
    LunarObj* lunar2solar( int32_t year, int32_t month, int32_t day, bool isLeapMonth);
};

#endif
