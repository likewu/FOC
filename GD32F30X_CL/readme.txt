using openocd read uid:
F:\xPacks\@xpack-dev-tools\openocd\0.12.0-2.1\.content\bin\openocd -s F:\xPacks\@xpack-dev-tools\openocd\0.12.0-2.1\.content\openocd\scripts -f interface/cmsis-dap.cfg -f target/stm32f3x.cfg -c "init" -c "mdw 0x1ffff7e8 3" -c "exit" 2>&1 | tail -n 1

F:\xPacks\@xpack-dev-tools\openocd\0.12.0-2.1\.content\bin\openocd -s F:\xPacks\@xpack-dev-tools\openocd\0.12.0-2.1\.content\openocd\scripts -f interface/stlink-v2.cfg -f target/stm32f1x.cfg -c "init" -c "mdw 0x1ffff7e8 3" -c "exit"

F:\xPacks\@xpack-dev-tools\openocd\0.12.0-2.1\.content\bin\openocd -s F:\xPacks\@xpack-dev-tools\openocd\0.12.0-2.1\.content\openocd\scripts -f interface/interface/esp_usb_jtag.cfg -f target/esp32s3.cfg