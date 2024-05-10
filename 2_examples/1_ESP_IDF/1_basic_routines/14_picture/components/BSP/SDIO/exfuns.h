/**
 ****************************************************************************************************
 * @file        exfuns.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2023-12-01
 * @brief       FATFS 扩展代码
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

#ifndef __EXFUNS_H
#define __EXFUNS_H

#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "ff.h"


extern FATFS *fs[FF_VOLUMES];
extern FIL *file;
extern FIL *ftemp;
extern UINT br, bw;
extern FILINFO fileinfo;
extern FF_DIR dir;
extern uint8_t *fatbuf;     /* SD卡数据缓存区 */


/* exfuns_file_type返回的类型定义
 * 根据表FILE_TYPE_TBL获得.在exfuns.c里面定义
 */
#define T_BMP       0X01    /* BMP文件 */
#define T_JPG       0X02    /* jpg文件 */
#define T_JPEG      0X03    /* jpeg文件 */
#define T_GIF       0X04    /* gif文件 */
#define T_PNG       0X05    /* PNG文件 */

uint8_t exfuns_init(void);                              /* 申请内存 */
uint8_t exfuns_file_type(char *fname);                  /* 识别文件类型 */
uint8_t exfuns_get_free(uint8_t *pdrv, uint32_t *total, uint32_t *free);    /* 得到磁盘总容量和剩余容量 */
uint32_t exfuns_get_folder_size(uint8_t *fdname);       /* 得到文件夹大小 */
uint8_t *exfuns_get_src_dname(uint8_t *dpfn);
uint8_t exfuns_file_copy(uint8_t(*fcpymsg)(uint8_t *pname, uint8_t pct, uint8_t mode), uint8_t *psrc, uint8_t *pdst, uint32_t totsize, uint32_t cpdsize, uint8_t fwmode);       /* 文件复制 */
uint8_t exfuns_folder_copy(uint8_t(*fcpymsg)(uint8_t *pname, uint8_t pct, uint8_t mode), uint8_t *psrc, uint8_t *pdst, uint32_t *totsize, uint32_t *cpdsize, uint8_t fwmode);   /* 文件夹复制 */

#endif
























