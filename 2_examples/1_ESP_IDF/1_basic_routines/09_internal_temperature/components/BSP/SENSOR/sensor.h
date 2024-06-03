/**
 ****************************************************************************************************
 * @file        sensor.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2023-08-26
 * @brief       内部温度传感器驱动代码
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

#ifndef __SENSOR_H_
#define __SENSOR_H_

#include "esp_err.h"
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/temperature_sensor.h"


/* 参数定义 */
#define SENSOR_RANGE_MIN    20      /* 要测试温度的最小值 */
#define SENSOR_RANGE_MAX    50      /* 要测试温度的最大值 */

/* 函数声明 */
void temperature_sensor_init(void); /* 初始化内部温度传感器 */
short sensor_get_temperature(void); /* 获取内部温度传感器温度值 */

#endif