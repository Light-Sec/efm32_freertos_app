#ifndef BOOT_STORAGE_H
#define BOOT_STORAGE_H

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include "string.h"
#include "app_error.h"

#ifdef __cplusplus
extern "C" {
#endif

//�洢��غ궨��

//OTA FLASH�洢��header��Ϣ
#define HEADER_NOT_OTA_UPDATE                   (0xffffffff)
#define HEADER_OTA_UPDATE                       (0x55555555)

typedef struct
{
    uint32_t ota_update_flag;                   //OTA���±�־
    uint32_t ota_update_crc;                    //OTA����CRCֵ
    uint32_t ota_update_length;                 //OTA�����ļ���С
    uint32_t ota_update_version;                //OTA�����ļ��汾��
}ota_flash_header_t;




//���⺯���ӿ�



#ifdef __cplusplus
}
#endif

#endif

