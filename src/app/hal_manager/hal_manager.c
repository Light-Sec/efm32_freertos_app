#include "hal_manager.h"

#include "sleep.h"
#include "gpiointerrupt.h"
#include "em_gpio.h"
#include "inner_rtc_action.h"

#include "app_manager.h"
#include "thread_manager.h"
#include "app_timer.h"

#include "bsp_config.h"

#if defined(FM_INNER_BURTC)
#include "app_timer_hal_burtc.h"
#endif

#include "global.h"

static void gpio_interrupt_init_and_disable(void)
{
    NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
    NVIC_DisableIRQ(GPIO_ODD_IRQn);

    NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
    NVIC_DisableIRQ(GPIO_EVEN_IRQn);
}

static void gpio_interrupt_enable(void)
{
    NVIC_EnableIRQ(GPIO_ODD_IRQn);
    NVIC_EnableIRQ(GPIO_EVEN_IRQn);
}

/*******************************************************************************
 * @brief hal_set_common_unuse_pin
 ******************************************************************************/
static void hal_set_common_unuse_pin(void)
{
    GPIO_PinModeSet(COMMON_UNUSE_PORT, COMMON_UNUSE_PIN, gpioModeDisabled, 0);
}

/*******************************************************************************
 * @brief hal_init
 ******************************************************************************/
void hal_init(void)
{
    //�ⲿio�ж�ʵ�ֳ�ʼ��
    GPIOINT_Init();
    gpio_interrupt_init_and_disable();

#if defined(FM_INNER_BURTC)
    //BURTC��ʼ��
    hal_burtc_init();

    //�ڲ�BURTCʱ�䣬ͬ�������������
    calendar_sync_from_inner_rtc();
#endif

    //ȫ���������õ�����Ϊʧ��״̬
    hal_set_common_unuse_pin();

    //APP��ĸ�task�Լ�����Ӧ�ò���صĳ�ʼ��
    app_task_init();

    //����ϵͳ����ģ���ʼ����������Է�ֹ�ϵ��������룬�����쳣
    //��IO�ж�
    gpio_interrupt_enable();

}

