#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "cwm_lib.h"
#include "cwm_lib_api.h"
#include "config.h"
#include "simulate_signal.h"
#include "board_nordic.h"
#include "sensor_init.h"
#include "lsm6dso_reg.h"


#define Device_ID      0x6b
#define whomi_Address  0x0f


int main(void)
{
    board_init();	
    nrf_delay_ms(10);
	
		//readwhomi(Device_ID,whomi_Address);
		lsm6dso_init();		
		cwm_init();

  while(1)
  {	
		lsm6dso_getfifo();		
	}

}


