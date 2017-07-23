#ifndef __LOG_H
#define __LOG_H

#include "platform.h"

#ifndef USE_BOOTLOADER
#include "log_uart.h"
#else
#include "uart.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif

#ifndef RELEASE_VERSION_ENABLE
//release app����bootloader�汾ʹ��λ
#define RELEASE_VERSION_ENABLE              (1)//(0) //Ĭ�ϴ�
#endif


#ifndef USE_BOOTLOADER
    #define DEBUG_LOG                       (0 == RELEASE_VERSION_ENABLE)
#else
    #define DEBUG_LOG                       (1)//bootloader ʼ�մ�
#endif

//��־�̰߳�ȫʹ��λ
#ifndef USE_BOOTLOADER
#define LOG_THREAD_SAFE                     (1)
#else
#define LOG_THREAD_SAFE                     (0)
#endif

//��־ʱ���ӡʹ��λ
#ifndef USE_BOOTLOADER
#define LOG_TIME_PRINT                      (1)
#else
#define LOG_TIME_PRINT                      (0)
#endif


//level
#define LEVEL_CLOSE                         (1)
#define LEVEL_SIMPLE_FORCE                  (4)
#define LEVEL_FORCE                         (5)

#define LEVEL_RELEASE                       (10)
#define LEVEL_SIMPLE                        (11)
#define LEVEL_DEBUG                         (12)
#define LEVEL_INFO                          (13)
#define LEVEL_WARNING                       (14)
#define LEVEL_ERROR                         (15)
#define __LEVEL__                           (LEVEL_ERROR)


//�͹��Ĺ�����־�Ƿ�ǿ�����ʹ��λ
#define MANAGER_SLEEP_LOG_LOG_EN            (0)

#if (MANAGER_SLEEP_LOG_LOG_EN == 1)
#define LEVEL_MANAGER_SLEEP                 (LEVEL_DEBUG)
#else
#define LEVEL_MANAGER_SLEEP                 (LEVEL_CLOSE)
#endif


/* when LOG_THREAD_SAFE  is  1,
 * level :
 *           LEVEL_FORCE ���� �̲߳���ȫ��ֱ�ӵ��õײ����������һ��ϵͳ�쳣����־���ø�level
 *              ���� �������������������� �̰߳�ȫ����־�߳����
 *
 *             */
void __log(uint8_t level, /*const char * func, uint32_t line,*/ const char * restrict format, ...);

#define LOG_PRINT(level, format, ...)               \
    do                                              \
    {                                               \
        if( (LEVEL_SIMPLE_FORCE  <= level )         \
         && (level <= __LEVEL__) )                  \
        {                                           \
            __log(level, format, ##__VA_ARGS__);    \
        }                                           \
    }while(0)

#if (DEBUG_LOG == 1)
    #define LOG(level, format, ...)             LOG_PRINT(level, format, ##__VA_ARGS__)
    #define LOG_RELEASE(level, format, ...)     LOG_PRINT(level, format, ##__VA_ARGS__)
#else
    #define LOG(level, format, ...)
    #define LOG_RELEASE(level, format, ...)     LOG_PRINT((LEVEL_SIMPLE == level)? LEVEL_SIMPLE: ((LEVEL_FORCE == level)? LEVEL_SIMPLE_FORCE: LEVEL_RELEASE), format, ##__VA_ARGS__)
#endif

//���⺯���ӿ�
void log_init(uart_init log_uart_init, uart_tx log_uart_tx, uart_shutdown log_uart_shutdown);

#ifdef __cplusplus
}
#endif

#endif

