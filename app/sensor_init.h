#ifndef __SENSOR_INIT_H__
#define __SENSOR_INIT_H__

#include "cwm_lib_api.h"

#if LSM6DSOW
#define Device_ID1     0x6a
#define Device_ID2     0x6b
#define whomi_Address  0x0f
#define sensor_whomi   0x6c
#endif
#if ICM_42607
#define Device_ID1     0x68
#define Device_ID2     0x69
#define whomi_Address  0x75
#define sensor_whomi   0x61
#endif

void lsm6dso_init(void);
void lsm6dso_getfifo(void);
uint8_t readwhomi(uint16_t Device_ID,uint16_t Address);
int cheakwhomi(void);
void test_process(void);


#endif //__SENSOR_INIT_H__
