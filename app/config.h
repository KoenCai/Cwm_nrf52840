#ifndef __CONFIG_H__
#define __CONFIG_H__

#define aikemu52832

#define IMU_SCL_PIN    NRF_GPIO_PIN_MAP(1,14)
#define IMU_SDA_PIN    NRF_GPIO_PIN_MAP(1,13)

/*sensor选择*/
#define LSM6DSOW           (1)
#define ICM_42607          (0)

/*sensor enable */
#define USE_ACC						 (1)
#define USE_GYRO					 (1)
#define ACC_RANGE					 (16)
#define GYRO_RANGE				 (2000)

/*sensor input mode*/
#define FIFO_MODE          (1)
#define CWM_FIFO_COUNT     (50)
#define SEN_FIFO_COUNT     (10)

/* fake sensor data */
#define fake_HR            (0)
#define fake_BARO          (0)
#define offbody_state      (1)

/*Set Cwm_ODR*/
#define CWM_ODR            (50)  				//（25（50（100 

/* Debug */
#define TEST_API           (1)
#define TEST_HOOK          (0)
#define CWM_DEBUG          (0) 
#define CHEAK_WHOMI        (1) 

/* extra info输出  */
#define extra_info         (0)
#define processTime        (0)
#define DataTime           (0)

/*set activity mode*/									  //activity mode
#define NORMAL    				 (1)  				//一般计步
#define TREADMILL 				 (0)  				//跑步机
#define OUTDOOR_RUNNING		 (0)  				//户外健走/跑步
#define CLIMBING_STAIRS		 (0)  				//爬楼梯
#define HIKING		         (0)  				//登山
#define INDOOR_RUNNING		 (0)  				//室内跑步
#define INDOOR_SWIMMING		 (0)  				//室内游泳
#define HYBRID_ACTIVITY    (0)  				//室内外运动
#define FITNESS_PEDOMETER  (0)  				//防误触计步
#define INDOOR_SWIMMING	   (0)  				//室内游泳
#define OPENWATER_SWIMMING (0)  				//开放水域游泳
#define OUTDOOR_BIKING		 (0)  				//户外骑行
#define BOXING		 			   (0)  				//拳击
#define DUMBBELL		 		   (0)  				//哑铃
#define SKIPPING		 		   (0)  				//跳绳
#define ELLIPTICAL		     (0)  				//椭圆机
#define ROWING		         (0)  				//划船机

/*set inactivity mode*/
#define INACTIVITY_MODE    (1)  				//（0:关闭（1:久坐（2:睡眠（3:久坐+小睡（4:小睡（5:睡眠+有效站立

/*add function*/
#define WatchHandUp        (0) 				//抬腕亮屏
#define Folling        	   (0) 				//跌倒算法
#define Lowpowermode			 (0)				//开启低功耗模式



#endif //__CONFIG_H__
