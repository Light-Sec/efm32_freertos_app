#include "app_timer_drv_freertos.h"

#include "log.h"
#include "timers.h"
#include "calendar.h"

#if (APP_TIMER_DRV_FREERTOS == APP_TIMER_DRV_SRC)
/******************************************************************************
                        ����ϵͳtick��ʱ��ʵ��ƫ�������ͺ�����
 *****************************************************************************/

/*******************************************************************************
 * @brief app_timer_get_second
 ******************************************************************************/
uint32_t app_timer_get_second(void)
{
    return get_second_wallclock();
}

/*******************************************************************************
 * @brief app_timer_get_millisecond
 ******************************************************************************/
uint32_t app_timer_get_millisecond(void)
{
    //�޷�ʵ��
}
#endif

/******************************************************************************
                        ����ϵͳsoft_timersʵ�������ʱ��
 *****************************************************************************/
static volatile TimerHandle_t     timer[EMDRV_RTCDRV_NUM_TIMERS] = {NULL, };
static volatile RTCDRV_Callback_t timeout_callback[EMDRV_RTCDRV_NUM_TIMERS] = {NULL, };
static volatile bool              timer_is_running[EMDRV_RTCDRV_NUM_TIMERS] = {false, };
static uint16_t                   timer_count = 0;

/*******************************************************************************
 * @brief empty_timer_callback
 ******************************************************************************/
static void empty_timer_callback(TimerHandle_t xtimer)
{
    uint32_t id = (uint32_t)pvTimerGetTimerID(xtimer);

    if(NULL != timeout_callback[id])
    {
        timeout_callback[id](id, NULL);
    }

    //��ǰ��ʱ����ʱ���н���
    timer_is_running[id] = false;
}

/*******************************************************************************
 * @brief app_timer_drv_check_running
 ******************************************************************************/
bool app_timer_drv_check_running(RTCDRV_TimerID_t timer_id)
{
    //warnning:xTimerIsTimerActive�������ж���ͷ����

    return timer_is_running[timer_id];
}

/*******************************************************************************
 * @brief app_timer_drv_create
 * note : timer������
 * �޸� rtcdrv_config.h  �궨��#define EMDRV_RTCDRV_NUM_TIMERS
 * �޸� FreeRTOSConfig.h �궨��#configTIMER_QUEUE_LENGTH
 ******************************************************************************/
void app_timer_drv_create(RTCDRV_TimerID_t *id)
{
    //warnning:xTimerCreate���������жϵ���

    timer[timer_count] = xTimerCreate(NULL, pdMS_TO_TICKS(1000), pdFALSE, (void *)(timer_count), empty_timer_callback);

    //��ǰ��ʱ����ʼ��ֹͣ����
    timer_is_running[timer_count] = false;

    *id = timer_count;

    timer_count++;
    APP_ERROR_CHECK_BOOL(EMDRV_RTCDRV_NUM_TIMERS >= timer_count);
    APP_ERROR_CHECK_BOOL(NULL != timer[timer_count - 1]);
}

/*******************************************************************************
 * @brief app_timer_drv_stop
 ******************************************************************************/
void app_timer_drv_stop(RTCDRV_TimerID_t id)
{
    APP_ERROR_CHECK_BOOL(id != 0);

    portBASE_TYPE xStatus;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    volatile uint32_t isr_vector_num = (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk);
    portTickType xTicksToWait = 500/portTICK_RATE_MS;


    if (0 == isr_vector_num)
    {
        xStatus = xTimerStop(timer[id], xTicksToWait);
    }
    else
    {
        xStatus = xTimerStopFromISR(timer[id], &xHigherPriorityTaskWoken);
    }
    APP_ERROR_CHECK_BOOL(xStatus == pdPASS);

    //��ǰ��ʱ��ֹͣ����
    timer_is_running[id] = false;
}

/*******************************************************************************
 * @brief app_timer_drv_restart
 ******************************************************************************/
void app_timer_drv_restart(RTCDRV_TimerID_t id,
                        RTCDRV_TimerType_t type,
                        uint32_t timeout,
                        RTCDRV_Callback_t callback,
                        void *user)
{
    APP_ERROR_CHECK_BOOL(id != 0);

    //warnning:��ʹ�ò���type��user
    portBASE_TYPE xStatus;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    volatile uint32_t isr_vector_num = (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk);
    portTickType xTicksToWait = 500/portTICK_RATE_MS;


    //ֹͣ��ʱ��
    app_timer_drv_stop(id);

    //�������ó�ʱ����
    timeout_callback[id] = callback;

    if(0 == isr_vector_num)
    {
        xStatus = xTimerChangePeriod(timer[id], pdMS_TO_TICKS(timeout), xTicksToWait);
    }
    else
    {
        xStatus = xTimerChangePeriodFromISR(timer[id], pdMS_TO_TICKS(timeout), &xHigherPriorityTaskWoken);
    }
    APP_ERROR_CHECK_BOOL(xStatus == pdPASS);

    //������ʱ��
    if(0 == isr_vector_num)
    {
        xStatus = xTimerStart(timer[id], xTicksToWait);
    }
    else
    {
        xStatus = xTimerStartFromISR(timer[id], &xHigherPriorityTaskWoken);
    }
    APP_ERROR_CHECK_BOOL(xStatus == pdPASS);

    //��ǰ��ʱ����ʼ����
    timer_is_running[id] = true;
}

