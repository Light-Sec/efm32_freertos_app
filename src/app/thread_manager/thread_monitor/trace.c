#include "trace.h"

#include "platform.h"
#include "log.h"

static TaskStatus_t pxTaskStatusArray[MAX_THREAD_NUMS];

/******************************************************************************
 * @brief  get_thread_state_implement
 * ��ȡOS������Ϣ
 *****************************************************************************/
void get_thread_state_implement(uint8_t* data, uint16_t data_length)
{
    const char task_state[] = {'r', 'R', 'B', 'S', 'D'};
    volatile UBaseType_t uxArraySize = 0;
    uint32_t ulTotalRunTime = 0;
    uint32_t ulStatsAsPercentage = 0;

    //��ȡ��������Ŀ
    uxArraySize = uxTaskGetNumberOfTasks();
    if(uxArraySize > MAX_THREAD_NUMS)
    {
        LOG(LEVEL_DEBUG, "thread nums=%d invalid", uxArraySize);
        return;
    }

    //��ȡÿ�������״̬��Ϣ
    uxArraySize = uxTaskGetSystemState(pxTaskStatusArray, uxArraySize, &ulTotalRunTime);

#if ( configUSE_TRACE_FACILITY == 1 )

    LOG(LEVEL_SIMPLE, "==================================================\r\n");
//    LOG(LEVEL_DEBUG, "������ʱ��:%d", ulTotalRunTime);
    LOG(LEVEL_SIMPLE, "������                ״̬  ID    ���ȼ�  ��ջ\r\n");

    //����õ�ÿһ������״̬��Ϣ���ֵ�ת��Ϊ����Ա����ʶ����ַ�����ʽ
    for(uint32_t x = 0; x < uxArraySize; x++ )
    {
        char tmp[128] = {0, };

        //����������
        if(ulTotalRunTime > 0)
        {
            //������������ʱ����������ʱ��İٷֱ�
            ulStatsAsPercentage = (uint64_t)(pxTaskStatusArray[ x ].ulRunTimeCounter)*100 / ulTotalRunTime;
        }

        if( ulStatsAsPercentage > 0UL )
        {
            sprintf(tmp, "%-22s%-6c%-6d%-8d%-8d", pxTaskStatusArray[ x].pcTaskName,   task_state[pxTaskStatusArray[ x ].eCurrentState],
                                                  pxTaskStatusArray[ x ].xTaskNumber, pxTaskStatusArray[ x].uxCurrentPriority,
                                                  pxTaskStatusArray[ x ].usStackHighWaterMark);
        }
        else
        {
            sprintf(tmp, "%-22s%-6c%-6d%-8d%-8d", pxTaskStatusArray[x ].pcTaskName,   task_state[pxTaskStatusArray[ x ].eCurrentState],
                                                  pxTaskStatusArray[ x ].xTaskNumber, pxTaskStatusArray[ x].uxCurrentPriority,
                                                  pxTaskStatusArray[ x ].usStackHighWaterMark);
        }

        LOG(LEVEL_SIMPLE, "%s\r\n", tmp);
    }

    LOG(LEVEL_SIMPLE, "����״̬:             r-����  R-����  B-����  S-����  D-ɾ��\r\n");

#endif
}

