clc
HALF_SAMPLE_N = 500;
AMP = 65535;%����
t=linspace(0,3.1415926,HALF_SAMPLE_N);
uo=(power(sin(t),1));%����
uo=abs(uo);
y=uo*AMP;
y=round(y);
% ��һ��
% y=1000*y/AMP;
% y=round(y);
% --------------------------------- д���ļ���ʼ -------------------------------------
fp=fopen('E:\study\ͨ�ð��ŵ�·������\����\PFC\User\sample.c','wt');
fprintf(fp,'#include "sample.h"\n',HALF_SAMPLE_N);
fprintf(fp,'// ����ֵ���Ϊ 0xFFFF\n',HALF_SAMPLE_N);% ˵������
fprintf(fp,'const unsigned short sample[HALF_SAMPLE_N]=\n{');
for j=0:(HALF_SAMPLE_N/10-1)   
    fprintf(fp,'\n\t');
    for i=0:9
        fprintf(fp,'0x%04X,',y(j*10+i+1));
    end
end
fprintf(fp,'\n};');
fclose(fp);
fp=fopen('E:\study\ͨ�ð��ŵ�·������\����\PFC\User\sample.h','wt');
fprintf(fp,'#ifndef _SAMPLE_H\n#define _SAMPLE_H\n');
fprintf(fp,'#define HALF_SAMPLE_N (%d)\n',HALF_SAMPLE_N);
fprintf(fp,'extern const unsigned short sample[HALF_SAMPLE_N];\n#endif');
fclose(fp);
% --------------------------------- д���ļ����� -------------------------------------
x=[1:HALF_SAMPLE_N*2];
plot(x,[y -y],'*-',x,0,'-');
display('succeed');
