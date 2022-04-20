#ifndef __SENSOR_INIT_H__
#define __SENSOR_INIT_H__

#include "cwm_lib_api.h"


void lsm6dso_init(void);
void lsm6dso_getfifo(void);
void readwhomi(uint16_t ID,uint16_t Address);



#endif //__SENSOR_INIT_H__
