#ifndef __BSP_CONFIG_A_H
#define __BSP_CONFIG_A_H

#include "module_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************ʱ��*************************************/
//�ⲿ��Ƶ��������ʹ��λ
#define HAL_USE_HFXO                                (1)

//�ⲿ��Ƶ��������ʹ��λ
#define HAL_USE_LFXO                                (1)

//CPUʱ��Ƶ��
#if (0 == HAL_USE_HFXO)
#define configCPU_CLOCK_HZ_DEFINE                   (28*1000*1000)
#else
#define configCPU_CLOCK_HZ_DEFINE                   (48*1000*1000)
#endif

/****************************Ӳ��IO***************************************/
//���õ�����
#define COMMON_UNUSE_PORT                           (gpioPortE)
#define COMMON_UNUSE_PIN                            (9)


#if (1)/*defined(FM_LOG)*/
/********************************log_uart.h********************************/
#define LOG_USART_DEVICE                            (USART2)
#define LOG_USART_DEVICE_POS                        (_USART_ROUTE_LOCATION_LOC0)

#define LOG_UART_CLOCK                              (cmuClock_USART2)

#define LOG_USART_TX_PORT                           (gpioPortC)
#define LOG_USART_TX_PIN                            (2)

#define LOG_USART_RX_PORT                           (gpioPortC)
#define LOG_USART_RX_PIN                            (3)
#define LOG_USART_DEVICE_USE_LEUART                 (false)
#endif


#ifdef __cplusplus
}
#endif

#endif

