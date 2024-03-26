#include "main.h"  
#include "stdio.h"
#include "Lin_buffer.h"
#include "lin_driver.h"
 
 
LIN_RxState       LIN_RxStateGet = BREAK_GET; 
LIN_ERROR_Code    ErrCode = FIFO_VOID;
uint8_t                data[8] = {0x11,0x01,0x34,0x34,0x56,0x34,0x78,0x67};
 
 
//LIN接收缓冲区
LIN_BUFFER  LIN_RxDataBuff = {
  .Head = 0,
  .Tail = 0,
  .Length = 0,
  .BufOutcnt = 0  
};
 
 
/**
  * @brief  LIN从机中断函数
  * @param  None
  * @retval None
  */
void LIN_MASTER_IRQHandler(void)
{
  uint8_t ReceiveData = 0;
  
  //LIN断开帧中断
  if ((USART_GetITStatus(USART1,UART_IT_LBD) == SET)){   //LIN断路检测中断  1 -- 有同步间隔段
    LIN_RingBUF_ClearRxMsg(&LIN_RxDataBuff);              //清空当前缓冲区
    USART1->SR;     //读取USART1->SR会导致接收缓冲区中的数据被移除或清空
    USART1->DR; 
    LIN_RxStateGet = SYNCH_GET;
    USART_ClearITPendingBit(USART1,UART_IT_LBD);         //清除LIN断路检测中断
    USART_ClearITPendingBit(USART1,UART_IT_RXNE);
    return;   
  }
  
  if (USART_GetITStatus(USART1,UART_IT_RXNE) == SET){            //LIN接收中断
    ReceiveData = USART_ReceiveData(USART1);                      //返回USARTx外设最近接收到的数据。
    if (USART_GetFlagStatus(USART1,UART_FLAG_FE) == RESET){      //帧错误标志  0 -- 没有检测到帧错误
      if ((ReceiveData==0x55)&&(LIN_RxStateGet==BREAK_GET)){      //处理无同步间隔信号的LIN数据                                    //初始化定时器值 类似喂狗
        LIN_RingBUF_ClearRxMsg(&LIN_RxDataBuff);                  //清空当前缓冲区
        USART1->SR;     
        USART1->DR;
        LIN_RxStateGet = SYNCH_GET;
        return;   
      }
        LIN_MasterRxMsg(ReceiveData);   //消息处理
    }
    USART_ClearITPendingBit(USART1,UART_IT_RXNE);
  }
}
 
 
/**
  * @brief  LIN从机接收处理函数
  * @param  Data: 串口消息
  * @retval None
  */
void LIN_MasterRxMsg(uint8_t Data)
{
  LIN_MSG* pLINMsg = LIN_RingBUF_GetMsg(&LIN_RxDataBuff);
  
  switch (LIN_RxStateGet){
    case BREAK_GET:      //同步间隔段
    {
    }
    break;
    
    case SYNCH_GET:      //同步段
    {
      if(Data != 0x55){  //判断是不是同步段
        ErrCode = SYNC_ERR;
        LIN_RxStateGet = BREAK_GET; 
      }else{
        pLINMsg->Sync = Data;
        LIN_RxStateGet = PID_GET; 
      } 
    }
    break;  
    
    case PID_GET:        //PID段
    {
      pLINMsg->FrameID = Data&0x3F;
      pLINMsg->PID = Data;
      uint8_t PID = LIN_GetPID(pLINMsg->FrameID);  //根据ID获取奇偶校验位 得到校验的PID
      
      if(PID == pLINMsg->PID){                     //判断PID是否正确  后续根据LDF定
        //根据判断是执行还是反馈 改变标志位
        if(pLINMsg->FrameID == 0x31){              // 1 -- 执行  即继续接收数据
          LIN_RxStateGet = MSG_GET; 
        }else if(pLINMsg->FrameID == 0x32){        // 2 -- 反馈  即向LIN总线发送消息
          LIN_Rx_data(pLINMsg->PID,data,sizeof(data)/sizeof(data[0]));   //反馈消息
          LIN_RxStateGet = BREAK_GET;
        }else{                                     // 3 -- 其他  即不执行也不反馈                       
          LIN_RingBUF_ClearRxMsg(&LIN_RxDataBuff); //清空当前缓冲区
          LIN_RxStateGet = BREAK_GET;
        }
      }else{   //PID校验不正确
        ErrCode = PID_ERR;
        LIN_RingBUF_ClearRxMsg(&LIN_RxDataBuff);   //清空当前缓冲区
        LIN_RxStateGet = BREAK_GET;       
      }                       
    }
    break;  
    
    case MSG_GET:       //数据段
    {
      pLINMsg->Data[pLINMsg->DataLen++] = Data;
      pLINMsg->Checksum = Data;
      LIN_RxStateGet = (pLINMsg->DataLen>=8)?CHECKSUM_GET:MSG_GET;  
    }
    break;  
    
    case CHECKSUM_GET:  //校验和段
    {
      pLINMsg->Checksum = Data;
      uint8_t Checksum = LIN_GetChecksum(pLINMsg->PID,pLINMsg->Data,pLINMsg->DataLen,1);     //获取校验和段
      if((Checksum+pLINMsg->Checksum) == 0xFF){            //判断校验和是否正确
        LIN_RingBUF_Write(&LIN_RxDataBuff);
      }else{
        ErrCode = FORMAT_ERR;
        LIN_RingBUF_ClearRxMsg(&LIN_RxDataBuff);           //清空当前缓冲区
      } 
      LIN_RxStateGet = BREAK_GET;   
    }
    break;
    
    default:            //其他
      LIN_RxStateGet = BREAK_GET; 
    break;
  }
}
 
 
/**
  * @brief  反馈消息给总线 
  * @param  PID：校验ID，pData：数据指针，Length：数据长度
  * @retval 无
  */
void LIN_Rx_data(uint8_t PID, uint8_t* pData,uint8_t DataLen)
{
  uint8_t Linbuffer[DataLen+1];                                //定义发送数组(数据+校验和)
  uint8_t Checksum = LIN_GetChecksum(PID,pData,DataLen,0);     //获取校验和段
  for (uint8_t i = 0; i < DataLen; i++)                        //存DataLen个字节数据段
  {     
    Linbuffer[i] = *(pData + i);    
  }
  Linbuffer[DataLen] = Checksum;                   //校验和
  LIN_SendBytes(USART1, Linbuffer ,DataLen+1);     //发送从机数据
}
 
 
/**
  * @brief  LIN同步间隔段发送    
  * @param  USARTx：串口号
  * @retval 无
  */
void LIN_SendBreak(USART_TypeDef* USARTx)
{
  USART_SendBreak(USARTx);
}
 
 
/**
  * @brief  LIN发送字节 
  * @param  USARTx：串口号，pData：数据指针，Length：数据长度
  * @retval 无
  */
void LIN_SendBytes(USART_TypeDef* USARTx,uint8_t* pData,uint8_t DataLen)
{
  for (uint8_t i = 0; i < DataLen; i++){
    USART_SendData(USARTx,*pData++);
    while (USART_GetFlagStatus(USARTx,UART_FLAG_TXE) == RESET);  //传输数据寄存器空标志
  }
  while (USART_GetFlagStatus(USARTx,UART_FLAG_TC) == RESET);   //传输完成标志
}
 
 
/**
  * @brief  LIN协议规定校验和长度为1个字节，获取校验和 
  * @param  PID：校验ID，pData：数据指针，DataLen：数据长度，flag ：发送0 接收1 
  * @retval 累加校验和
  */
uint8_t LIN_GetChecksum(uint8_t PID, uint8_t* pData,uint8_t DataLen,uint8_t flag)        
{  
    uint16_t CheckSum = 0;  
    //FrameID为3C 3D的PID为3C 7D
    if((PID!=0x3C)&&(PID!=0x7D)){    //诊断帧只能使用标准校验和，标准校验和不包含PID 只校验数据段                  
      CheckSum = PID;         
    }
    for(uint8_t i = 0; i < DataLen; i++){
      CheckSum += pData[i];     
      if (CheckSum > 0xFF){
        CheckSum -= 0xFF;  
      }
    }
    
    if(flag == 0){
      return (~CheckSum) & 0xFF;  //发送方需要取反
    }else{
      return CheckSum & 0xFF;     //接收方不需要
    }
}
 
 
/**
  * @brief LIN_PID校验函数    
  * @param ID(FrameID)：帧ID(0 ~ 63) 
  * P0(bit6) =   ID0 ^ ID1 ^ ID2 ^ ID4   <==>  (偶校验：操作数中1的个数为偶数，校验位为0，1的个数为奇数校验位为1)
  * P1(bit7) = ~(ID1 ^ ID3 ^ ID4 ^ ID5)  <==>  (奇校验：操作数中1的个数为奇数，校验位为0，1的个数为偶数校验位为1)
  * @retval 返回PID
  */
uint8_t LIN_GetPID(uint8_t ID)  
{
  uint8_t PID = 0,P0 = 0,P1 = 0;  
  P0 = (((ID>>0)^(ID>>1)^(ID>>2)^(ID>>4))&0x01)<<6; //偶校验位                
  P1 = ((~((ID>>1)^(ID>>3)^(ID>>4)^(ID>>5)))&0x01)<<7; //奇校验位 
  PID = (ID|P0|P1); 
  return PID;   
}
