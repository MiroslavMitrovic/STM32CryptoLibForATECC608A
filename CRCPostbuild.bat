::SCRIPT FOR FILLING THE IMAGE AND PERFORMING THE CRC32 CALCULATION# 

@echo ON
::# Set the variables 
set IMAGE_NAME=.\Debug\CyberSec.srec
set IMAGE_START_ADDRESS=0x08000000
set IMAGE_END_ADDRESS=0x0800FFDC
::# Get the image data 
echo srec_info.exe %IMAGE_NAME%
::srec_cat.exe %IMAGE_NAME% -fill 0xFF 0x08040000 0x0805FFFC -crc32-b-e 0x0805FFFC  -o ReflowPreheater_crc.srec
srec_cat.exe %IMAGE_NAME% -fill 0xFF %IMAGE_START_ADDRESS% %IMAGE_END_ADDRESS% -crc32-b-e 0x0800FFDC  -o .\Debug\CyberSec_crc.srec

PAUSE