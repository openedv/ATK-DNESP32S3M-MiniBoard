/**
 ****************************************************************************************************
 * @file        rtc.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2023-08-26
 * @brief       RTC驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 ESP32-S3 最小系统板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "esp_rtc.h"


_calendar_obj calendar;         /* 时间结构体 */

/**
 * @brief       RTC设置时间
 * @param       year    :年
 * @param       mon     :月
 * @param       mday    :日
 * @param       hour    :时
 * @param       min     :分
 * @param       sec     :秒
 * @retval      无
 */
void rtc_set_time(int year,int mon,int mday,int hour,int min,int sec)
{
    struct tm datetime;
    /* 设置时间 */
    datetime.tm_year = year - 1900;
    datetime.tm_mon = mon - 1;
    datetime.tm_mday = mday;
    datetime.tm_hour = hour;
    datetime.tm_min = min;
    datetime.tm_sec = sec;
    datetime.tm_isdst = -1;
    /* 获取1970.1.1以来的总秒数 */
    time_t second = mktime(&datetime);
    struct timeval val = { .tv_sec = second, .tv_usec = 0 };
    /* 设置当前时间 */
    settimeofday(&val, NULL);
}

/**
 * @brief       获取当前的时间
 * @param       无
 * @retval      无
 */
void rtc_get_time(void)
{
    struct tm *datetime;
    time_t second;
    /* 返回自(1970.1.1 00:00:00 UTC)经过的时间(秒) */
    time(&second);
    datetime = localtime(&second);

    calendar.hour = datetime->tm_hour;          /* 时 */
    calendar.min = datetime->tm_min;            /* 分 */
    calendar.sec = datetime->tm_sec;            /* 秒 */
    /* 公历年月日周 */
    calendar.year = datetime->tm_year + 1900;   /* 年 */
    calendar.month = datetime->tm_mon + 1;      /* 月 */
    calendar.date = datetime->tm_mday;          /* 日 */
    /* 周 */
    calendar.week = rtc_get_week(calendar.year, calendar.month, calendar.date);
}

/**
 * @brief       将年月日时分秒转换成秒钟数
 *   @note      输入公历日期得到星期(起始时间为: 公元0年3月1日开始, 输入往后的任何日期, 都可以获取正确的星期)
 *              使用 基姆拉尔森计算公式 计算, 原理说明见此贴:
 *              https://www.cnblogs.com/fengbohello/p/3264300.html
 * @param       syear : 年份
 * @param       smon  : 月份
 * @param       sday  : 日期
 * @retval      0, 星期天; 1 ~ 6: 星期一 ~ 星期六
 */
uint8_t rtc_get_week(uint16_t year, uint8_t month, uint8_t day)
{
    uint8_t week = 0;

    if (month < 3)
    {
        month += 12;
        --year;
    }

    week = (day + 1 + 2 * month + 3 * (month + 1) / 5 + year + (year >> 2) - year / 100 + year / 400) % 7;
    return week;
}

