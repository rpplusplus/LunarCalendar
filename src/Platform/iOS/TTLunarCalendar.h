//
//  TTLunarCalendar.h
//  JSCoretest
//
//  Created by Xiaoxuan Tang on 2/12/15.
//  Copyright (c) 2015 liwushuo. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "lunar.h"
@class TTLunarDate;

@interface TTLunarCalendar : NSObject

+ (TTLunarDate*) convertFromLunarDateYear: (int32_t) year
                                    month: (int32_t) month
                                      day: (int32_t) day
                                   isLeap: (BOOL) isLeap;

+ (TTLunarDate*) convertFromGeneralDate: (NSDate* ) date;

+ (TTLunarDate*) convertFromGeneralDateYear: (int32_t) year
                                      month: (int32_t) month
                                        day: (int32_t) day;

+ (NSArray* ) lunarMonthListWithYear: (int32_t) year;

+ (NSArray* ) lunarDayListWithYear: (int32_t) year
                             month: (int32_t) month
                         leapMonth: (BOOL) leapMonth;


@end

@interface TTLunarDate : NSObject

@property (nonatomic, assign) int32_t lunarDay;
@property (nonatomic, assign) int32_t lunarYear;
@property (nonatomic, assign) int32_t lunarMonth;

@property (nonatomic, assign) int32_t solarDay;
@property (nonatomic, assign) int32_t solarYear;
@property (nonatomic, assign) int32_t solarMonth;

@property (nonatomic, strong) NSString* ganzhiDay;
@property (nonatomic, strong) NSString* ganzhiYear;
@property (nonatomic, strong) NSString* ganzhiMonth;

@property (nonatomic, strong) NSString* animal;
@property (nonatomic, strong) NSString* lunarDayInChinese;
@property (nonatomic, strong) NSString* lunarMonthInChinese;

@property (nonatomic, assign) BOOL isLeap;
@property (nonatomic, assign) BOOL isTerm;

- (instancetype) initWithLunarObj: (LunarObj*) obj;

@end