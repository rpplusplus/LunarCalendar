//
//  lunar.cpp
//  LunarCore
//
//  Created by Xiaoxuan Tang on 2/12/15.
//  Copyright (c) 2015 liwushuo. All rights reserved.
//

#include <stdio.h>
#include <time.h>
#include <string>
#include <vector>
#include <math.h>

#include "lunar.h"

/**
 *  农历1900-2100的润大小信息表
 */
static int32_t lunarInfo[] = {0x04bd8,0x04ae0,0x0a570,0x054d5,0x0d260,0x0d950,0x16554,0x056a0,0x09ad0,0x055d2,//1900-1909
                              0x04ae0,0x0a5b6,0x0a4d0,0x0d250,0x1d255,0x0b540,0x0d6a0,0x0ada2,0x095b0,0x14977,//1910-1919
                              0x04970,0x0a4b0,0x0b4b5,0x06a50,0x06d40,0x1ab54,0x02b60,0x09570,0x052f2,0x04970,//1920-1929
                              0x06566,0x0d4a0,0x0ea50,0x06e95,0x05ad0,0x02b60,0x186e3,0x092e0,0x1c8d7,0x0c950,//1930-1939
                              0x0d4a0,0x1d8a6,0x0b550,0x056a0,0x1a5b4,0x025d0,0x092d0,0x0d2b2,0x0a950,0x0b557,//1940-1949
                              0x06ca0,0x0b550,0x15355,0x04da0,0x0a5b0,0x14573,0x052b0,0x0a9a8,0x0e950,0x06aa0,//1950-1959
                              0x0aea6,0x0ab50,0x04b60,0x0aae4,0x0a570,0x05260,0x0f263,0x0d950,0x05b57,0x056a0,//1960-1969
                              0x096d0,0x04dd5,0x04ad0,0x0a4d0,0x0d4d4,0x0d250,0x0d558,0x0b540,0x0b6a0,0x195a6,//1970-1979
                              0x095b0,0x049b0,0x0a974,0x0a4b0,0x0b27a,0x06a50,0x06d40,0x0af46,0x0ab60,0x09570,//1980-1989
                              0x04af5,0x04970,0x064b0,0x074a3,0x0ea50,0x06b58,0x055c0,0x0ab60,0x096d5,0x092e0,//1990-1999
                              0x0c960,0x0d954,0x0d4a0,0x0da50,0x07552,0x056a0,0x0abb7,0x025d0,0x092d0,0x0cab5,//2000-2009
                              0x0a950,0x0b4a0,0x0baa4,0x0ad50,0x055d9,0x04ba0,0x0a5b0,0x15176,0x052b0,0x0a930,//2010-2019
                              0x07954,0x06aa0,0x0ad50,0x05b52,0x04b60,0x0a6e6,0x0a4e0,0x0d260,0x0ea65,0x0d530,//2020-2029
                              0x05aa0,0x076a3,0x096d0,0x04bd7,0x04ad0,0x0a4d0,0x1d0b6,0x0d250,0x0d520,0x0dd45,//2030-2039
                              0x0b5a0,0x056d0,0x055b2,0x049b0,0x0a577,0x0a4b0,0x0aa50,0x1b255,0x06d20,0x0ada0,//2040-2049
                              /**Add By JJonline@JJonline.Cn**/
                              0x14b63,0x09370,0x049f8,0x04970,0x064b0,0x168a6,0x0ea50, 0x06b20,0x1a6c4,0x0aae0,//2050-2059
                              0x0a2e0,0x0d2e3,0x0c960,0x0d557,0x0d4a0,0x0da50,0x05d55,0x056a0,0x0a6d0,0x055d4,//2060-2069
                              0x052d0,0x0a9b8,0x0a950,0x0b4a0,0x0b6a6,0x0ad50,0x055a0,0x0aba4,0x0a5b0,0x052b0,//2070-2079
                              0x0b273,0x06930,0x07337,0x06aa0,0x0ad50,0x14b55,0x04b60,0x0a570,0x054e4,0x0d160,//2080-2089
                              0x0e968,0x0d520,0x0daa0,0x16aa6,0x056d0,0x04ae0,0x0a9d4,0x0a2d0,0x0d150,0x0f252,//2090-2099
                              0x0d520};

/**
 *  公历每个月份的天数普通表
 */
static int32_t solarMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};

/**
 *  天干地支之天干速查表
 */
static std::string Gan[] = {"\u7532","\u4e59","\u4e19","\u4e01","\u620a","\u5df1","\u5e9a","\u8f9b","\u58ec","\u7678"};

/**
 *  天干地支之地支速查表
 */
static std::string Zhi[] = {"\u5b50","\u4e11","\u5bc5","\u536f","\u8fb0","\u5df3","\u5348","\u672a","\u7533","\u9149","\u620c","\u4ea5"};

/**
 *  天干地支之地支速查表<=>生肖
 */
static std::string Animals[] = {"\u9f20","\u725b","\u864e","\u5154","\u9f99","\u86c7","\u9a6c","\u7f8a","\u7334","\u9e21","\u72d7","\u732a"};

/**
 *  24节气速查表
 */
static std::string solarTerm[] = {"\u5c0f\u5bd2","\u5927\u5bd2","\u7acb\u6625","\u96e8\u6c34","\u60ca\u86f0","\u6625\u5206","\u6e05\u660e","\u8c37\u96e8","\u7acb\u590f","\u5c0f\u6ee1","\u8292\u79cd","\u590f\u81f3","\u5c0f\u6691","\u5927\u6691","\u7acb\u79cb","\u5904\u6691","\u767d\u9732","\u79cb\u5206","\u5bd2\u9732","\u971c\u964d","\u7acb\u51ac","\u5c0f\u96ea","\u5927\u96ea","\u51ac\u81f3"};

/**
 *  1900-2100各年的24节气日期速查表
 */
static std::string sTermInfo[] = {"9778397bd097c36b0b6fc9274c91aa","97b6b97bd19801ec9210c965cc920e","97bcf97c3598082c95f8c965cc920f",
				"97bd0b06bdb0722c965ce1cfcc920f","b027097bd097c36b0b6fc9274c91aa","97b6b97bd19801ec9210c965cc920e",
				"97bcf97c359801ec95f8c965cc920f","97bd0b06bdb0722c965ce1cfcc920f","b027097bd097c36b0b6fc9274c91aa",
				"97b6b97bd19801ec9210c965cc920e","97bcf97c359801ec95f8c965cc920f",	"97bd0b06bdb0722c965ce1cfcc920f",
				"b027097bd097c36b0b6fc9274c91aa","9778397bd19801ec9210c965cc920e","97b6b97bd19801ec95f8c965cc920f",
				"97bd09801d98082c95f8e1cfcc920f","97bd097bd097c36b0b6fc9210c8dc2","9778397bd197c36c9210c9274c91aa",
				"97b6b97bd19801ec95f8c965cc920e","97bd09801d98082c95f8e1cfcc920f",	"97bd097bd097c36b0b6fc9210c8dc2",
				"9778397bd097c36c9210c9274c91aa","97b6b97bd19801ec95f8c965cc920e","97bcf97c3598082c95f8e1cfcc920f",
				"97bd097bd097c36b0b6fc9210c8dc2","9778397bd097c36c9210c9274c91aa","97b6b97bd19801ec9210c965cc920e",
				"97bcf97c3598082c95f8c965cc920f","97bd097bd097c35b0b6fc920fb0722","9778397bd097c36b0b6fc9274c91aa",
				"97b6b97bd19801ec9210c965cc920e","97bcf97c3598082c95f8c965cc920f",	"97bd097bd097c35b0b6fc920fb0722",
				"9778397bd097c36b0b6fc9274c91aa","97b6b97bd19801ec9210c965cc920e","97bcf97c359801ec95f8c965cc920f",
				"97bd097bd097c35b0b6fc920fb0722","9778397bd097c36b0b6fc9274c91aa","97b6b97bd19801ec9210c965cc920e",
				"97bcf97c359801ec95f8c965cc920f","97bd097bd097c35b0b6fc920fb0722","9778397bd097c36b0b6fc9274c91aa",
				"97b6b97bd19801ec9210c965cc920e","97bcf97c359801ec95f8c965cc920f",	"97bd097bd07f595b0b6fc920fb0722",
				"9778397bd097c36b0b6fc9210c8dc2","9778397bd19801ec9210c9274c920e","97b6b97bd19801ec95f8c965cc920f",
				"97bd07f5307f595b0b0bc920fb0722","7f0e397bd097c36b0b6fc9210c8dc2","9778397bd097c36c9210c9274c920e",
				"97b6b97bd19801ec95f8c965cc920f","97bd07f5307f595b0b0bc920fb0722","7f0e397bd097c36b0b6fc9210c8dc2",
				"9778397bd097c36c9210c9274c91aa","97b6b97bd19801ec9210c965cc920e","97bd07f1487f595b0b0bc920fb0722",
				"7f0e397bd097c36b0b6fc9210c8dc2","9778397bd097c36b0b6fc9274c91aa","97b6b97bd19801ec9210c965cc920e",
				"97bcf7f1487f595b0b0bb0b6fb0722","7f0e397bd097c35b0b6fc920fb0722",	"9778397bd097c36b0b6fc9274c91aa",
				"97b6b97bd19801ec9210c965cc920e","97bcf7f1487f595b0b0bb0b6fb0722","7f0e397bd097c35b0b6fc920fb0722",
				"9778397bd097c36b0b6fc9274c91aa","97b6b97bd19801ec9210c965cc920e","97bcf7f1487f531b0b0bb0b6fb0722",
				"7f0e397bd097c35b0b6fc920fb0722","9778397bd097c36b0b6fc9274c91aa","97b6b97bd19801ec9210c965cc920e",
				"97bcf7f1487f531b0b0bb0b6fb0722","7f0e397bd07f595b0b6fc920fb0722",	"9778397bd097c36b0b6fc9274c91aa",
				"97b6b97bd19801ec9210c9274c920e","97bcf7f0e47f531b0b0bb0b6fb0722","7f0e397bd07f595b0b0bc920fb0722",
				"9778397bd097c36b0b6fc9210c91aa","97b6b97bd197c36c9210c9274c920e","97bcf7f0e47f531b0b0bb0b6fb0722",
				"7f0e397bd07f595b0b0bc920fb0722","9778397bd097c36b0b6fc9210c8dc2","9778397bd097c36c9210c9274c920e",
				"97b6b7f0e47f531b0723b0b6fb0722","7f0e37f5307f595b0b0bc920fb0722",	"7f0e397bd097c36b0b6fc9210c8dc2",
				"9778397bd097c36b0b70c9274c91aa","97b6b7f0e47f531b0723b0b6fb0721","7f0e37f1487f595b0b0bb0b6fb0722",
				"7f0e397bd097c35b0b6fc9210c8dc2","9778397bd097c36b0b6fc9274c91aa","97b6b7f0e47f531b0723b0b6fb0721",
				"7f0e27f1487f595b0b0bb0b6fb0722","7f0e397bd097c35b0b6fc920fb0722",	"9778397bd097c36b0b6fc9274c91aa",
				"97b6b7f0e47f531b0723b0b6fb0721","7f0e27f1487f531b0b0bb0b6fb0722","7f0e397bd097c35b0b6fc920fb0722",
				"9778397bd097c36b0b6fc9274c91aa","97b6b7f0e47f531b0723b0b6fb0721","7f0e27f1487f531b0b0bb0b6fb0722",
				"7f0e397bd097c35b0b6fc920fb0722","9778397bd097c36b0b6fc9274c91aa","97b6b7f0e47f531b0723b0b6fb0721",
				"7f0e27f1487f531b0b0bb0b6fb0722","7f0e397bd07f595b0b0bc920fb0722",	"9778397bd097c36b0b6fc9274c91aa",
				"97b6b7f0e47f531b0723b0787b0721","7f0e27f0e47f531b0b0bb0b6fb0722","7f0e397bd07f595b0b0bc920fb0722",
				"9778397bd097c36b0b6fc9210c91aa","97b6b7f0e47f149b0723b0787b0721","7f0e27f0e47f531b0723b0b6fb0722",
				"7f0e397bd07f595b0b0bc920fb0722","9778397bd097c36b0b6fc9210c8dc2","977837f0e37f149b0723b0787b0721",
				"7f07e7f0e47f531b0723b0b6fb0722","7f0e37f5307f595b0b0bc920fb0722","7f0e397bd097c35b0b6fc9210c8dc2",
				"977837f0e37f14998082b0787b0721","7f07e7f0e47f531b0723b0b6fb0721","7f0e37f1487f595b0b0bb0b6fb0722",
				"7f0e397bd097c35b0b6fc9210c8dc2","977837f0e37f14998082b0787b06bd","7f07e7f0e47f531b0723b0b6fb0721",
				"7f0e27f1487f531b0b0bb0b6fb0722","7f0e397bd097c35b0b6fc920fb0722","977837f0e37f14998082b0787b06bd",
				"7f07e7f0e47f531b0723b0b6fb0721","7f0e27f1487f531b0b0bb0b6fb0722","7f0e397bd097c35b0b6fc920fb0722",
				"977837f0e37f14998082b0787b06bd","7f07e7f0e47f531b0723b0b6fb0721","7f0e27f1487f531b0b0bb0b6fb0722",
				"7f0e397bd07f595b0b0bc920fb0722","977837f0e37f14998082b0787b06bd","7f07e7f0e47f531b0723b0b6fb0721",
				"7f0e27f1487f531b0b0bb0b6fb0722","7f0e397bd07f595b0b0bc920fb0722",	"977837f0e37f14998082b0787b06bd",
				"7f07e7f0e47f149b0723b0787b0721","7f0e27f0e47f531b0b0bb0b6fb0722","7f0e397bd07f595b0b0bc920fb0722",
				"977837f0e37f14998082b0723b06bd","7f07e7f0e37f149b0723b0787b0721","7f0e27f0e47f531b0723b0b6fb0722",
				"7f0e397bd07f595b0b0bc920fb0722","977837f0e37f14898082b0723b02d5","7ec967f0e37f14998082b0787b0721",
				"7f07e7f0e47f531b0723b0b6fb0722","7f0e37f1487f595b0b0bb0b6fb0722","7f0e37f0e37f14898082b0723b02d5",
				"7ec967f0e37f14998082b0787b0721","7f07e7f0e47f531b0723b0b6fb0722","7f0e37f1487f531b0b0bb0b6fb0722",
				"7f0e37f0e37f14898082b0723b02d5","7ec967f0e37f14998082b0787b06bd","7f07e7f0e47f531b0723b0b6fb0721",
				"7f0e37f1487f531b0b0bb0b6fb0722","7f0e37f0e37f14898082b072297c35","7ec967f0e37f14998082b0787b06bd",
				"7f07e7f0e47f531b0723b0b6fb0721","7f0e27f1487f531b0b0bb0b6fb0722","7f0e37f0e37f14898082b072297c35",
				"7ec967f0e37f14998082b0787b06bd","7f07e7f0e47f531b0723b0b6fb0721",	"7f0e27f1487f531b0b0bb0b6fb0722",
				"7f0e37f0e366aa89801eb072297c35","7ec967f0e37f14998082b0787b06bd","7f07e7f0e47f149b0723b0787b0721",
				"7f0e27f1487f531b0b0bb0b6fb0722","7f0e37f0e366aa89801eb072297c35","7ec967f0e37f14998082b0723b06bd",
				"7f07e7f0e47f149b0723b0787b0721","7f0e27f0e47f531b0723b0b6fb0722","7f0e37f0e366aa89801eb072297c35",
				"7ec967f0e37f14998082b0723b06bd","7f07e7f0e37f14998083b0787b0721","7f0e27f0e47f531b0723b0b6fb0722",
				"7f0e37f0e366aa89801eb072297c35","7ec967f0e37f14898082b0723b02d5","7f07e7f0e37f14998082b0787b0721",
				"7f07e7f0e47f531b0723b0b6fb0722","7f0e36665b66aa89801e9808297c35",	"665f67f0e37f14898082b0723b02d5",
				"7ec967f0e37f14998082b0787b0721","7f07e7f0e47f531b0723b0b6fb0722",	"7f0e36665b66a449801e9808297c35",
				"665f67f0e37f14898082b0723b02d5","7ec967f0e37f14998082b0787b06bd","7f07e7f0e47f531b0723b0b6fb0721",
				"7f0e36665b66a449801e9808297c35","665f67f0e37f14898082b072297c35",	"7ec967f0e37f14998082b0787b06bd",
				"7f07e7f0e47f531b0723b0b6fb0721","7f0e26665b66a449801e9808297c35",	"665f67f0e37f1489801eb072297c35",
				"7ec967f0e37f14998082b0787b06bd","7f07e7f0e47f531b0723b0b6fb0721",	"7f0e27f1487f531b0b0bb0b6fb0722"};

/**
 *  数字转中文速查表
 */
static std::string nStr1[] = {"\u65e5","\u4e00","\u4e8c","\u4e09","\u56db","\u4e94","\u516d","\u4e03","\u516b","\u4e5d","\u5341"};

/**
 *  日期转农历称呼速查表
 */
static std::string nStr2[] = {"\u521d","\u5341","\u5eff","\u5345"};

/**
 *  月份转农历称呼速查表
 */
static std::string nStr3[] = {"\u6b63","\u4e8c","\u4e09","\u56db","\u4e94","\u516d","\u4e03","\u516b","\u4e5d","\u5341","\u51ac","\u814a"};

int32_t Lunar::lYearDays( int32_t year )
{
    int32_t i, sum = 348;
    for( i=0x8000; i>0x8; i>>=1 )
    {
        sum += ( lunarInfo[ year - 1900 ] & i ) ? 1 : 0;
    }
    return( sum+ this->leapDays( year ) );
}

int32_t Lunar::leapMonth( int32_t year )
{
    return( lunarInfo[ year - 1900 ] & 0xf );
}

int32_t Lunar::leapDays( int32_t year )
{
    if ( this->leapMonth( year ) )
    {
        return( lunarInfo[year-1900] & 0x10000 ) ? 30 : 29;
    }
    return 0;
}

int32_t Lunar::monthDays( int32_t year, int32_t month )
{
    if( month > 12 || month < 1)
    {
        return -1; //月份参数从1至12，参数错误返回-1
    }
    
    return ( lunarInfo[year - 1900 ] & ( 0x10000 >> month ) ) ? 30 : 29 ;
}

int32_t Lunar::solarDays( int32_t year, int32_t month )
{
    if( month > 12 || month < 1) { return -1; } //若参数错误 返回-1

    int32_t ms = month - 1;
    if( ms == 1 )
    { //2月份的闰平规律测算后确认返回28或29
        return ((( year % 4 == 0 ) && ( year % 100 != 0 )) || ( year % 400 == 0 )) ? 29 : 28;
    } else
    {
        return( solarMonth[ ms ] );
    }
}

std::string Lunar::toGanZhi( int32_t offset )
{
    return( Gan[ offset % 10 ] + Zhi[ offset % 12 ] );
}

int32_t Lunar::getTerm( int32_t year, int32_t number )
{
    if ( year < 1900 || year > 2100 ) { return -1; }
    if ( number < 1 || number > 24 ) { return -1; }
    
    std::string _table = sTermInfo[ year - 1900 ];
    std::vector< std::string > _info = std::vector< std::string >();
    
    for (int32_t i=0; i<6; i++) {
        std::string tmp = _table.substr( i * 5, 5 );
        tmp = "0x" + tmp;
        int32_t number = (int32_t) strtol( tmp.c_str(), NULL, 16 );
        _info.push_back( std::to_string( number ) );
    }
    
    std::vector< int32_t > _calday = std::vector< int32_t >();
    
    for (int32_t i=0; i<6; i++) {
        _calday.push_back( std::stoi( _info[ i ].substr( 0, 1 ) ));
        _calday.push_back( std::stoi( _info[ i ].substr( 1, 2 ) ));
        _calday.push_back( std::stoi( _info[ i ].substr( 3, 1 ) ));
        _calday.push_back( std::stoi( _info[ i ].substr( 4, 2 ) ));
    }
    
    return _calday[ number - 1 ];
}

std::string Lunar::toChinaMonth( int32_t month )
{
    if ( month > 12 || month < 1 ) { return ""; } //若参数错误 返回""
    std::string s = nStr3[ month - 1 ];
    s+= "\u6708";//加上月字
    return s;
}

std::string Lunar::toChinaDay( int32_t day )
{
    std::string s;
    
    switch ( day ) {
        case 10:
            s = "\u521d\u5341"; break;
        case 20:
            s = "\u4e8c\u5341"; break;
            break;
        case 30:
            s = "\u4e09\u5341"; break;
            break;
        default :
            
            s = nStr2[ (int32_t) floor( day / 10 ) ];
            s += nStr1[ day % 10 ];
    }
    
    return( s );
}

std::string Lunar::getAnimal( int32_t year )
{
    return Animals[ ( year - 4 ) % 12 ];
}

int32_t Lunar::deltaDaysWith19000131(int32_t year, int32_t month, int32_t day)
{
    if (year == 1900)
    {
        int32_t s = 0;
        for (int32_t m=2; m<month; m++) {
            s += this->solarDays(year, m);
        }
        
        if (month != 1)
            s += day;
        
        return s;
    }
    else
    {
        int32_t s = 365 - 31;
        for (int32_t i=1901; i<year; i++)
        {
            s += ((i % 4 == 0 && i % 100 != 0) || i % 400 == 0) ? 366 : 365;
        }
        
        if (month != 1)
        {
            for (int32_t i=1; i < month; i++) {
                s += this->solarDays(year, i);
            }
        }
        
        s += day;
        
        return s;
    }
}

LunarObj* Lunar::solar2lunar( int32_t year, int32_t month, int32_t day )
{
    if ( year < 1900 || year > 2100 ) return NULL;
    if ( year == 1900 && month ==1 && day < 31) return NULL;
    
    int32_t i = 0, temp = 0, leap = 0;
    
    int32_t offset = this->deltaDaysWith19000131(year, month, day);

    for (i=1900; i<2101 && offset > 0; i++)
    {
        temp = lYearDays( i );
        offset -= temp;
    }
    
    if ( offset < 0 )  {
        offset += temp;
        i--;
    }
    
#warning TODO: 是否是今天
#warning TODO: 星期几
    
    int32_t lunarYear = i;
    leap = this->leapMonth( i );
    
    bool isLeap = false;
    
    for ( i=1; i<13 && offset > 0; i++ )
    {
        if ( leap > 0 && i == ( leap + 1 ) && isLeap == false )
        {
            --i;
            isLeap = true;
            temp = this->leapDays( lunarYear );
        }
        else
        {
            temp = this->monthDays( lunarYear, i);
        }
        
        if ( isLeap == true && i == ( leap + 1 ))
        {
            isLeap = false;
        }
        
        offset -= temp;
    }
    
    if ( offset == 0 && leap > 0 && i == leap + 1 )
    {
        if ( isLeap ) {
            isLeap = false;
        }
        else
        {
            isLeap = true;
            --i;
        }
    }
    
    if ( offset < 0 )
    {
        offset += temp;
        --i;
    }
    
    int32_t lunarMonth = i;
    int32_t lunarDay = (int32_t) offset + 1;
    
    int32_t sm = month - 1;
    int32_t term3 = this->getTerm( lunarYear, 3 );
    std::string gzY = this->toGanZhi( lunarYear - 4 );
    
    if ( sm < 2 && day < term3 ) {
        gzY = this->toGanZhi( lunarYear - 5);
    }
    else
    {
        gzY = this->toGanZhi( lunarYear - 4);
    }
    
    int32_t firstNode = this->getTerm( year, month * 2 - 1 );
    int32_t secondNode = this->getTerm( year, month * 2);
    
    std::string gzM = this->toGanZhi( ( year - 1900 ) * 12 + month + 11 );
    
    if ( day >= firstNode ) {
        gzM = this->toGanZhi( ( year - 1900 ) * 12 + month + 12 );
    }

    bool isTerm = false;
    std::string Term = "";
    
    if ( firstNode == day )
    {
        isTerm = true;
        Term = solarTerm[ month * 2 - 2 ];
    }
    
    if ( secondNode == day )
    {
        isTerm = true;
        Term = solarTerm[ month * 2 -1 ];
    }
    
    int32_t dayCyclical =  (int32_t) (deltaDaysWith19000131(year, sm+1, 1)+30)  + 10;
    std::string gzD = this->toGanZhi( dayCyclical + day - 1);
    
    LunarObj* obj = new LunarObj;
    
    obj->lunarYear = lunarYear;
    obj->lunarMonth = lunarMonth;
    obj->lunarDay = lunarDay;
    
    obj->animal = this->getAnimal( lunarYear );
    obj->lunarMonthChineseName = ( isLeap ? "\u95f0" : "" ) + this->toChinaMonth( lunarMonth );
    obj->lunarDayChineseName = this->toChinaDay( lunarDay );
    
    obj->solarYear = year;
    obj->solarMonth = month;
    obj->solarDay = day;
    
    obj->ganzhiYear = gzY;
    obj->ganzhiMonth = gzM;
    obj->ganzhiDay = gzD;
    
    obj->isLeap = isLeap;
    obj->term = Term;
    obj->isTerm = isTerm;
    
    return obj;
}

std::string timeStampToHReadble(const time_t rawtime)
{
    struct tm * dt;
    char buffer [30];
    dt = localtime(&rawtime);
    strftime(buffer, sizeof(buffer), "%m-%d-%H-%M-%y", dt);
    return std::string(buffer);
}

LunarObj* Lunar::lunar2solar( int32_t year, int32_t month, int32_t day, bool isLeapMonth )
{
    int32_t leapMonth = this->leapMonth( year );
    
    if ( isLeapMonth && leapMonth != month ) return NULL;
    if ( (year == 2100 && month == 12 && day > 1) || (year == 1900 && month == 1 && day < 31) ) return NULL;
    int32_t dd = this->monthDays( year, month );
    if (year < 1900 || year > 2100 || day> ((isLeapMonth)?this->leapDays(year):dd)) return NULL;
    
    int32_t offset = 0;
    
    for (int i=1900; i<year; i++) {
        offset += this->lYearDays( i );
    }
    
    int32_t leap = 0; bool isAdd = false;
    for (int32_t i = 1; i <month; i++) {
        leap = this->leapMonth(year);
        if (!isAdd) {
            if (leap <= i && leap > 0) {
                offset += this->leapDays( year );
                isAdd = true;
            }
        }
        
        offset += this->monthDays(year, i);
    }
    
    if (isLeapMonth) offset += dd;
    
    int32_t y = 1900;
    int32_t m = 1;
    int32_t d = 30;
    offset = offset + day;
    
    while (true) {
        int yy = y;
        int t = 365 + ((yy % 4 == 0 && yy % 100 != 0) || yy % 400 == 0);
        
        if (offset >= t)
        {
            offset -= t;
            y ++;
        }
        else
        {
            for (int i=0; i<12; i++)
            {
                int tmp = solarMonth[i];
                if (t == 366 && i == 1) tmp ++;
                
                if (offset > tmp)
                {
                    offset -= tmp;
                }
                else
                {
                    m = i + 1;
                    d += offset;
                    
                    int a = i;
                    while (d > tmp)
                    {
                        d -= tmp;
                        m += 1;
                        
                        if (m > 12)
                        {
                            y ++;
                            m = 1;
                        }
                        
                        tmp = solarMonth[(a+1) % 12];
                        if (t == 366 && a+1 == 1) tmp ++;
                        a++;
                    }
                    
                    break;
                }
            }
            break;
        }
    }
    
    return this->solar2lunar( y, m, d );
}

