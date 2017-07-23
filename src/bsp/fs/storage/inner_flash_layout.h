#ifndef INNER_FLASH_LAYOUT_H
#define INNER_FLASH_LAYOUT_H

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include "string.h"
#include "app_error.h"

#include "em_device.h"

#ifdef __cplusplus
extern "C" {
#endif


//ҳʵ�ʴ�С
#if defined(_EFM32_GIANT_FAMILY)
#define PAGE_SIZE                       ((FLASH_SIZE >= 0x80000) ? 0x1000 : 0x800)
#else
#pragma error "Unknown page size"
#endif

//����ҳ�Ŷ�Ӧ��ʵ�ʵ�ַ
#define ST_PAGE_ADDR(PAGE_NUM)          (PAGE_NUM*PAGE_SIZE)


//ҳ��125~128
//����������ڲ��ļ��Ĵ洢�ռ䣬ʵ����:��ST_END_PAGE_NUM(�ߵ�ַ)��ST_START_PAGE_NUM(�͵�ַ)����
#define ST_FILE_REGION_SIZE             (4*4*1024)

//ҳ��121~124
//������ʷ��¼�Ĵ洢�ռ䣬ʵ����:��ST_RAW_REGION_START_ADDRESS(�͵�ַ)��ST_RAW_REGION_END_ADDRESS(�ߵ�ַ)����
#define ST_RAW_REGION_SIZE              (4*4*1024)
#define ST_RAW_REGION_END_ADDRESS       (FLASH_SIZE - ST_FILE_REGION_SIZE)
#define ST_RAW_REGION_START_ADDRESS     (ST_RAW_REGION_END_ADDRESS - ST_RAW_REGION_SIZE)


#define DATA_ISOLATE_RESERVE_SIZE       (0*4*1024)
#define PROGRAM_REGION_END_ADDRESS      (ST_RAW_REGION_START_ADDRESS - DATA_ISOLATE_RESERVE_SIZE)

//����ota�Ĵ洢�ռ�
#define WHOLE_BOOT_MAX_LIMIT_SIZE       (4*4*1024)
#define WHOLE_APP_MAX_LIMIT_SIZE        ((PROGRAM_REGION_END_ADDRESS - WHOLE_BOOT_MAX_LIMIT_SIZE)/2)

//swap:ҳ��63~120
#define SWAP_REGION_END_ADDRESS         (PROGRAM_REGION_END_ADDRESS)
#define SWAP_REGION_START_ADDRESS       (SWAP_REGION_END_ADDRESS - WHOLE_APP_MAX_LIMIT_SIZE)

//app:ҳ��5~62
#define APP_REGION_END_ADDRESS          (SWAP_REGION_START_ADDRESS)
#define APP_REGION_START_ADDRESS        (APP_REGION_END_ADDRESS - WHOLE_APP_MAX_LIMIT_SIZE)

//bootloader:ҳ��0~4
#define BOOT_REGION_START_ADDRESS       (0)
#define BOOT_REGION_END_ADDRESS         (BOOT_REGION_START_ADDRESS + WHOLE_BOOT_MAX_LIMIT_SIZE)




//���⺯���ӿ�


#ifdef __cplusplus
}
#endif

#endif

