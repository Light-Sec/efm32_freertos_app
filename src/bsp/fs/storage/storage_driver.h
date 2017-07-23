#ifndef STORAGE_DRIVER_H
#define STORAGE_DRIVER_H

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include "string.h"
#include "app_error.h"

#ifdef __cplusplus
extern "C" {
#endif


//����ָ��
typedef int16_t (*write_byte_operation)(uint32_t *address, void const *data, uint32_t numBytes);
typedef int16_t (*read_byte_operation)(uint32_t *address, void *data, uint32_t numBytes);
typedef int16_t (*erase_page_operation)(uint32_t *startAddress);
typedef void     (*init_operation)(void);


//���⺯���ӿ�
bool storage_is_idle(void);

//��д�����������ӿڣ��̰߳�ȫ
int16_t emu_flash_write_safe(uint32_t *address, void const *data, uint32_t numBytes);
int16_t emu_flash_read_safe(uint32_t *address, void *data, uint32_t numBytes);
int16_t emu_flash_erase_page_safe(uint32_t *startAddress);
void     emu_flash_init(void);

#ifdef __cplusplus
}
#endif

#endif

