#ifndef __CONFIG_H__
#define __CONFIG_H__

#define aikemu52832

/*sensorѡ��*/
#define LSM6DSOW
//#define ICM_42607

/*sensor input mode*/
#define FIFO_mode

/* set activity mode */
#define CWM_ACTIVITY_NORMAL													1001
//#define CWM_ACTIVITY_TREADMILL										1002
//#define CWM_ACTIVITY_OUTDOOR_RUNNING							1003
//#define CWM_ACTIVITY_CLIMBING_STAIRS							1004
//#define CWM_ACTIVITY_HIKING												1005
//#define CWM_ACTIVITY_INDOOR_RUNNING								1006
//#define CWM_ACTIVITY_INDOOR_SWIMMING							2001
//#define CWM_ACTIVITY_OUTDOOR_BIKING								3001
//#define CWM_ACTIVITY_FREETRAINING									5001
//#define CWM_ACTIVITY_WORKOUT_MACHINE							6001

#if 1
#define AR_Alert		//AR��ʱ���˶����
#endif

#if 0
#define extra_info		//extra info��� 
#endif


#if 0
#define fake_HR
#define fake_BARO
#endif

#if 0
//#define WatchHandUp      //̧������
#endif


#if 0
#define nothing 						//��������̬�㷨
#define inactivity_mode 0
#endif

#if 1

#define sedentary 					//����
#define inactivity_mode 1
#endif

#if 0

#define sleeping						//˯��
#define inactivity_mode 2
#endif

#if 0

#define Sedentary_and_Nap  //������˯��
#define inactivity_mode 3
#endif

#if 0

#define Nap  								//С˯
#define inactivity_mode 4
#endif


#endif //__CONFIG_H__
