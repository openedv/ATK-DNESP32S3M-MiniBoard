/**
 ****************************************************************************************************
 * @file        rtc.h
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

#ifndef __ESP_RTC_H
#define __ESP_RTC_H

#include "driver/spi_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <sys/time.h>


/* 时间结构体, 包括年月日周时分秒等信息 */
typedef struct
{
    uint8_t hour;       /* 时 */
    uint8_t min;        /* 分 */
    uint8_t sec;        /* 秒 */
    /* 公历年月日周 */
    uint16_t year;      /* 年 */
    uint8_t  month;     /* 月 */
    uint8_t  date;      /* 日 */
    uint8_t  week;      /* 周 */
} _calendar_obj;

extern _calendar_obj calendar;      /* 时间结构体 */

/* 函数声明 */
void rtc_set_time(int year,int mon,int mday,int hour,int min,int sec);  /* 设置时间 */
void rtc_get_time(void);                                                /* 获取时间 */
uint8_t rtc_get_week(uint16_t year, uint8_t month, uint8_t day);        /* 获取周几 */

#endif
