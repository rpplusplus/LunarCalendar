# LunarCalendar
一个由 JS 万年历移植而来的，兼容 iOS / Android 的 cpp 农历模块。

**对于 iOS，还提供一个用 JSCore 直接调用 Calendar.js 的版本**

## 功能

- 支持 农历 转 公历
- 支持 公历 转 农历
- 支持 天干、地支 数据
- 支持 24 节气
- 支持 农历日期的中文显示

## 使用

### C++

```
lunar = new Lunar();
LunarObj* obj = lunar->solar2lunar(2015, 3, 4);

obj >>>
lunarYear: 2015, 
lunarMonth: 1, 
lunarDay: 14, 
Animal: "羊"
lunarMonthChineseName: "正月"
lunarDayChineseName: "十四"
solarDay: 4
solarMonth: 3
solarYear: 2015
gzDay: "己卯"
gzMonth: "戊寅"
gzYear: "乙未"
isLeap: false
isTerm: false
```
其他方法请参考头文件注释、和 iOS 单元测试

### Objective-C

```
TTLunarDate* lunarDate = [TTLunarCalendar convertFromGeneralDateYear: 2015
                                                               month: 3
                                                                 day: 4];
                                                                 
lunarDate Property 同 cpp
```

其他方法请参考头文件注释、和 iOS 单元测试，JSCore 版本对外接口一样。

### Android

Todo

## Todo
- CocoaPod 支持
- NDK 封装