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




/* extra info���  */
//#define extra_info

/* fake sensor data */
#define fake_HR
//#define fake_BARO

/*sensor enable */
#define USE_ACC
#define USE_GYRO



/*set */
//����
#define sedentary

//̧������
//#define WatchHandUp

//�����㷨
//#define falling

//SHAKE
//#define shake

//˯��
//#define sleeping




#endif //__CONFIG_H__
