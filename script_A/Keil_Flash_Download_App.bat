@echo off

cd ../script_A/


@echo ����SWAP...
commander device pageerase --range 0x3e000:+0x3a000

JLink.exe -Device EFM32GG230F512 -CommanderScript Keil_Flash_download_APP.jlink

::@echo ��дAPP...
::commander flash --address 0x4000 application_header.bin

::@echo ��λ...
::commander device reset


@echo ==============  ��д�ɹ���==============

pause;

@echo on