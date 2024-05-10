/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2023-08-26
 * @brief       SPIFFS实验
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "led.h"
#include "spi.h"
#include "lcd.h"
#include "esp_spiffs.h"
#include "esp_vfs.h"
#include "nvs_flash.h"


#define DEFAULT_FD_NUM          5
#define DEFAULT_MOUNT_POINT     "/spiffs"
#define WRITE_DATA              "ESP32-S3\r\n"
static const char               *TAG = "spiffs";

/**
 * @brief       spiffs初始化
 * @param       partition_label:分区表的分区名称
 * @param       mount_point:文件系统关联的文件路径前缀
 * @param       max_files:可以同时打开的最大文件数
 * @retval      无
 */
esp_err_t spiffs_init(char *partition_label, char *mount_point, size_t max_files)
{
    /* 配置spiffs文件系统各个参数 */
    esp_vfs_spiffs_conf_t conf = {
        .base_path = mount_point,
        .partition_label = partition_label,
        .max_files = max_files,
        .format_if_mount_failed = true,
    };

    /* 使用上面定义的设置来初始化和挂载SPIFFS文件系统 */
    esp_err_t ret_val = esp_vfs_spiffs_register(&conf);

    /* 判断SPIFFS挂载及初始化是否成功 */
    if (ret_val != ESP_OK)
    {
        if (ret_val == ESP_FAIL)
        {
            printf("Failed to mount or format filesystem\n");
        }
        else if (ret_val == ESP_ERR_NOT_FOUND)
        {
            printf("Failed to find SPIFFS partition\n");
        }
        else
        {
            printf("Failed to initialize SPIFFS (%s)\n", esp_err_to_name(ret_val));
        }

        return ESP_FAIL;
    }

    /* 打印SPIFFS存储信息 */
    size_t total = 0, used = 0;
    ret_val = esp_spiffs_info(conf.partition_label, &total, &used);

    if (ret_val != ESP_OK)
    {
        ESP_LOGI(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret_val));
    }
    else
    {
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    }

    return ret_val;
}

/**
 * @brief       注销spiffs初始化
 * @param       partition_label：分区表标识
 * @retval      无
 */
esp_err_t spiffs_deinit(char *partition_label)
{
    return esp_vfs_spiffs_unregister(partition_label);
}

/**
 * @brief       测试spiffs
 * @param       无
 * @retval      无
 */
void spiffs_test(void)
{

    ESP_LOGI(TAG, "Opening file");
    /* 建立一个名为/spiffs/hello.txt的只写文件 */
    FILE* f = fopen("/spiffs/hello.txt", "w");

    if (f == NULL)
    {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return;
    }

    /* 写入字符 */
    fprintf(f, WRITE_DATA);

    fclose(f);
    ESP_LOGI(TAG, "File written");
 
    /* 重命名之前检查目标文件是否存在 */
    struct stat st;

    if (stat("/spiffs/foo.txt", &st) == 0) /* 获取文件信息，获取成功返回0 */
    {
        /*  从文件系统中删除一个名称。
            如果名称是文件的最后一个连接，并且没有其它进程将文件打开，
            名称对应的文件会实际被删除。 */
        unlink("/spiffs/foo.txt");
    }
 
    /* 重命名创建的文件 */
    ESP_LOGI(TAG, "Renaming file");

    if (rename("/spiffs/hello.txt", "/spiffs/foo.txt") != 0)
    {
        ESP_LOGE(TAG, "Rename failed");
        return;
    }
 
    /* 打开重命名的文件并读取 */
    ESP_LOGI(TAG, "Reading file");
    f = fopen("/spiffs/foo.txt", "r");

    if (f == NULL)
    {
        ESP_LOGE(TAG, "Failed to open file for reading");
        return;
    }

    char line[64];
    fgets(line, sizeof(line), f);
    fclose(f);
    
    char* pos = strchr(line, '\n'); /* 指针pos指向第一个找到‘\n’ */

    if (pos)
    {
        *pos = '\0';                /* 将‘\n’替换为‘\0’ */
    }

    ESP_LOGI(TAG, "Read from file: '%s'", line);

    lcd_show_string(90, 20, 200, 16, 16, line, RED);
}

/**
 * @brief       程序入口
 * @param       无
 * @retval      无
 */
void app_main(void)
{
    esp_err_t ret;

    ret = nvs_flash_init();                                         /* 初始化NVS */
    
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    ESP_ERROR_CHECK(ret);

    led_init();                                                     /* LCD初始化 */
    spi2_init();                                                    /* SPI初始化 */
    lcd_init();                                                     /* LCD初始化 */
    spiffs_init("storage", DEFAULT_MOUNT_POINT, DEFAULT_FD_NUM);    /* SPIFFS初始化 */

    /* 显示实验信息 */
    lcd_show_string(0, 0, 200, 16, 16, "SPIFFS TEST", RED);
    lcd_show_string(0, 20, 200, 16, 16, "Read file:", BLUE);

    spiffs_test();                                                  /* SPIFFS测试 */

    while (1)
    {
        LED_TOGGLE();
        vTaskDelay(500);
    }
}
