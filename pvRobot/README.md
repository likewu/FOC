https://kkgithub.com/samuelsadok/fibre-cpp

F:\STM32Cube\Repository\STM32Cube_FW_F4_V1.27.0\Drivers\CMSIS\DSP\Include\arm_math.h
F:\STM32Cube\Repository\STM32Cube_FW_F4_V1.27.0\Drivers\CMSIS\Lib\GCC\libarm_cortexM4lf_math.a

目前的归零和定位设计非常简洁和高效，但FOC的无感模式需要硬件电路上支持，忍不住考虑是1 否可以移除FOC的无感模式
2 目前使用了FOC的力矩变大作为归零依据
3 到达机械零点时绝对值编码器的读取值应该也是有所体现
4 是否可能以这个做为规律依据
5 产生位置的相位差异应该等效于力矩变化量
6 这样FOC的力矩控制可能不是必要的，这样可能可以简化FOC和电路的一些部分
7 当然 使用FOC的力矩模式，进行动作学习的那个功能也是依赖FOC的力矩模式
8 但不妨可考虑把静态情况下的位置值变化作为跟随依据