#include "em_chip.h"
#include "em_rmu.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "sleep.h"

#include "wdt.h"
#include "app_timer.h"
#include "delay.h"
#include "hal_manager.h"

#include "log.h"

#include "bsp_config.h"

//����task��ͷ�ļ�
#include "global.h"
#include "thread_manager.h"

/******************************************************************************
 * @brief  cmu_setup
 *****************************************************************************/
static void cmu_setup(void)
{
#if (HAL_USE_HFXO == 1)
    #warning "use HFXO"
    /* Enable clock for HF peripherals */
    CMU_ClockEnable(cmuClock_HFPER, true);
    /* Setup crystal frequency */
    SystemHFXOClockSet(configCPU_CLOCK_HZ_DEFINE);
    /* Use HFXO as core clock frequency */
    CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
    /* Disable HFRCO */
    CMU_OscillatorEnable(cmuOsc_HFRCO, false, false);

    /* enable HFXO */
    CMU_OscillatorEnable(cmuOsc_HFXO, true, true);
    while (!(CMU->STATUS & CMU_STATUS_HFXORDY))
    {
    }
#else
    #warning "not use HFXO"
    //�ϵ�Ĭ�����ڲ�RC����14MHZ�����Ե������½ӿ��޸�HFRCOƵ��
    //Set the clock frequency to 11MHz so the ADC can run on the undivided HFCLK
    CMU_HFRCOBandSet(cmuHFRCOBand_28MHz);
#endif

    /* Starting LFRCO and waiting until it is stable */
    CMU_OscillatorEnable(cmuOsc_LFRCO, true, true);
    while (!(CMU->STATUS & CMU_STATUS_LFRCORDY))
    {
    }

    /* Enabling clock to the interface of the low energy modules */
    CMU_ClockEnable(cmuClock_CORELE, true);

#if (HAL_USE_LFXO == 1)
    #warning "use LFXO"
    /* Starting LFXO and waiting in EM2 until it is stable */
    CMU_OscillatorEnable(cmuOsc_LFXO, true, false);
    while (!(CMU->STATUS & CMU_STATUS_LFXORDY))
    {
    }
#else
    #warning "not use LFXO"
#endif
}

/******************************************************************************
 * @brief  Main function
 *****************************************************************************/
int main(void)
{
    /* Chip errata */
    CHIP_Init();
    /* If first word of user data page is non-zero, enable eA Profiler trace */
    //BSP_TraceProfilerSetup();

    /* Initialize SLEEP driver, no calbacks are used */
    SLEEP_Init(NULL, NULL);

#if (configSLEEP_MODE < 3)
    /* do not let to sleep deeper than define */
    SLEEP_SleepBlockBegin((SLEEP_EnergyMode_t)(configSLEEP_MODE+1));
#endif

    //ʱ��ϵͳ������ʼ��
    cmu_setup();

    //�����ж����ȼ���
    NVIC_SetPriorityGrouping(0);//[7:5] b100-b000 ȫ������Ϊ��ռʽ���ȼ�

    //���Ź���ʼ��
    wdt_init();
    wdt_start();

    //��־ģ���ʼ������־�߳�
    log_init(log_uart_init, log_uart_tx, log_uart_shutdown);

    //�����ʱ����ʼ��
    app_timer_init();

    //��ʱ��ʵ����ʱ��ʼ��
    delay_init();

    /******************************************************
    * ���´������������̣߳���Ϊ�����̷߳���������Ź���ͬʱ��������̣߳������ظ����̵߳Ĵ�
    ******************************************************/

    //1��ϵͳ�¼����ȳ�ʼ��:�¼������߳�
    app_task_sched_init();

    //Ӳ����ʼ������������ģ����Ӧ���߳�
    hal_init();

    //�����߳�
    manager_init();

    //����߳�
    monitor_init();

    //�������߳�
    period_init();

    //��ʼFreeRTOS������
    vTaskStartScheduler();

    //���һ��������main()������Ӧ�û�ִ�е���������ִ�е�����ܿ������ڴ�ѿռ䲻�㵼�¿��������޷�������
    while(1)
    {
    }

    return 0;
}
