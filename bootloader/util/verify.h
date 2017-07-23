/**************************************************************************//**
 * @file verify.h
 * @brief Verification of firmware validity
 * @author Silicon Labs
 * @version 1.04
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/
#ifndef _VERIFY_H_
#define _VERIFY_H_

#include "ramfunc.h"

#include "boot_storage.h"
#include "boot_bootloader_config.h"

#include "storage_driver.h"


//���⺯���ӿ�
bool verifyFirmware(uint32_t firmwareAddress);
bool verifyActiveFirmware(void);
bool verifyTempStorage(void);
bool isFirmwareValid(void);
void markFirmwareAsVerified(void);
void markFirmwareAsDeleted(void);
void markTempAsVerified(void);
bool isTempStorageValid(void);

bool verifyTempStorageOtaCrc(void);
bool isTempStorageOtaFlag(void);
#ifndef USE_BOOTLOADER
/******************************************************************************
 * @brief boot_set_ota_flash_header
 * �ýӿ����ⲿ���ã�����֮ǰ��Ҫ��֤fwHeader->header�Ѿ���������
 *****************************************************************************/
void boot_set_ota_flash_header(const write_byte_operation write_operation, const ota_flash_header_t *header)
{
    FirmwareHeader *fwHeader = (FirmwareHeader *)TEMP_START_ADDRESS;

    if(NULL != write_operation)
    {
        write_operation((uint32_t *)(&fwHeader->header), (uint8_t *)header, sizeof(ota_flash_header_t));
    }

}
#endif

#endif
