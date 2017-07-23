#include "log_uart.h"

#include "bsp_config.h"
#include "app_timer.h"

DEFINE_BUF_QUEUE(EMDRV_UARTDRV_MAX_CONCURRENT_RX_BUFS, rxBufferQueue_LOG_LEUSART);// Define receive operation queues
DEFINE_BUF_QUEUE(EMDRV_UARTDRV_MAX_CONCURRENT_TX_BUFS, txBufferQueue_LOG_LEUSART);// Define transmit operation queues

// Configuration for leuart
#define LOG_USART                                               \
{                                                               \
    LOG_USART_DEVICE,                                           \
    LOG_USART_BAUDRATE,                                         \
    LOG_USART_DEVICE_POS,                                       \
    usartStopbits1,                                             \
    usartNoParity,                                              \
    usartOVS16,                                                 \
    false,                                                      \
    uartdrvFlowControlNone,                                     \
    COMMON_UNUSE_PORT,                                          \
    COMMON_UNUSE_PIN,                                           \
    COMMON_UNUSE_PORT,                                          \
    COMMON_UNUSE_PIN,                                           \
    (UARTDRV_Buffer_FifoQueue_t *)&rxBufferQueue_LOG_LEUSART,   \
    (UARTDRV_Buffer_FifoQueue_t *)&txBufferQueue_LOG_LEUSART    \
}
#define LOG_LEUART                                              \
{                                                               \
    LOG_USART_DEVICE,                                           \
    LOG_USART_BAUDRATE,                                         \
    LOG_USART_DEVICE_POS,                                       \
    leuartStopbits1,                                            \
    leuartNoParity,                                             \
    uartdrvFlowControlNone,                                     \
    COMMON_UNUSE_PORT,                                          \
    COMMON_UNUSE_PIN,                                           \
    COMMON_UNUSE_PORT,                                          \
    COMMON_UNUSE_PIN,                                           \
    (UARTDRV_Buffer_FifoQueue_t *)&rxBufferQueue_LOG_LEUSART,   \
    (UARTDRV_Buffer_FifoQueue_t *)&txBufferQueue_LOG_LEUSART    \
}

static UARTDRV_Handle_t log_handle;
static uint8_t log_rx_buffer[LOG_RX_BUF_MAX_SIZE] = {0, };
static uart_rx log_data_rx_cb = NULL;
static bool log_is_data_rx_cb_flag = false;


/******************************************************************************
 *                         ʵ�֣�log uart dma timeout check
 *****************************************************************************/
static RTCDRV_TimerID_t log_uart_dma_timer_id;

/******************************************************************************
 * @brief  log_uart_dma_timer_hander
 *****************************************************************************/
static void log_uart_dma_timer_hander(RTCDRV_TimerID_t id, void *user)
{
    //log_data_rx_cbδ���������ֱ���˳����ȴ���һ����ʱ�¼��Ŵ���
    if(false == log_is_data_rx_cb_flag)
    {
        //��ʱ��ֹrx
        uartdrv_get_transfered_len(log_handle);
    }
    else
    {
        LOG(LEVEL_DEBUG, "\033[1;34;40m[%s]log_data_rx_cb busy.\033[0m");
    }
}

/******************************************************************************
 * @brief  log_uart_dma_timer_restart
 *****************************************************************************/
void log_uart_dma_timer_restart(uint32_t ms)
{
    app_timer_restart(log_uart_dma_timer_id, rtcdrvTimerTypeOneshot, ms, log_uart_dma_timer_hander, NULL);
}

/******************************************************************************
 * @brief  log_uart_dma_timer_check_running
 *****************************************************************************/
static bool log_uart_dma_timer_check_running(void)
{
    return app_timer_check_running(log_uart_dma_timer_id);
}

/******************************************************************************
 * @brief  log_uart_dma_timer_init
 *****************************************************************************/
static void log_uart_dma_timer_init(void)
{
    //��������DMA���ճ�ʱ��ⶨʱ��
    app_timer_create(&log_uart_dma_timer_id);
}


/******************************************************************************
 *                         ʵ�֣�log uart driver
 *****************************************************************************/
static int16_t log_uart_set_rx_active(void);
/******************************************************************************
 * @brief ����������dma���ջص�
 *****************************************************************************/
static void log_uart_rx_cb(struct UARTDRV_HandleData *handle,
                                   Ecode_t transferStatus,
                                   uint8_t *data,
                                   UARTDRV_Count_t transferCount)
{
    APP_ERROR_CHECK_BOOL(log_data_rx_cb != NULL);

    log_is_data_rx_cb_flag = true;

    if(UART_TRANSFER_OK == transferStatus)
    {
        //������ⳬʱ��ʱ��
        log_uart_dma_timer_restart(LOG_UART_DMA_TIMEOUT_MS);
    }

    //log���ڽ�������
    log_data_rx_cb(transferStatus, data, transferCount);

    //����log������Ч
    log_uart_set_rx_active();

    log_is_data_rx_cb_flag = false;
}

/******************************************************************************
 * @brief ����log_uart������Ч
 *****************************************************************************/
static int16_t log_uart_set_rx_active(void)
{
    uint32_t ret;
    uint32_t count;

    memset(log_rx_buffer, 0, sizeof(log_rx_buffer));

    //����dma���գ�buffer�Ĵ�С����count���ֽڣ�����dma�ж�
    if(true == log_uart_dma_timer_check_running())
    {
        count = LOG_DEAL_RX_BUF_SIZE;
    }
    else
    {
        count = LOG_DEAL_RX_BUF_SIZE;// 1
    }

    ret = UARTDRV_Receive(log_handle, log_rx_buffer, count, log_uart_rx_cb);// Transmit data using a no-blocking transmit function

    return ret;
}

/*****************************************************************************
 * @brief log_uart_init
 *****************************************************************************/
int16_t log_uart_init(uart_rx uart_rx_cb)
{
    uint32_t ret ;

    //����־�����豸���
    uart_open_device(&log_handle);

    //��ʼ��log�����豸
#if (false == LOG_USART_DEVICE_USE_LEUART)
    UARTDRV_InitUart_t initData = LOG_USART;

    ret = UARTDRV_InitUart(log_handle, &initData);
#else
    UARTDRV_InitLeuart_t initData = LOG_LEUART;
    ret = UARTDRV_InitLeuart(log_handle, &initData);
#endif

    return ret;
}

/******************************************************************************
 * @brief log_uart_tx
 *****************************************************************************/
int16_t log_uart_tx(uint8_t *data, uint32_t count)
{
    // Transmit data using a blocking transmit function
    uint32_t ret = UARTDRV_ForceTransmit(log_handle, data, count);

    return ret;
}

/******************************************************************************
 * @brief log_uart_shutdown
 *****************************************************************************/
int16_t log_uart_shutdown(void)
{
    return 0;
}

/*****************************************************************************
 * @brief log_uart_rx_start
 *****************************************************************************/
void log_uart_rx_start(uart_rx uart_rx_cb)
{
    APP_ERROR_CHECK_BOOL(uart_rx_cb != NULL);

    //���ݽ��ջص�ָ��
    log_data_rx_cb = uart_rx_cb;
    log_is_data_rx_cb_flag = false;

    //����DMA���ճ�ʱ��ⶨʱ����ʼ��
    log_uart_dma_timer_init();

    //����log������Ч
    log_uart_set_rx_active();
}


