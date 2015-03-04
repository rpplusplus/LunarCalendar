//
//  TTLunarCalendar.m
//  JSCoretest
//
//  Created by Xiaoxuan Tang on 2/12/15.
//  Copyright (c) 2015 liwushuo. All rights reserved.
//

#import "TTLunarCalendar.h"

#import <JavaScriptCore/JavaScriptCore.h>

@interface TTLunarCalendarCore : NSObject

@property (nonatomic, strong) JSContext* jsContext;

+ (TTLunarCalendarCore* ) sharedCore;

- (TTLunarDate*) convertFromGeneralDateYear: (NSInteger) year
                                      month: (NSInteger) month
                                        day: (NSInteger) day;

- (TTLunarDate*) convertFromLunarDateYear: (NSInteger) year
                                    month: (NSInteger) month
                                      day: (NSInteger) day
                                   isLeap: (BOOL) isLeap;;

- (NSInteger) leapMonthWithLunarYear: (NSInteger) lunarYear;

- (NSString* ) getChineseNameWithLunarMonth: (NSInteger) lunarMonth;

- (NSString* ) getChineseNameWithLunarDay: (NSInteger) lunarDay;

- (NSInteger) getLeapDaysWithLunarYear: (NSInteger) lunarYear;

- (NSInteger) getLunarDaysWithLunarYear: (NSInteger) lunarYear
                                  month: (NSInteger) lunarMonth;
@end


@implementation TTLunarCalendar

+ (TTLunarDate*) convertFromLunarDateYear: (NSInteger) year
                                    month: (NSInteger) month
                                      day: (NSInteger) day
                                   isLeap: (BOOL) isLeap
{
    return [[TTLunarCalendarCore sharedCore] convertFromLunarDateYear:year
                                                                month:month
                                                                  day:day
                                                               isLeap:isLeap];
}

+ (TTLunarDate*) convertFromGeneralDate: (NSDate* ) date
{
    NSDateComponents* components = [[NSCalendar currentCalendar] components:NSCalendarUnitDay | NSCalendarUnitMonth | NSCalendarUnitYear
                                                                   fromDate: date];
    return [self convertFromGeneralDateYear:[components year]
                                      month:[components month]
                                        day:[components day]];
}

+ (TTLunarDate*) convertFromGeneralDateYear: (NSInteger) year
                                      month: (NSInteger) month
                                        day: (NSInteger) day
{
    return [[TTLunarCalendarCore sharedCore] convertFromGeneralDateYear:year
                                                                  month:month
                                                                    day:day];
}

+ (NSArray* ) lunarMonthListWithYear: (NSInteger) year
{
    NSInteger leapMonth = [[TTLunarCalendarCore sharedCore] leapMonthWithLunarYear: year];
    
    NSMutableArray* result = [NSMutableArray array];
    
    for (NSInteger i=0; i<12; i++) {
        [result addObject:[[TTLunarCalendarCore sharedCore] getChineseNameWithLunarMonth:i+1]];
    }
    
    if (leapMonth) {
        [result insertObject: [NSString stringWithFormat:@"闰%@", result[leapMonth-1]]
                     atIndex:leapMonth];
        }
    
    return result;
}

+ (NSArray* ) lunarDayListWithYear: (NSInteger) year
                             month: (NSInteger) month
                         leapMonth: (BOOL) leapMonth
{
    NSMutableArray* arr = [NSMutableArray array];
    NSInteger cnt = 0;
    if (leapMonth)
    {
        cnt = [[TTLunarCalendarCore sharedCore] getLeapDaysWithLunarYear:year];
    }
    else
    {
        cnt = [[TTLunarCalendarCore sharedCore] getLunarDaysWithLunarYear:year
                                                                    month:month];
    }
    
    for (int i=0; i<cnt; i++) {
        [arr addObject:[[TTLunarCalendarCore sharedCore] getChineseNameWithLunarDay: i + 1]];
    }
    
    return arr;
}

@end

@implementation TTLunarCalendarCore

+ (TTLunarCalendarCore* ) sharedCore
{
    static TTLunarCalendarCore* _sharedCore = nil;
    
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _sharedCore = [TTLunarCalendarCore new];
    });
    
    return _sharedCore;
}

- (instancetype) init
{
    if (self = [super init])
    {
        NSString* path = [[NSBundle mainBundle] pathForResource:@"calendar" ofType:@"js"];
        NSString* jsStr = [NSString stringWithContentsOfFile:path
                                                    encoding:NSUTF8StringEncoding
                                                       error:nil];
        
        JSContext* context = [JSContext new];
        [context evaluateScript: jsStr];
        
        self.jsContext = context;
    }
    
    return self;
}

/*
 Animal: "马"IDayCn: "十四"IMonthCn: "正月"Term: nullcDay: 13cMonth: 2cYear: 2014gzDay: "乙卯"gzMonth: "丙寅"gzYear: "甲午"isLeap: falseisTerm: falseisToday: falselDay: 14lMonth: 1lYear: 2014nWeek: 4ncWeek: "星期四"__proto__: Object
 */
- (TTLunarDate*) convertFromLunarDateYear: (NSInteger) year
                                    month: (NSInteger) month
                                      day: (NSInteger) day
                                   isLeap: (BOOL) isLeap
{
    JSValue* value = self.jsContext[@"calendar"][@"lunar2solar"];
    JSValue* v = [value callWithArguments:@[@(year), @(month), @(day), @(isLeap)]];
    return [[TTLunarDate alloc] initWithJSValue: v];
}

- (TTLunarDate*) convertFromGeneralDateYear: (NSInteger) year
                                      month: (NSInteger) month
                                        day: (NSInteger) day
{
    JSValue* value = self.jsContext[@"calendar"][@"solar2lunar"];
    JSValue* v = [value callWithArguments:@[@(year), @(month), @(day)]];
    return [[TTLunarDate alloc] initWithJSValue: v];
}

- (NSInteger) leapMonthWithLunarYear: (NSInteger) lunarYear
{
    JSValue* value = self.jsContext[@"calendar"][@"leapMonth"];
    JSValue* v = [value callWithArguments:@[@(lunarYear)]];
    return [v toInt32];
}

- (NSString* ) getChineseNameWithLunarMonth: (NSInteger) lunarMonth
{
    JSValue* value = self.jsContext[@"calendar"][@"toChinaMonth"];
    JSValue* v = [value callWithArguments:@[@(lunarMonth)]];
    return [v toString];
}

- (NSString* ) getChineseNameWithLunarDay: (NSInteger) lunarDay
{
    JSValue* value = self.jsContext[@"calendar"][@"toChinaDay"];
    JSValue* v = [value callWithArguments:@[@(lunarDay)]];
    return [v toString];
}

- (NSInteger) getLeapDaysWithLunarYear: (NSInteger) lunarYear
{
    JSValue* value = self.jsContext[@"calendar"][@"leapDays"];
    JSValue* v = [value callWithArguments:@[@(lunarYear)]];
    return [v toInt32];
}

- (NSInteger) getLunarDaysWithLunarYear: (NSInteger) lunarYear
                                  month: (NSInteger) lunarMonth
{
    JSValue* value = self.jsContext[@"calendar"][@"monthDays"];
    JSValue* v = [value callWithArguments:@[@(lunarYear), @(lunarMonth)]];
    return [v toInt32];
}

@end

@implementation TTLunarDate

- (instancetype) initWithJSValue: (JSValue*) value
{
    if (self = [super init])
    {
        self.animal = [value[@"Animal"] toString];
        self.lunarDayInChinese = [value[@"IDayCn"] toString];
        self.lunarMonthInChinese = [value[@"IMonthCn"] toString];
        
        self.lunarDay = [value[@"lDay"] toInt32];
        self.lunarYear = [value[@"lYear"] toInt32];
        self.lunarMonth = [value[@"lMonth"] toInt32];
        
        self.solarDay = [value[@"cDay"] toInt32];
        self.solarYear = [value[@"cYear"] toInt32];
        self.solarMonth = [value[@"cMonth"] toInt32];
        
        self.ganzhiDay = [value[@"gzDay"] toString];
        self.ganzhiYear = [value[@"gzYear"] toString];
        self.ganzhiMonth = [value[@"gzMonth"] toString];
        
        self.isLeap = [value[@"isLeap"] toBool];
        self.isTerm = [value[@"isTerm"] toBool];
    }
    
    return self;
}

@end