/**
 ****************************************************************************************************
 * @file        tud_sd.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2023-12-01
 * @brief       SD卡模拟U盘（USB）代码
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

#ifndef __TUD_SD_H
#define __TUD_SD_H

#include <inttypes.h>
#include "ff.h"
#include "diskio.h"
#include "esp_vfs_fat.h"
#include "tinyusb.h"
#include "esp_idf_version.h"


/* USB控制器 */
typedef struct
{
    uint8_t status;                         /* bit0:0,断开;1,连接 */
}__usbdev;

extern __usbdev g_usbdev;                   /* USB控制器 */

/* 函数声明 */
void tud_usb_sd(void);

#endif
