@echo off

cd ../script_A/

@echo ====================����һ=========================

del encrypt.exe
del encrypt_header.exe
del keys.txt

@echo 1.����encrypt.exe,encrypt_header.exe,keys.txt
copy ..\bootloader\encrypt\bin\encrypt.exe
copy ..\bootloader\encrypt\bin\encrypt_header.exe
copy ..\bootloader\encrypt\keys.txt

::pause;

@echo ====================�����=========================

del aes_keys.c
del ..\bootloader\util\aes_keys.c

@echo 1.����aes_keys.c......
encrypt.exe --create-bootloader-keys

@echo 2.�滻aes_keys.c......
@echo   ���keys.txt���޸ģ���Ҫ���±���bootloader......
copy aes_keys.c ..\bootloader\util\aes_keys.c

::pause;


@echo ====================������=========================

del application.hex
del application.bin
del bootloader.hex
del bootloader.bin

@echo 1.����application.hex��application.bin
copy ..\arm\obj\efm32_freertos_app.hex  application.hex
copy ..\arm\obj\efm32_freertos_app.bin  application.bin

@echo 2.����bootloader.hex��bootloader.bin
copy ..\arm\obj1\bootloader_EFM32GG230.hex bootloader.hex
copy ..\arm\obj1\bootloader_EFM32GG230.bin bootloader.bin

::pause;


@echo ====================������=========================

del application_encrypt.bin
del application_header.bin

@echo 1.����encrypt_application.bin......
encrypt.exe application.bin application_encrypt.bin

@echo 2.����application_header.bin......
encrypt_header.exe application.bin application_header.bin

::pause;


@echo ====================������=========================
commander convert bootloader.hex application_header.bin         --address 0x4000   --outfile  efm32_freertos_demo.hex

::pause;


@echo on