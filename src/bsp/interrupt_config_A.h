#ifndef __INTERRUPT_CONFIG_A_H
#define __INTERRUPT_CONFIG_A_H


#ifdef __cplusplus
extern "C" {
#endif

#if (1)
//����os_tickʵ�ֶ�ʱ����Դ
#define OS_SYSTICK_RTC               (1)
#define OS_SYSTICK_BURTC             (2)
#define OS_SYSTICK_SOURCE            (OS_SYSTICK_RTC)
#endif

#if (1)
//����app_timerʵ�ֶ�ʱ����Դ
#define APP_TIMER_DRV_RTC            (1)
#define APP_TIMER_DRV_BURTC          (2)
#define APP_TIMER_DRV_LETIMER        (3)
#define APP_TIMER_DRV_FREERTOS       (4)
#define APP_TIMER_DRV_SRC            (APP_TIMER_DRV_LETIMER)
#define APP_TIMER_DRV_IRQHandler     (LETIMER0_IRQHandler)
#endif


#if (1)
//���¶�������ж�Ӳ�����ȼ�
//����ϵͳ�ɹ���
#define LOWEST_IRQ_PRIORITY          (7)//���ɸģ��ṩfree rtosϵͳ����ʱ��

#define RTC_IRQ_PRIORITY             ((OS_SYSTICK_RTC == OS_SYSTICK_SOURCE)? LOWEST_IRQ_PRIORITY : 6)
#define BURTC_IRQ_PRIORITY           ((OS_SYSTICK_BURTC == OS_SYSTICK_SOURCE)? LOWEST_IRQ_PRIORITY : 6)
#define LETIMER_IRQ_PRIORITY         (6)
#define ADC_IRQ_PRIORITY             (7)
#define DMA_IRQ_PRIORITY             (4)
#define TIMER0_IRQ_PRIORITY          (3)//rgb pwdʵ��
#define TIMER1_IRQ_PRIORITY          (3)//buzz pwmʵ��
#define GPIOINT_IRQ_PRIORITY         (2)
#endif


#ifdef __cplusplus
}
#endif

#endif
