#include "platform.h"

#include "em_int.h"
#include "app_timer.h"

#include "log.h"

extern int16_t is_free_rtos_Heap_space_flag(void *addr);//����heap.c�Ľӿ�

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
                                     )
{
    BaseType_t xStatus;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    volatile uint32_t isr_vector_num = (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk);
    if (0 == isr_vector_num)
    {
        xStatus = xQueueSendToBack(xQueue, pvItemToQueue, xTicksToWait);
    }
    else
    {
        xStatus = xQueueSendToBackFromISR(xQueue, pvItemToQueue, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    return xStatus;
}

#define operate_p(a)                ((a)++)

volatile static uint32_t pvPortMalloc_cnt = 0;
volatile static uint32_t malloc_cnt = 0;

volatile static uint32_t vPortFree_cnt = 0;
volatile static uint32_t free_cnt = 0;

/***********************************************************************************************
 * mem_manager_print
 **********************************************************************************************/
void mem_manager_print(void)
{
    uint32_t malloc_cnt_1 = pvPortMalloc_cnt;
    uint32_t free_cnt_1 = vPortFree_cnt;

    uint32_t malloc_cnt_2 = malloc_cnt;
    uint32_t free_cnt_2 = free_cnt;

    LOG(LEVEL_DEBUG, "pvPortMallocs=%d, mallocs=%d", malloc_cnt_1 - free_cnt_1, malloc_cnt_2 - free_cnt_2);
}

/***********************************************************************************************
 * ��дpvPortMalloc��malloc�ӿ�
 * size_t bytes:            ��̬������ڴ��ֽ���
 * note��SCB->ICSR��                �����ж�������
 **********************************************************************************************/
void * malloc_overwrite(size_t bytes)
{
    void * ptr = NULL;
    volatile uint32_t isr_vector_num = (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk);
    if (0 == isr_vector_num)
    {
        //�ӴӶ�ջpsp���ڴ�ռ����:���������free rtos��api
        ptr = pvPortMalloc(bytes);
        operate_p(pvPortMalloc_cnt);
    }
    else
    {
        //������ջmsp���ڴ�ռ����
        //Ϊ��ֹ�ж���ռ�������￪���ж�ʵ�ֻ���
        INT_Disable();
        ptr = malloc(bytes);
        operate_p(malloc_cnt);
        INT_Enable();
    }
    return ptr;
}

/***********************************************************************************************
 * ��дvPortFree��free�ӿ�
 * void *ptr:            ��̬������ڴ��ַ
 * note��SCB->ICSR��                �����ж�������
 **********************************************************************************************/
void free_overwrite(void *ptr)
{
    if (0 == is_free_rtos_Heap_space_flag(ptr))
    {
        vPortFree(ptr);
        operate_p(vPortFree_cnt);
    }
    else
    {
        //free()����ִ��ʱ��̣�Ϊ��ֹ�ж���ռ�������￪���ж�ʵ�ֻ���
        INT_Disable();
        free(ptr);
        operate_p(free_cnt);
        INT_Enable();
    }
}

/******************************************************************************
 * @brief  sleep_overwrite
 *****************************************************************************/
void sleep_overwrite(uint32_t ms)
{
    uint16_t delay = ms / portTICK_RATE_MS;

    //�������˯�ߵ���Сֵ
    APP_ERROR_CHECK_BOOL(delay >= 1);

    vTaskDelay(delay);
}

/*******************************************************************************
 * @brief get_uptime_second
 ******************************************************************************/
uint32_t get_uptime_second(void)
{
    return app_timer_get_second();
}

/******************************************************************************
 * @brief  vApplicationStackOverflowHook
 *****************************************************************************/
#if configCHECK_FOR_STACK_OVERFLOW > 0
void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
    LOG(LEVEL_FORCE, "\r\n\r\nstack over flow on thread:%s", pcTaskName);

    while(1);
}
#endif

/******************************************************************************
 * @brief  vApplicationMallocFailedHook
 *****************************************************************************/
#if( configUSE_MALLOC_FAILED_HOOK == 1 )
void vApplicationMallocFailedHook( void )
{
    LOG(LEVEL_FORCE, "\r\n\r\nthread malloc fail");

    while(1);
}
#endif

#if (RELEASE_VERSION_ENABLE == 0)
/******************************************************************************
 * @brief  hard_fault_handler_c
 *****************************************************************************/
void hard_fault_handler_c(unsigned int * hardfault_args)
{
    unsigned int stacked_r0;
    unsigned int stacked_r1;
    unsigned int stacked_r2;
    unsigned int stacked_r3;
    unsigned int stacked_r12;
    unsigned int stacked_lr;
    unsigned int stacked_pc;
    unsigned int stacked_psr;

    stacked_r0 = ((unsigned long)hardfault_args[0]);
    stacked_r1 = ((unsigned long)hardfault_args[1]);
    stacked_r2 = ((unsigned long)hardfault_args[2]);
    stacked_r3 = ((unsigned long)hardfault_args[3]);

    stacked_r12 = ((unsigned long)hardfault_args[4]);
    stacked_lr  = ((unsigned long)hardfault_args[5]);
    stacked_pc  = ((unsigned long)hardfault_args[6]);
    stacked_psr = ((unsigned long)hardfault_args[7]);

    LOG(LEVEL_DEBUG, "[Hard fault handler - all numbers in hex]");
    LOG(LEVEL_DEBUG, "R0 = 0x%x", stacked_r0);
    LOG(LEVEL_DEBUG, "R1 = 0x%x", stacked_r1);
    LOG(LEVEL_DEBUG, "R2 = 0x%x", stacked_r2);
    LOG(LEVEL_DEBUG, "R3 = 0x%x", stacked_r3);
    LOG(LEVEL_DEBUG, "R12 = 0x%x", stacked_r12);
    LOG(LEVEL_DEBUG, "LR [R14] = 0x%x  subroutine call return address", stacked_lr);
    LOG(LEVEL_DEBUG, "PC [R15] = 0x%x  program counter", stacked_pc);
    LOG(LEVEL_DEBUG, "PSR = 0x%x", stacked_psr);

    while (1);
}

/******************************************************************************
 * @brief  HardFault_Handler
 *****************************************************************************/
#if defined ( __CC_ARM   )
__asm void HardFault_Handler(void)
{
    TST lr, #4
    ITE EQ
    MRSEQ r0, msp
    MRSNE r0, psp
    IMPORT hard_fault_handler_c
    B hard_fault_handler_c
}
#else
void HardFault_Handler(void)
{
    __asm("TST LR, #4");   //��LR��ֵ��4��λ����
    __asm("ITE EQ");       //��Ϊ0����MSP��������PSP
    __asm("MRSEQ R0, MSP");
    __asm("MRSNE R0, PSP");
    __asm("B hard_fault_handler_c");
}
#endif

#endif

