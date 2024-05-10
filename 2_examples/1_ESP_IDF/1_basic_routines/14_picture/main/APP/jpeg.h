/**
 ****************************************************************************************************
 * @file        jpeg.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2023-12-01
 * @brief       图片解码-jpeg/jpg解码 代码
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

#ifndef __JPEG_H
#define __JPEG_H

#include <string.h>
#include <unistd.h>
#include "esp_system.h"
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "ff.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "piclib.h"
#include "pngle.h"
#include "tjpgd.h"


/* rgb565格式 */
typedef uint16_t pixel_jpeg;

/* 函数声明 */
esp_err_t decode_jpeg(pixel_jpeg ***pixels, char * file, int screenWidth, int screenHeight, int * imageWidth, int * imageHeight);
esp_err_t release_image(pixel_jpeg ***pixels, int screenWidth, int screenHeight);
TickType_t jpeg_decode(const char *filename, int width, int height); /* BMP解码 */

#endif
