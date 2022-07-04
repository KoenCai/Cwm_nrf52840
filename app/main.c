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


extern uint16_t LPM_FLAG;
extern uint16_t Sixty_Sec_FLAG;


int main(void)
{
	board_init();	
	nrf_delay_ms(10);
#if CHEAK_WHOMI
	cheakwhomi();
#endif		
	lsm6dso_init();	
	cwm_init();
  while(1)
  {			
#if TEST_API		
		test_api();
#else
		//button_process();	
		if(LPM_FLAG == 1 && Sixty_Sec_FLAG == 1)
		{			
				CWM_process();
				Sixty_Sec_FLAG = 0;
		}
		else if(LPM_FLAG==0)		
			lsm6dso_getfifo();
#endif			
	}

}


