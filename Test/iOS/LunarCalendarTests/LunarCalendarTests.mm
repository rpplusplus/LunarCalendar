//
//  JSCoretestTests.m
//  JSCoretestTests
//
//  Created by Xiaoxuan Tang on 2/12/15.
//  Copyright (c) 2015 liwushuo. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <XCTest/XCTest.h>
#import <JavaScriptCore/JavaScriptCore.h>
#import "lunar.h"
#import "TTLunarCalendar.h"

@interface LunarCalendarTests : XCTestCase

@property (nonatomic, strong) JSContext* context;
@property (nonatomic, assign) Lunar*     lunar;

@end

@implementation LunarCalendarTests

- (void)setUp {
    [super setUp];

    NSString* path = [[NSBundle mainBundle] pathForResource:@"calendar" ofType:@"js"];
    NSString* jsStr = [NSString stringWithContentsOfFile:path
                                                encoding:NSUTF8StringEncoding
                                                   error:nil];
    
    JSContext* context = [JSContext new];
    [context evaluateScript: jsStr];
    
    self.context = context;
    self.lunar = new Lunar();
}

- (void)tearDown {
    [super tearDown];
    delete self.lunar;
}

#ifdef DEBUG

- (void) testlYearDays
{
    JSValue* value =  self.context[@"calendar"][@"lYearDays"];
    
    for (int i=1901; i<2100; i++) {
        XCTAssertEqual(self.lunar->lYearDays(i), [[value callWithArguments: @[@(i)]] toInt32]);
    }
}

- (void) testLeapMonth
{
    JSValue* value =  self.context[@"calendar"][@"leapMonth"];
    
    for (int i=1901; i<2100; i++) {
        XCTAssertEqual(self.lunar->leapMonth(i), [[value callWithArguments: @[@(i)]] toInt32]);
    }
}

- (void) testLeapDays
{
    JSValue* value =  self.context[@"calendar"][@"leapDays"];
    
    for (int i=1901; i<2100; i++) {
        XCTAssertEqual(self.lunar->leapDays(i), [[value callWithArguments: @[@(i)]] toInt32]);
    }
}

- (void) testMonthDays
{
    JSValue* value = self.context[@"calendar"][@"monthDays"];
    for (int i=1901; i<2100; i++) {
        for (int j=1; j<13; j++) {
            int t = [[value callWithArguments: @[@(i), @(j)]] toInt32];
            XCTAssertEqual(self.lunar->monthDays(i, j), t);
        }
    }
}

- (void) testSolarDays
{
    JSValue* value = self.context[@"calendar"][@"solarDays"];
    for (int i=1901; i<2100; i++) {
        for (int j=1; j<13; j++) {
            int t = [[value callWithArguments: @[@(i), @(j)]] toInt32];
            XCTAssertEqual(self.lunar->solarDays(i, j), t);
        }
    }
}

- (void) testToGanZhi
{
    JSValue* value = self.context[@"calendar"][@"toGanZhi"];
    for (int i=0; i<1000; i++) {
        int t = rand();

        NSString* str1 = [NSString stringWithUTF8String:self.lunar->toGanZhi(t).c_str()];
        NSString* str2 = [[value callWithArguments: @[@(t)]] toString];
        
        XCTAssertEqualObjects(str1, str2);
    }
}

- (void) testGetTerm
{
    JSValue* value = self.context[@"calendar"][@"getTerm"];
    for (int i=1901; i<2100; i++) {
        for (int j=1; j<25; j++) {
            int t = [[value callWithArguments: @[@(i), @(j)]] toInt32];
            XCTAssertEqual(self.lunar->getTerm(i, j), t);
        }
    }
}

- (void) testToChinaMonth
{
    JSValue* value =  self.context[@"calendar"][@"toChinaMonth"];
    
    for (int i=1; i<13; i++) {
        NSString* str1 = [NSString stringWithUTF8String:self.lunar->toChinaMonth(i).c_str()];
        NSString* str2 = [[value callWithArguments: @[@(i)]] toString];
        
        XCTAssertEqualObjects(str1, str2);
    }
}

- (void) testToChinaDay
{
    JSValue* value =  self.context[@"calendar"][@"toChinaDay"];
    
    for (int i=1; i<31; i++) {
        NSString* str1 = [NSString stringWithUTF8String:self.lunar->toChinaDay(i).c_str()];
        NSString* str2 = [[value callWithArguments: @[@(i)]] toString];
        
        XCTAssertEqualObjects(str1, str2);
    }
}

- (void) testGetAnimal
{
    JSValue* value =  self.context[@"calendar"][@"getAnimal"];
    
    for (int i=1901; i<2100; i++) {
        NSString* str1 = [NSString stringWithUTF8String:self.lunar->getAnimal(i).c_str()];
        NSString* str2 = [[value callWithArguments: @[@(i)]] toString];
        
        XCTAssertEqualObjects(str1, str2);
    }
}

- (TTLunarDate*) lunarDateWithJSValue: (JSValue*) value
{
    TTLunarDate* lunarDate = [TTLunarDate new];
    
    lunarDate.animal = [value[@"Animal"] toString];
    lunarDate.lunarDayInChinese = [value[@"IDayCn"] toString];
    lunarDate.lunarMonthInChinese = [value[@"IMonthCn"] toString];
    
    lunarDate.lunarDay = [value[@"lDay"] toInt32];
    lunarDate.lunarYear = [value[@"lYear"] toInt32];
    lunarDate.lunarMonth = [value[@"lMonth"] toInt32];
    
    lunarDate.solarDay = [value[@"cDay"] toInt32];
    lunarDate.solarYear = [value[@"cYear"] toInt32];
    lunarDate.solarMonth = [value[@"cMonth"] toInt32];
    
    lunarDate.ganzhiDay = [value[@"gzDay"] toString];
    lunarDate.ganzhiYear = [value[@"gzYear"] toString];
    lunarDate.ganzhiMonth = [value[@"gzMonth"] toString];
    
    lunarDate.isLeap = [value[@"isLeap"] toBool];
    lunarDate.isTerm = [value[@"isTerm"] toBool];
    
    return lunarDate;
}


- (void) testSolarToLunar
{
    JSValue* value =  self.context[@"calendar"][@"solar2lunar"];
    
    for (int i=1901; i<2100; i++) {
        for (int j=1; j<13; j++)
            for (int k=1; k<=self.lunar->solarDays(i, j); k++)
            {
                LunarObj* obj = self.lunar->solar2lunar(i, j, k);
                JSValue* v = [value callWithArguments: @[@(i), @(j), @(k)]];
                
                TTLunarDate* date = [self lunarDateWithJSValue:v];
                
                XCTAssertEqual(obj->solarDay, date.solarDay);
                XCTAssertEqual(obj->solarYear, date.solarYear);
                XCTAssertEqual(obj->solarMonth, date.solarMonth);
                
                XCTAssertEqual(obj->lunarDay, date.lunarDay);
                XCTAssertEqual(obj->lunarYear, date.lunarYear);
                XCTAssertEqual(obj->lunarMonth, date.lunarMonth);
                
                XCTAssertEqual(obj->isLeap, date.isLeap);
                XCTAssertEqual(obj->isTerm, date.isTerm);
                
                XCTAssertEqualObjects([NSString stringWithUTF8String:obj->animal.c_str()], date.animal);
                XCTAssertEqualObjects([NSString stringWithUTF8String:obj->lunarDayChineseName.c_str()], date.lunarDayInChinese);
                XCTAssertEqualObjects([NSString stringWithUTF8String:obj->lunarMonthChineseName.c_str()], date.lunarMonthInChinese);
                
                XCTAssertEqualObjects([NSString stringWithUTF8String:obj->ganzhiDay.c_str()], date.ganzhiDay);
                XCTAssertEqualObjects([NSString stringWithUTF8String:obj->ganzhiYear.c_str()], date.ganzhiYear);
                XCTAssertEqualObjects([NSString stringWithUTF8String:obj->ganzhiMonth.c_str()], date.ganzhiMonth);

                
                LunarObj* obj2 = self.lunar->lunar2solar(obj->lunarYear, obj->lunarMonth, obj->lunarDay, obj->isLeap);
                
                XCTAssertEqual(obj2->solarDay, k);
                XCTAssertEqual(obj2->solarYear, i);
                XCTAssertEqual(obj2->solarMonth, j);
                
                delete obj;
                delete obj2;
            }
    }
}

- (void) compareCpp: (LunarObj*) obj
                 js: (JSValue*) v
{
    TTLunarDate* date = [self lunarDateWithJSValue:v];
    
    XCTAssertEqual(obj->solarDay, date.solarDay);
    XCTAssertEqual(obj->solarYear, date.solarYear);
    XCTAssertEqual(obj->solarMonth, date.solarMonth);
    
    XCTAssertEqual(obj->lunarDay, date.lunarDay);
    XCTAssertEqual(obj->lunarYear, date.lunarYear);
    XCTAssertEqual(obj->lunarMonth, date.lunarMonth);
    
    XCTAssertEqual(obj->isLeap, date.isLeap);
    XCTAssertEqual(obj->isTerm, date.isTerm);
    
    XCTAssertEqualObjects([NSString stringWithUTF8String:obj->animal.c_str()], date.animal);
    XCTAssertEqualObjects([NSString stringWithUTF8String:obj->lunarDayChineseName.c_str()], date.lunarDayInChinese);
    XCTAssertEqualObjects([NSString stringWithUTF8String:obj->lunarMonthChineseName.c_str()], date.lunarMonthInChinese);
    
    XCTAssertEqualObjects([NSString stringWithUTF8String:obj->ganzhiDay.c_str()], date.ganzhiDay);
    XCTAssertEqualObjects([NSString stringWithUTF8String:obj->ganzhiYear.c_str()], date.ganzhiYear);
    XCTAssertEqualObjects([NSString stringWithUTF8String:obj->ganzhiMonth.c_str()], date.ganzhiMonth);
}

- (void) testLunarToSolar
{
    JSValue* value =  self.context[@"calendar"][@"lunar2solar"];
    
    for (int i=1902; i<2100; i++) {
        
        int leapMonth = self.lunar->leapMonth(i);
        
        if (leapMonth)
        {
            for (int k=1; k<=self.lunar->leapDays(i); k++) {
                LunarObj* obj = self.lunar->lunar2solar(i, leapMonth, k, true);
                JSValue* v = [value callWithArguments: @[@(i), @(leapMonth), @(k), @(true)]];
                [self compareCpp:obj js:v];
                delete obj;
            }
        }
        
        for (int j=1; j<13; j++)
            for (int k=1; k<=self.lunar->monthDays(i, j); k++)
            {
                LunarObj* obj = self.lunar->lunar2solar(i, j, k, false);
                JSValue* v = [value callWithArguments: @[@(i), @(j), @(k), @(false)]];
                [self compareCpp:obj js:v];
                delete obj;
            }
    }
}

#endif

@end
