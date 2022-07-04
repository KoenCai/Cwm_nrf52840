#ifndef __CWM_LIB_API_H__
#define __CWM_LIB_API_H__

#include "board_nordic.h"
#include "cwm_lib.h"
#include "config.h"



/* Activity Mode */
#define CWM_ACTIVITY_NORMAL												1001
#define CWM_ACTIVITY_TREADMILL										1002
#define CWM_ACTIVITY_OUTDOOR_RUNNING							1003
#define CWM_ACTIVITY_CLIMBING_STAIRS							1004
#define CWM_ACTIVITY_HIKING												1005
#define CWM_ACTIVITY_INDOOR_RUNNING								1006
#define CWM_ACTIVITY_HYBRID_ACTIVITY							1007
#define CWM_ACTIVITY_FITNESS_PEDOMETER						1008

#define CWM_ACTIVITY_INDOOR_SWIMMING							2001
#define CWM_ACTIVITY_OPENWATER_SWIMMING						2002

#define CWM_ACTIVITY_OUTDOOR_BIKING								3001
#define CWM_ACTIVITY_FREETRAINING									5001
#define CWM_ACTIVITY_WORKOUT_MACHINE							6001

/* Inactivity Mode */
#define CWM_INACTIVITY_SEDENTARY										1
#define CWM_INACTIVITY_SLEEPING											2
#define CWM_INACTIVITY_SEDENTARY_NAP								3
#define CWM_INACTIVITY_NAP													4
#define CWM_INACTIVITY_SLEEPING_STAND								5   


/* Workout_machine Mode */
#define 	wm_boxing                                 635511698  				//È­»÷
#define 	wm_dumbbell                               180110491  				//ÑÆÁå
#define 	wm_skipping                               2087076015  			//ÌøÉþ
#define 	wm_elliptical                             1332114369   			//ÍÖÔ²»ú
#define 	wm_rowing                                 398202153  				//»®´¬»ú

void cwm_init(void);
void DataInput(int SensorTyp,float data_x,float data_y,float data_z);
void Cwm_fifo_start(int SensorTyp,int DTms);
void Cwm_fifo_end(int SensorTyp);
void anymotion(void);
void test_api(void);

#endif //__CWM_LIB_API_H__
