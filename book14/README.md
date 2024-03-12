[分分钟带你入门无刷电机控制——P-NUCLEO-IHM001套件评测](https://www.21ic.com/evm/evaluate/MCU/201601/658578.htm)
[直流有刷电机驱动基于STM32F302R8+X-NUCLEO-IHM07M1（一）](https://blog.csdn.net/weixin_42650162/article/details/126613529?spm=1001.2014.3001.5502)
[直流无刷电机开环调速基于STM32F302R8+X-NUCLEO-IHM07M1（一）](https://blog.csdn.net/weixin_42650162/article/details/127708668)


https://blog.csdn.net/lone5moon/article/details/117790199
xpm install @xpack-dev-tools/openocd@latest --verbose
F:\xPacks\@xpack-dev-tools\openocd\0.12.0-2.1\.content\bin\openocd.exe -f interface\cmsis-dap.cfg -f target\stm32f4x.cfg
GDB连接设置改为远程GDB服务器-->端口保持默认-->调试探头改为OpenOCD-->取消勾选实时表达式，否则无法成功调试-->最后开始Debug调试。