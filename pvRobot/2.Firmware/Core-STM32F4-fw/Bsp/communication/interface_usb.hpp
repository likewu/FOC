#ifndef __INTERFACE_USB_HPP
#define __INTERFACE_USB_HPP

#ifdef __cplusplus

#include "fibre/protocol.hpp"

extern StreamSink *usbStreamOutputPtr;

extern "C" {
#endif

#include <cmsis_os.h>
#include <stdint.h>

typedef struct
{
    uint32_t rx_cnt;
    uint32_t tx_cnt;
    uint32_t tx_overrun_cnt;
} USBStats_t;

extern USBStats_t usb_stats_;

#define ODRIVE_IN_EP                                0x83  /* EP3 IN: ODrive device TX endpoint */
#define ODRIVE_OUT_EP                               0x03  /* EP3 OUT: ODrive device RX endpoint */

void usb_rx_process_packet(uint8_t *buf, uint32_t len, uint8_t endpoint_pair);
void StartUsbServer(void);

#ifdef __cplusplus
}
#endif

#endif // __INTERFACE_USB_HPP
