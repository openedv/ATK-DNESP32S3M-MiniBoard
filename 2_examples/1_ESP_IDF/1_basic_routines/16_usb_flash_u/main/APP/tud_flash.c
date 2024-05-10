/**
 ****************************************************************************************************
 * @file        tud_flash.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2023-12-01
 * @brief       FLASH模拟U盘（USB）代码
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

#include "tud_flash.h"


static const char *TAG = "usb_msc";
const char *disk_path = "/disk";                /* 磁盘的路径 */
static uint8_t s_pdrv = 0;                      /* 用于识别驱动器的物理驱动器 */
static int s_disk_block_size = 0;               /* 磁盘块的大小 */
#define LOGICAL_DISK_NUM        1               /* 磁盘个数 */
static bool ejected[LOGICAL_DISK_NUM] = {true}; /* 弹出状态 */
__usbdev g_usbdev;                              /* USB控制器 */
//--------------------------------------------------------------------+
// 以下是USB回调函数，一般用来判断连接过程
//--------------------------------------------------------------------+

/**
 * @brief       在安装USB设备时调用此函数
 * @param       无
 * @retval      无
 */
void tud_mount_cb(void)
{
    /* 每次插入USB时重置及跟踪。这样可以获取插入，弹出状态，有利于重新插上获得驱动器 */
    for (uint8_t i = 0; i < LOGICAL_DISK_NUM; i++)
    {
        ejected[i] = false;
    }

    g_usbdev.status |= 0x01;

    ESP_LOGI(__func__, "");
}

/**
 * @brief       USB设备卸载时调用此函数
 * @param       无
 * @retval      无
 */
void tud_umount_cb(void)
{
    ESP_LOGW(__func__, "");
}

/**
 * @brief       USB总线挂起时调用此函数
 * @param       无
 * @retval      无
 */
void tud_suspend_cb(bool remote_wakeup_en)
{
    g_usbdev.status &= 0x00;
    ESP_LOGW(__func__, "");
}

/**
 * @brief       恢复USB总线时调用
 * @param       无
 * @retval      无
 */
void tud_resume_cb(void)
{
    ESP_LOGW(__func__, "");
}

/**
 * @brief       用于刷新任何挂起的缓存
 * @param       无
 * @retval      无
 */
void tud_msc_write10_complete_cb(uint8_t lun)
{
    if (lun >= LOGICAL_DISK_NUM)
    {
        ESP_LOGE(__func__, "invalid lun number %u", lun);
        return;
    }

    /* 此写入完成，启动自动重新加载时钟 */
    ESP_LOGD(__func__, "");
}

/**
 * @brief       已弹出磁盘
 * @param       无
 * @retval      无
 */
static bool _logical_disk_ejected(void)
{
    bool all_ejected = true;

    for (uint8_t i = 0; i < LOGICAL_DISK_NUM; i++)
    {
        all_ejected &= ejected[i];
    }

    return all_ejected;
}

/**
 * @brief       收到SCSI_CMD_INQUIRY时调用此函数，用于从目标设备获取基本信息
 * @param       lun         :磁盘数量
 * @param       vendor_id   :供应商id
 * @param       product_id  :产品id
 * @param       product_rev :修订版
 * @retval      无
 */
void tud_msc_inquiry_cb(uint8_t lun, uint8_t vendor_id[8], uint8_t product_id[16], uint8_t product_rev[4])
{
    ESP_LOGD(__func__, "");

    if (lun >= LOGICAL_DISK_NUM)
    {
        ESP_LOGE(__func__, "invalid lun number %u", lun);
        return;
    }

    const char vid[] = "ESPS3-S3";
    const char pid[] = "Mass Storage";
    const char rev[] = "1.0";

    memcpy(vendor_id, vid, strlen(vid));
    memcpy(product_id, pid, strlen(pid));
    memcpy(product_rev, rev, strlen(rev));
}

/**
 * @brief       收到测试单元就绪命令时调用
 * @param       lun:磁盘数量
 * @retval      true允许主机读取/写入此LUN，例如插入的SD卡
 */
bool tud_msc_test_unit_ready_cb(uint8_t lun)
{
    ESP_LOGD(__func__, "");

    if (lun >= LOGICAL_DISK_NUM)
    {
        ESP_LOGE(__func__, "invalid lun number %u", lun);
        return false;
    }

    if (_logical_disk_ejected())
    {
        /* 为不存在的磁盘设置0x3a */
        tud_msc_set_sense(lun, SCSI_SENSE_NOT_READY, 0x3A, 0x00);
        return false;
    }

    return true;
}

/**
 * @brief       当收到SCSI_CMD_READ_CAPACITY_10和SCSI_CMD _READ_FORMAT_CCAPITATION时调用此函数，以确定磁盘大小
 * @param       lun         :磁盘数量
 * @param       block_count :块数量
 * @param       block_size  :块大小
 * @retval      true允许主机读取/写入此LUN，例如插入的SD卡
 */
void tud_msc_capacity_cb(uint8_t lun, uint32_t *block_count, uint16_t *block_size)
{
    ESP_LOGD(__func__, "");

    if (lun >= LOGICAL_DISK_NUM)
    {
        ESP_LOGE(__func__, "invalid lun number %u", lun);
        return;
    }

    disk_ioctl(s_pdrv, GET_SECTOR_COUNT, block_count);
    disk_ioctl(s_pdrv, GET_SECTOR_SIZE, block_size);
    s_disk_block_size = *block_size;
    ESP_LOGD(__func__, "GET_SECTOR_COUNT = %"PRIu32"，GET_SECTOR_SIZE = %d", *block_count, *block_size);
}

/**
 * @brief       调用以检查设备是否可作为SCSI
 * @param       lun         :磁盘数量
 * @retval      true:可以;false:不可以
 */
bool tud_msc_is_writable_cb(uint8_t lun)
{
    ESP_LOGD(__func__, "");

    if (lun >= LOGICAL_DISK_NUM)
    {
        ESP_LOGE(__func__, "invalid lun number %u", lun);
        return false;
    }

    return true;
}

/**
 * @brief       收到“启动-停止单元”命令时调用
 * @param       lun             :磁盘数量
 * @param       power_condition :电源条件
 * @param       start           :Start = 0：停止电源模式;Start = 1：活动模式
 * @param       load_eject      :Start = 0,load_eject = 1：卸载磁盘存储;Start = 1,load_eject=1：加载磁盘存储
 * @retval      true:加载磁盘存储成功;false:卸载磁盘存储成功
 */
bool tud_msc_start_stop_cb(uint8_t lun, uint8_t power_condition, bool start, bool load_eject)
{
    ESP_LOGI(__func__, "");
    (void) power_condition;

    if (lun >= LOGICAL_DISK_NUM)
    {
        ESP_LOGE(__func__, "invalid lun number %u", lun);
        return false;
    }

    if (load_eject)
    {
        if (!start)
        {
            /* 弹出磁盘 */
            if (disk_ioctl(s_pdrv, CTRL_SYNC, NULL) != RES_OK)
            {
                return false;
            }
            else
            {
                ejected[lun] = true;
            }
        }
        else
        {
            /* 只有在它没有弹出的情况下才能加载 */
            return !ejected[lun];
        }
    }
    else
    {
        if (!start)
        {
            /* 停止装置，但不弹出 */
            if (disk_ioctl(s_pdrv, CTRL_SYNC, NULL) != RES_OK)
            {
                return false;
            }
        }

        /* 始终启动设备，即使弹出 */
    }

    return true;
}

/**
 * @brief       收到READ10命令时调用回调此函数
 * @param       lun     :磁盘数量
 * @param       lba     :块地址
 * @param       offset  :数据偏移
 * @param       buffer  :读取数据的存储区
 * @param       bufsize :读取数据大小
 * @retval      返回读取的字节数
 */
int32_t tud_msc_read10_cb(uint8_t lun, uint32_t lba, uint32_t offset, void *buffer, uint32_t bufsize)
{
    ESP_LOGD(__func__, "");

    if (lun >= LOGICAL_DISK_NUM)
    {
        ESP_LOGE(__func__, "invalid lun number %u", lun);
        return 0;
    }

    const uint32_t block_count = bufsize / s_disk_block_size;
    /* 磁盘读取 */
    disk_read(s_pdrv, buffer, lba, block_count);

    return block_count * s_disk_block_size;
}

/**
 * @brief       收到WRITE10命令时调用回调此函数
 * @param       lun     :磁盘数量
 * @param       lba     :块地址
 * @param       offset  :写入偏移
 * @param       buffer  :写入数据的存储区
 * @param       bufsize :写入数据大小
 * @retval      返回写入的字节数
 */
int32_t tud_msc_write10_cb(uint8_t lun, uint32_t lba, uint32_t offset, uint8_t *buffer, uint32_t bufsize)
{
    ESP_LOGD(__func__, "");
    (void) offset;

    if (lun >= LOGICAL_DISK_NUM)
    {
        ESP_LOGE(__func__, "invalid lun number %u", lun);
        return 0;
    }

    const uint32_t block_count = bufsize / s_disk_block_size;
    /* 磁盘写入 */
    disk_write(s_pdrv, buffer, lba, block_count);

    return block_count * s_disk_block_size;
}

/**
 * @brief       当收到不在下面内置列表中的SCSI命令时调用此函数
 * @param       lun         :磁盘数量
 * @param       scsi_cmd    :scsi命令内容，应用程序必须检查该命令内容才能做出相应响应
 * @param       buffer      :SCSI数据阶段的缓冲区
 * @param       bufsize     :缓冲区的长度
 * @retval      返回写入的字节数
 */
int32_t tud_msc_scsi_cb(uint8_t lun, uint8_t const scsi_cmd[16], void *buffer, uint16_t bufsize)
{
    /* read10&write10有自己的回调，不能在这里处理 */
    ESP_LOGD(__func__, "");

    if (lun >= LOGICAL_DISK_NUM)
    {
        ESP_LOGE(__func__, "invalid lun number %u", lun);
        return 0;
    }

    void const *response = NULL;
    uint16_t resplen = 0;

    /* 处理的大多数scsi都是输入 */
    bool in_xfer = true;

    switch (scsi_cmd[0])
    {
        case SCSI_CMD_PREVENT_ALLOW_MEDIUM_REMOVAL:
            /* 主机即将读/写等。。。最好不要断开磁盘连接 */
            resplen = 0;
            break;

        default:
            /* 设置无效的命令操作 */
            tud_msc_set_sense(lun, SCSI_SENSE_ILLEGAL_REQUEST, 0x20, 0x00);

            /* 否定意味着错误->tinyusb可能会暂停和/或以失败状态响应 */
            resplen = -1;
            break;
    }

    /* resplen不得大于bufsize */
    if (resplen > bufsize)
    {
        /* 如果大于，则resplen = bufsize */
        resplen = bufsize;
    }

    if (response && (resplen > 0))
    {
        if (in_xfer)
        {
            memcpy(buffer, response, resplen);
        }
        else
        {
            /* SCSI output */
        }
    }

    return resplen;
}

//--------------------------------------------------------------------+
// 以上是USB回调函数，一般用来判断连接过程
//--------------------------------------------------------------------+

/**
 * @brief       初始化SPIFFS的函数
 * @param       base_path:定义分区的名称
 * @retval      无
 */
static esp_err_t tud_spiffs_partitions_init(const char *base_path)
{
    ESP_LOGI(TAG, "Mounting FAT filesystem");
    esp_err_t ret = ESP_FAIL;
    /* 如果是新分区并且以前没有格式化，则允许格式化分区 */
    wl_handle_t wl_handle_1 = WL_INVALID_HANDLE;
    ESP_LOGI(TAG, "using internal flash");
    
    const esp_vfs_fat_mount_config_t mount_config = {
        .format_if_mount_failed = true,
        .max_files = 9,
        .allocation_unit_size = CONFIG_WL_SECTOR_SIZE
    };

    /* 判断IDF版本释放大于5.0版本，本教程使用的是5.0版本，所以执行第一句代码 */
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)
    /* 挂在分区 */
    ret = esp_vfs_fat_spiflash_mount_rw_wl(base_path, "storage", &mount_config, &wl_handle_1);
#else
    ret = esp_vfs_fat_spiflash_mount(base_path, "storage", &mount_config, &wl_handle_1);
#endif
    /* 挂在分区失败 */
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to mount FATFS (%s)", esp_err_to_name(ret));
        return ESP_FAIL;
    }

    return ESP_OK;
}

/**
 * @brief       FLASH模拟U盘函数初始化
 * @param       无
 * @retval      无
 */
void tud_usb_flash(void)
{
    /* 初始化SPIFFS分区 */
    ESP_ERROR_CHECK(tud_spiffs_partitions_init(disk_path));
    vTaskDelay(100);

    const tinyusb_config_t tusb_cfg = {0};
    /* USB设备登记 */
    ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));
    ESP_LOGI(TAG, "USB MSC initialization DONE");
}
