#ifndef __CONFIG_H__
#define __CONFIG_H__

#define aikemu52832

#define IMU_SCL_PIN    NRF_GPIO_PIN_MAP(1,14)
#define IMU_SDA_PIN    NRF_GPIO_PIN_MAP(1,13)

/*sensorѡ��*/
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
#define CWM_ODR            (50)  				//��25��50��100 

/* Debug */
#define TEST_API           (1)
#define TEST_HOOK          (0)
#define CWM_DEBUG          (0) 
#define CHEAK_WHOMI        (1) 

/* extra info���  */
#define extra_info         (0)
#define processTime        (0)
#define DataTime           (0)

/*set activity mode*/									  //activity mode
#define NORMAL    				 (1)  				//һ��Ʋ�
#define TREADMILL 				 (0)  				//�ܲ���
#define OUTDOOR_RUNNING		 (0)  				//���⽡��/�ܲ�
#define CLIMBING_STAIRS		 (0)  				//��¥��
#define HIKING		         (0)  				//��ɽ
#define INDOOR_RUNNING		 (0)  				//�����ܲ�
#define INDOOR_SWIMMING		 (0)  				//������Ӿ
#define HYBRID_ACTIVITY    (0)  				//�������˶�
#define FITNESS_PEDOMETER  (0)  				//���󴥼Ʋ�
#define INDOOR_SWIMMING	   (0)  				//������Ӿ
#define OPENWATER_SWIMMING (0)  				//����ˮ����Ӿ
#define OUTDOOR_BIKING		 (0)  				//��������
#define BOXING		 			   (0)  				//ȭ��
#define DUMBBELL		 		   (0)  				//����
#define SKIPPING		 		   (0)  				//����
#define ELLIPTICAL		     (0)  				//��Բ��
#define ROWING		         (0)  				//������

/*set inactivity mode*/
#define INACTIVITY_MODE    (1)  				//��0:�رգ�1:������2:˯�ߣ�3:����+С˯��4:С˯��5:˯��+��Чվ��

/*add function*/
#define WatchHandUp        (0) 				//̧������
#define Folling        	   (0) 				//�����㷨
#define Lowpowermode			 (0)				//�����͹���ģʽ



#endif //__CONFIG_H__
