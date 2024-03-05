#include "visualscope.h"

void CRC16( uint8_t *array, uint8_t *rcvbuf, uint16_t len )
{
	uint16_t x,y,crc;
	
	crc = 0xffff;
	
	if( len <= 0 ) crc = 0;
	else
	{
		for( x = 0; x < len; x++ )
		{
			crc ^= array[x];
			for( y=0; y <8; y++ )
			{
				if( crc&1 ) crc = (crc>>1)^0xa001;
				else crc = crc>>1;
			}
		}
	}
	
	rcvbuf[0] = crc;
	rcvbuf[1] = crc>>8;
}

