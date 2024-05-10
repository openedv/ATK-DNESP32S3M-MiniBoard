/**
 ****************************************************************************************************
 * @file        udp.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2023-08-26
 * @brief       LWIP实验
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

#include "lwip_demo.h"


#define LWIP_DEMO_RX_BUFSIZE         200                        /* 最大接收数据长度 */
#define LWIP_DEMO_PORT               8080                       /* 连接的本地端口号 */
#define LWIP_SEND_THREAD_PRIO       ( tskIDLE_PRIORITY + 3 )    /* 发送数据线程优先级 */

/* 接收数据缓冲区 */
uint8_t g_lwip_demo_recvbuf[LWIP_DEMO_RX_BUFSIZE]; 
/* 发送数据内容 */
uint8_t g_lwip_demo_sendbuf[] = "ALIENTEK DATA \r\n";

/* 数据发送标志位 */
uint8_t g_lwip_send_flag;
int g_sock_conn;                          /* 请求的 socked */
int g_lwip_connect_state = 0;
static void lwip_send_thread(void *arg);


/**
 * @brief       发送数据线程
 * @param       无
 * @retval      无
 */
void lwip_data_send(void)
{
    xTaskCreate(lwip_send_thread, "lwip_send_thread", 4096, NULL, LWIP_SEND_THREAD_PRIO, NULL);
}

/**
 * @brief       lwip_demo实验入口
 * @param       无
 * @retval      无
 */
void lwip_demo(void)
{
    struct sockaddr_in server_addr; /* 服务器地址 */
    struct sockaddr_in conn_addr;   /* 连接地址 */
    socklen_t addr_len;             /* 地址长度 */
    int err;
    int length;
    int sock_fd;
    lwip_data_send();                                       /* 创建一个发送线程 */
    
    sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);    /* 建立一个新的socket连接 */
    memset(&server_addr, 0, sizeof(server_addr));           /* 将服务器地址清空 */
    server_addr.sin_family = AF_INET;                       /* 地址家族 */
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);        /* 注意转化为网络字节序 */
    server_addr.sin_port = htons(LWIP_DEMO_PORT);           /* 使用SERVER_PORT指定为程序头设定的端口号 */
    
    err = bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)); /* 建立绑定 */

    if (err < 0)                /* 如果绑定失败则关闭套接字 */
    {
        closesocket(sock_fd);   /* 关闭套接字 */
    }

    err = listen(sock_fd, 4);   /* 监听连接请求 */

    if (err < 0)                /* 如果监听失败则关闭套接字 */
    {
        closesocket(sock_fd);   /* 关闭套接字 */
    }
    
    while(1)
    {
        g_lwip_connect_state = 0;
        addr_len = sizeof(struct sockaddr_in); /* 将链接地址赋值给addr_len */

        g_sock_conn = accept(sock_fd, (struct sockaddr *)&conn_addr, &addr_len); /* 对监听到的请求进行连接，状态赋值给sock_conn */

        if (g_sock_conn < 0) /* 状态小于0代表连接故障，此时关闭套接字 */
        {
            closesocket(sock_fd);
        }
        else 
        {
            lcd_show_string(5, 190, 200, 16, 16, "State:Connection...", BLUE);
            g_lwip_connect_state = 1;
        }

        while (1)
        {
            memset(g_lwip_demo_recvbuf,0,LWIP_DEMO_RX_BUFSIZE);
            length = recv(g_sock_conn, (unsigned int *)g_lwip_demo_recvbuf, sizeof(g_lwip_demo_recvbuf), 0); /* 将收到的数据放到接收Buff */
            
            if (length <= 0)
            {
                lcd_show_string(5, 190, 200, 16, 16, "State:no Connection", BLUE);
                break;
            }
            
            printf("%s\r\n",g_lwip_demo_recvbuf);
        }


        if (g_sock_conn >= 0)
        {
            closesocket(g_sock_conn);
        }

        g_sock_conn = -1;
    }
}

/**
 * @brief       发送数据线程函数
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void lwip_send_thread(void *pvParameters)
{
    pvParameters = pvParameters;
    
    while (1)
    {
        if(((g_lwip_send_flag & LWIP_SEND_DATA) == LWIP_SEND_DATA) && (g_lwip_connect_state == 1)) /* 有数据要发送 */
        {
            send(g_sock_conn, g_lwip_demo_sendbuf, sizeof(g_lwip_demo_sendbuf), 0); /* 发送数据 */
            g_lwip_send_flag &= ~LWIP_SEND_DATA;
        }
        
        vTaskDelay(1);
    }
}
