//
//  TTLunarCalendar.m
//  JSCoretest
//
//  Created by Xiaoxuan Tang on 2/12/15.
//  Copyright (c) 2015 liwushuo. All rights reserved.
//

#import "TTLunarCalendar.h"

@interface TTLunarCalendarCore : NSObject

@property (nonatomic, assign) Lunar* lunar;

+ (TTLunarCalendarCore* ) sharedCore;

- (TTLunarDate*) convertFromGeneralDateYear: (int32_t) year
                                      month: (int32_t) month
                                        day: (int32_t) day;

- (TTLunarDate*) convertFromLunarDateYear: (int32_t) year
                                    month: (int32_t) month
                                      day: (int32_t) day
                                   isLeap: (BOOL) isLeap;;

- (int32_t) leapMonthWithLunarYear: (int32_t) lunarYear;

- (NSString* ) getChineseNameWithLunarMonth: (int32_t) lunarMonth;

- (NSString* ) getChineseNameWithLunarDay: (int32_t) lunarDay;

- (int32_t) getLeapDaysWithLunarYear: (int32_t) lunarYear;

- (int32_t) getLunarDaysWithLunarYear: (int32_t) lunarYear
                                  month: (int32_t) lunarMonth;
@end


@implementation TTLunarCalendar

+ (TTLunarDate*) convertFromLunarDateYear: (int32_t) year
                                    month: (int32_t) month
                                      day: (int32_t) day
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
    return [self convertFromGeneralDateYear:(int32_t) [components year]
                                      month:(int32_t) [components month]
                                        day:(int32_t) [components day]];
}

+ (TTLunarDate*) convertFromGeneralDateYear: (int32_t) year
                                      month: (int32_t) month
                                        day: (int32_t) day
{
    return [[TTLunarCalendarCore sharedCore] convertFromGeneralDateYear:year
                                                                  month:month
                                                                    day:day];
}

+ (NSArray* ) lunarMonthListWithYear: (int32_t) year
{
    int32_t leapMonth = [[TTLunarCalendarCore sharedCore] leapMonthWithLunarYear: year];
    
    NSMutableArray* result = [NSMutableArray array];
    
    for (int32_t i=0; i<12; i++) {
        [result addObject:[[TTLunarCalendarCore sharedCore] getChineseNameWithLunarMonth:i+1]];
    }
    
    if (leapMonth) {
        [result insertObject: [NSString stringWithFormat:@"闰%@", result[leapMonth-1]]
                     atIndex:leapMonth];
        }
    
    return result;
}

+ (NSArray* ) lunarDayListWithYear: (int32_t) year
                             month: (int32_t) month
                         leapMonth: (BOOL) leapMonth
{
    NSMutableArray* arr = [NSMutableArray array];
    int32_t cnt = 0;
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
        self.lunar = new Lunar();
    }
    
    return self;
}

- (void) dealloc
{
    delete self.lunar;
    self.lunar = NULL;
}

- (TTLunarDate*) convertFromLunarDateYear: (int32_t) year
                                    month: (int32_t) month
                                      day: (int32_t) day
                                   isLeap: (BOOL) isLeap
{
    
    LunarObj* obj = self.lunar->lunar2solar(year, month, day, isLeap);
    TTLunarDate* result = [[TTLunarDate alloc] initWithLunarObj:obj];
    delete obj;
    return result;
}

- (TTLunarDate*) convertFromGeneralDateYear: (int32_t) year
                                      month: (int32_t) month
                                        day: (int32_t) day
{
    LunarObj* obj = self.lunar->solar2lunar(year, month, day);
    TTLunarDate* result = [[TTLunarDate alloc] initWithLunarObj:obj];
    delete obj;
    return result;
}

- (int32_t) leapMonthWithLunarYear: (int32_t) lunarYear
{
    return self.lunar->leapMonth( lunarYear );
}

- (NSString* ) getChineseNameWithLunarMonth: (int32_t) lunarMonth
{
    return [NSString stringWithUTF8String: self.lunar->toChinaMonth( lunarMonth ).c_str() ];
}

- (NSString* ) getChineseNameWithLunarDay: (int32_t) lunarDay
{
    return [NSString stringWithUTF8String: self.lunar->toChinaDay( lunarDay ).c_str() ];
}

- (int32_t) getLeapDaysWithLunarYear: (int32_t) lunarYear
{
    return self.lunar->leapDays( lunarYear );
}

- (int32_t) getLunarDaysWithLunarYear: (int32_t) lunarYear
                                  month: (int32_t) lunarMonth
{
    return self.lunar->monthDays(lunarYear, lunarMonth);
}

@end

@implementation TTLunarDate

- (instancetype) initWithLunarObj: (LunarObj*) obj
{
    if (self = [super init])
    {
        self.animal = [NSString stringWithUTF8String:obj->animal.c_str()];
        self.lunarDayInChinese = [NSString stringWithUTF8String:obj->lunarDayChineseName.c_str()];
        self.lunarMonthInChinese = [NSString stringWithUTF8String:obj->lunarMonthChineseName.c_str()];
        
        self.lunarDay = obj->lunarDay;
        self.lunarYear = obj->lunarYear;
        self.lunarMonth = obj->lunarMonth;
        
        self.solarDay = obj->solarDay;
        self.solarYear = obj->solarYear;
        self.solarMonth = obj->solarMonth;
        
        self.ganzhiDay = [NSString stringWithUTF8String: obj->ganzhiDay.c_str()];
        self.ganzhiYear = [NSString stringWithUTF8String: obj->ganzhiYear.c_str()];
        self.ganzhiMonth =[NSString stringWithUTF8String: obj->ganzhiMonth.c_str()];
        
        self.isLeap = obj->isLeap;
        self.isTerm = obj->isTerm;
    }
    
    return self;
}

@end