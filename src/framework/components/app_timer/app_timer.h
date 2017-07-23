#ifndef APP_TIMER_H
#define APP_TIMER_H

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include "string.h"
#include "app_error.h"

#include "app_timer_drv.h"

#ifdef __cplusplus
extern "C" {
#endif

//�ⲿ�����ӿ�
/*******************************************************************************
 * @brief �����ʱ����ʼ����
 * note : timer�������޸� rtcdrv_config.h �궨��#define EMDRV_RTCDRV_NUM_TIMERS
 ******************************************************************************/
void app_timer_init(void);
bool app_timer_check_running(RTCDRV_TimerID_t timer_id);
void app_timer_create(RTCDRV_TimerID_t *id);
void app_timer_stop(RTCDRV_TimerID_t id);
void app_timer_restart(RTCDRV_TimerID_t id,
                        RTCDRV_TimerType_t type,
                        uint32_t timeout,
                        RTCDRV_Callback_t callback,
                        void *user);

uint32_t app_timer_get_compare_second(const uint32_t s_1, const uint32_t s_2);
uint32_t app_timer_get_compare_millisecond(const uint32_t ms_1, const uint32_t ms_2);


#ifdef __cplusplus
}
#endif


#endif
