clc
HALF_SAMPLE_N = 500;
AMP = 65535;%幅度
t=linspace(0,3.1415926,HALF_SAMPLE_N);
uo=(power(sin(t),1));%次数
uo=abs(uo);
y=uo*AMP;
y=round(y);
% 归一化
% y=1000*y/AMP;
% y=round(y);
% --------------------------------- 写入文件开始 -------------------------------------
fp=fopen('E:\study\通用半桥电路及程序\程序\PFC\User\sample.c','wt');
fprintf(fp,'#include "sample.h"\n',HALF_SAMPLE_N);
fprintf(fp,'// 采样值最大为 0xFFFF\n',HALF_SAMPLE_N);% 说明文字
fprintf(fp,'const unsigned short sample[HALF_SAMPLE_N]=\n{');
for j=0:(HALF_SAMPLE_N/10-1)   
    fprintf(fp,'\n\t');
    for i=0:9
        fprintf(fp,'0x%04X,',y(j*10+i+1));
    end
end
fprintf(fp,'\n};');
fclose(fp);
fp=fopen('E:\study\通用半桥电路及程序\程序\PFC\User\sample.h','wt');
fprintf(fp,'#ifndef _SAMPLE_H\n#define _SAMPLE_H\n');
fprintf(fp,'#define HALF_SAMPLE_N (%d)\n',HALF_SAMPLE_N);
fprintf(fp,'extern const unsigned short sample[HALF_SAMPLE_N];\n#endif');
fclose(fp);
% --------------------------------- 写入文件结束 -------------------------------------
x=[1:HALF_SAMPLE_N*2];
plot(x,[y -y],'*-',x,0,'-');
display('succeed');
