//--------------------------------------------------------------
// File     : stm32_ub_lin_master.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __LIN_PRTOCOL_H
#define __LIN_PRTOCOL_H



//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
//#include "stm32f4xx.h"
//#include "stm32f4xx_gpio.h"
//#include "stm32f4xx_rcc.h"
//#include "stm32f4xx_usart.h"
//#include "misc.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_conf.h"
#include "main.h"

extern uint8_t btn;
extern uint8_t checker_1;
extern uint8_t checker_2;
extern uint8_t checker_3;

//--------------------------------------------------------------
// Global Defines
//--------------------------------------------------------------
#define  LIN_SYNC_DATA               0x55  // SyncField (do not change)
#define  LIN_MAX_DATA                   8  // max 8 databytes



//--------------------------------------------------------------
// Defines for the transceiver (do not change !!)
//--------------------------------------------------------------
#define  LIN_POWERON_DELAY          10000  // Pause at PowerOn     (ca. 10ms)
#define  LIN_AKTIV_DELAY               50  // Pause for Transceiver (ca. 50us)



//--------------------------------------------------------------
// breaks (be careful when changing !!)
//--------------------------------------------------------------
#define  LIN_INTER_FRAME_DELAY      10000  // Pause (Frame->Frame)   (ca. 10ms)
#define  LIN_FRAME_RESPONSE_DELAY    2000  // Pause (Header->Data)   (ca.  2ms)
#define  LIN_BREAKFIELD_DELAY        4000  // Pause (Breakfield)     (ca.  4ms)
#define  LIN_DATA_BYTE_DELAY         1000  // Pause (Data->Data)     (ca.  1ms)
#define  LIN_RX_TIMEOUT_CNT         30000  // Receive Timeout        (ca.  5ms)



//--------------------------------------------------------------
// LIN frame Struct
//--------------------------------------------------------------
typedef struct {
  uint8_t frame_id;              // unique ID number from the frame
  uint8_t data_len;              // number of data bytes
  uint8_t data[LIN_MAX_DATA];    // data fields
}LIN_FRAME_t;



//--------------------------------------------------------------
// Master Mode
//--------------------------------------------------------------
typedef enum {
  RECEIVE_DATA =0,  // receive data
  RECEIVE_CRC,      // receive CRC
  SEND_DATA         // send data
}LIN_MODE_t;



//--------------------------------------------------------------
// Master struct
//--------------------------------------------------------------
typedef struct {
  LIN_MODE_t mode;  // current Mode
  uint8_t data_ptr; // data pointer
  uint8_t crc;      // checksum
}LIN_MASTER_t;



//--------------------------------------------------------------
// error messages
//--------------------------------------------------------------
typedef enum {
  LIN_OK  = 0,   // no error
  LIN_WRONG_LEN, // wrong number of data
  LIN_RX_EMPTY,  // no frame received
  LIN_WRONG_CRC  // Checksum wrong
}LIN_ERR_t;



//--------------------------------------------------------------
// Global Functions
//--------------------------------------------------------------
//void UB_LIN_Master_Init(void);
//LIN_ERR_t UB_LIN_SendData(LIN_FRAME_t *frame);
//LIN_ERR_t UB_LIN_ReceiveData(LIN_FRAME_t *frame);
void led_testing();
LIN_ERR_t UB_LIN_SendData(LIN_FRAME_t *frame, UART_HandleTypeDef *huart);
LIN_ERR_t UB_LIN_ReceiveData(LIN_FRAME_t *frame, UART_HandleTypeDef *huart);
uint8_t p_LIN_makeChecksum(LIN_FRAME_t *frame);
void p_LIN_wait_us(uint32_t n);



//--------------------------------------------------------------
#endif // __LIN_PRTOCOL_H
