#ifndef __PLATFORM_H
#define __PLATFORM_H

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include "string.h"
#include "app_error.h"

//����free rtos���
#include <stdlib.h>
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "croutine.h"

#include "em_cmu.h"    //�ж������ļ��

#ifdef __cplusplus
extern "C" {
#endif

//������д��̬�ڴ���亯���ӿ�
/***********************************************************************************************
 * ��дxQueueSendToBack��xQueueSendToBackFromISR�ӿ�
 * xQueueSendToBack:
 * xQueueSendToBackFromISR: ����������:������ʱʱ�� �C ��������ʱ������ת������״̬�Եȴ����пռ���Ч
 * note��SCB->ICSR��                �����ж�������
 **********************************************************************************************/
BaseType_t xQueueSendToBack_OverWrite(
                                       QueueHandle_t    xQueue,
                                       const void       *pvItemToQueue,
                                       TickType_t       xTicksToWait
                                     );

/***********************************************************************************************
 * mem_manager_print
 **********************************************************************************************/
void mem_manager_print(void);

/***********************************************************************************************
 * ��дpvPortMalloc��malloc�ӿ�
 * size_t bytes:            ��̬������ڴ��ֽ���
 **********************************************************************************************/
void * malloc_overwrite(size_t bytes);

/***********************************************************************************************
 * ��дvPortFree��free�ӿ�
 * void *ptr:            ��̬������ڴ��ַ
 **********************************************************************************************/
void free_overwrite(void *ptr);

/******************************************************************************
 * @brief  sleep_overwrite
 *****************************************************************************/
void sleep_overwrite(uint32_t ms);

/*******************************************************************************
 * @brief get_uptime_second
 ******************************************************************************/
uint32_t get_uptime_second(void);


#ifdef __cplusplus
}
#endif

#endif

