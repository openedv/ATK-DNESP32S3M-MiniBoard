/**
 ****************************************************************************************************
 * @file        png.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2023-12-01
 * @brief       图片解码-png解码 代码
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

#ifndef __PNG_H
#define __PNG_H

#include <stdint.h>
#include <stdbool.h>
#include "pngle.h"
#include "piclib.h"
#include "ff.h"
#include "esp_log.h"


/* 函数声明 */
void png_init(pngle_t *pngle, uint32_t w, uint32_t h);
void png_draw(pngle_t *pngle, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t rgba[4]);
void png_finish(pngle_t *pngle);
TickType_t png_decode(const char *filename, int width, int height);

#endif
