//
//  TTLunarCalendar.h
//  JSCoretest
//
//  Created by Xiaoxuan Tang on 2/12/15.
//  Copyright (c) 2015 liwushuo. All rights reserved.
//

#import <Foundation/Foundation.h>

@class TTLunarDate;
@class JSValue;

@interface TTLunarCalendar : NSObject

+ (TTLunarDate*) convertFromLunarDateYear: (NSInteger) year
                                    month: (NSInteger) month
                                      day: (NSInteger) day
                                   isLeap: (BOOL) isLeap;

+ (TTLunarDate*) convertFromGeneralDate: (NSDate* ) date;

+ (TTLunarDate*) convertFromGeneralDateYear: (NSInteger) year
                                      month: (NSInteger) month
                                        day: (NSInteger) day;

+ (NSArray* ) lunarMonthListWithYear: (NSInteger) year;

+ (NSArray* ) lunarDayListWithYear: (NSInteger) year
                             month: (NSInteger) month
                         leapMonth: (BOOL) leapMonth;


@end

@interface TTLunarDate : NSObject

@property (nonatomic, assign) NSInteger lunarDay;
@property (nonatomic, assign) NSInteger lunarYear;
@property (nonatomic, assign) NSInteger lunarMonth;

@property (nonatomic, assign) NSInteger solarDay;
@property (nonatomic, assign) NSInteger solarYear;
@property (nonatomic, assign) NSInteger solarMonth;

@property (nonatomic, strong) NSString* ganzhiDay;
@property (nonatomic, strong) NSString* ganzhiYear;
@property (nonatomic, strong) NSString* ganzhiMonth;

@property (nonatomic, strong) NSString* animal;
@property (nonatomic, strong) NSString* lunarDayInChinese;
@property (nonatomic, strong) NSString* lunarMonthInChinese;

@property (nonatomic, assign) BOOL isLeap;
@property (nonatomic, assign) BOOL isTerm;

- (instancetype) initWithJSValue: (JSValue*) value;

@end