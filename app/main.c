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



//int main(void)
//{
//	board_init();
//	lsm6dso_init();
//	cwm_init();
//	

//	
//	while (1)
//	{									
//			
//		lsm6dso_getfifo();		
//		
//	}
//}  




/* Variables Defines */
static const char* REQ_SENS_NAME[] = {"acc", "gyro", "mag", "baro", "temp", "hr", "gnss", "ob", "oc", "accany"};
static int m_req_sensor_state = 0;

/* Function Implementations */
int getReqSensor(int index) {
    if (m_req_sensor_state & (1<<index))
        return 1;
    return 0;
}

typedef union{
  int16_t i16bit[3];
  uint8_t u8bit[6];
} axis3bit16_t;

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

static axis3bit16_t data_raw_acceleration;
static axis3bit16_t data_raw_angular_rate;
static float acceleration_mg[3];
static float angular_rate_mdps[3];
static uint8_t whoamI, rst;

/* Extern variables ----------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
static int32_t platform_write(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len);
static int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len);
static void platform_delay(uint32_t ms);
static void platform_init(void);

//CustomSensorData csd = {0};
float acc_in[3] = {0};
float gyr_in[3] = {0};


//FIFO_COUNT
#define SIGNAL_SIMULATE_FIFO_COUNT 10


void CWM_AP_SensorListen(pSensorEVT_t sensorEVT)
{
	switch(sensorEVT -> sensorType)
	{
        case IDX_REQUEST_SENSOR:
            if (sensorEVT->fData[2] != 0)
                m_req_sensor_state |= (1<<(int)sensorEVT->fData[1]);
            else
                m_req_sensor_state &= ~(1<<(int)sensorEVT->fData[1]);

            CWM_OS_dbgPrintf("req_sensor input_sensor: %s(%.1f), switch: %.1f\n",\
                REQ_SENS_NAME[(int)sensorEVT->fData[1]], sensorEVT->fData[1], sensorEVT->fData[2]);
            CWM_OS_dbgPrintf("input_sensor now: acc=%d gyro=%d mag=%d baro=%d temp=%d hr=%d gnss=%d ob=%d oc=%d accany=%d\n",\
                getReqSensor(0), getReqSensor(1), getReqSensor(2), getReqSensor(3), getReqSensor(4),\
                getReqSensor(5), getReqSensor(6), getReqSensor(7), getReqSensor(8), getReqSensor(9));
        break;
        case IDX_ALGO_SEDENTARY:
            CWM_OS_dbgPrintf("%s: %.4f, %.4f, %.4f\n", "sedentary",
                                                       sensorEVT->fData[0],
                                                       sensorEVT->fData[1],
                                                       sensorEVT->fData[2]);
        break;
        case IDX_ACCEL:
            CWM_OS_dbgPrintf("%s: %.4f, %.4f, %.4f\n", "accel",
                                                       sensorEVT->fData[0],
                                                       sensorEVT->fData[1],
                                                       sensorEVT->fData[2]);
        break;
        case IDX_GYRO:
            CWM_OS_dbgPrintf("%s: %.4f, %.4f, %.4f\n", "gyro",
                                                       sensorEVT->fData[0],
                                                       sensorEVT->fData[1],
                                                       sensorEVT->fData[2]);
        break;
        case IDX_MAG:
            CWM_OS_dbgPrintf("%s: %.4f, %.4f, %.4f,%.4f\n", "mag",
                                                       sensorEVT->fData[0],
                                                       sensorEVT->fData[1],
                                                       sensorEVT->fData[2],
                                                       sensorEVT->fData[3]);
        break;
        case IDX_BARO:
            CWM_OS_dbgPrintf("%s: %.4f\n", "baro",
                                           sensorEVT->fData[0]);
        break;
        case IDX_TEMP:
              CWM_OS_dbgPrintf("%s: %.4f\n", "temp",
                                           sensorEVT->fData[0]);
        break;
        case IDX_HEARTRATE:
            CWM_OS_dbgPrintf("%s: %.4f, %.4f\n", "heart rate",
                                               sensorEVT->fData[0],
                                               sensorEVT->fData[1]);
        break;
        case IDX_GNSS:
            CWM_OS_dbgPrintf("%s: %.4f, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f\n", "gnss",
                                                       sensorEVT->fData[0],
                                                       sensorEVT->fData[1],
                                                       sensorEVT->fData[2],
                                                       sensorEVT->fData[3],
                                                       sensorEVT->fData[4],
                                                       sensorEVT->fData[5],
                                                       sensorEVT->fData[6],
                                                       sensorEVT->fData[7],
                                                       sensorEVT->fData[8]);
        break;
        case IDX_OFFBODY_DETECT:
            CWM_OS_dbgPrintf("%s: %.4f, %.4f, %.4f\n", "offbody",
                                                       sensorEVT->fData[0],
                                                       sensorEVT->fData[1],
                                                       sensorEVT->fData[2]);
        break;
        case IDX_ALGO_WATCH_HANDUP:
            CWM_OS_dbgPrintf("%s: %.4f, %.4f, %.4f\n", "watch handup",
                                                       sensorEVT->fData[0],
                                                       sensorEVT->fData[1],
                                                       sensorEVT->fData[2]);
        break;
        case IDX_ALGO_SLEEP:
            CWM_OS_dbgPrintf("%s: %.4f, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f\n", "sleep",
                                                       sensorEVT->fData[0],
                                                       sensorEVT->fData[1],
                                                       sensorEVT->fData[2],
                                                       sensorEVT->fData[3],
                                                       sensorEVT->fData[4],
                                                       sensorEVT->fData[5],
                                                       sensorEVT->fData[6]);
        break;

        case IDX_ALGO_SHAKE:
            CWM_OS_dbgPrintf("%s: %.4f, %.4f\n", "shake",
                                                       sensorEVT->fData[0],
                                                       sensorEVT->fData[1]);
        break;
        case IDX_ALGO_ACTIVITY_OUTPUT:
            CWM_OS_dbgPrintf("%s: %.4f, %.4f, %.4f, %.4f, "\
                                  "%.4f, %.4f, %.4f, %.4f, "\
                                  "%.4f, %.4f, %.4f, %.4f, "\
                                  "%.4f, %.4f, %.4f, %.4f\n", "activity output",
                                                       sensorEVT->fData[0],
                                                       sensorEVT->fData[1],
                                                       sensorEVT->fData[2],
                                                       sensorEVT->fData[3],
                                                       sensorEVT->fData[4],
                                                       sensorEVT->fData[5],
                                                       sensorEVT->fData[6],
                                                       sensorEVT->fData[7],
                                                       sensorEVT->fData[8],
                                                       sensorEVT->fData[9],
                                                       sensorEVT->fData[10],
                                                       sensorEVT->fData[11],
                                                       sensorEVT->fData[12],
                                                       sensorEVT->fData[13],
                                                       sensorEVT->fData[14],
                                                       sensorEVT->fData[15]);
        break;
        case IDX_ALGO_ANY_MOTION:
            CWM_OS_dbgPrintf("%s: %.4f, %.4f\n", "any motion",
                                                       sensorEVT->fData[0],
                                                       sensorEVT->fData[1]);
        break;
        case IDX_ALGO_NO_MOTION:
            CWM_OS_dbgPrintf("%s: %.4f, %.4f\n", "no motion",
                                                       sensorEVT->fData[0],
                                                       sensorEVT->fData[1]);
        break;
	}
}

#define TEST_REPORT_NUM 30
void hook(pSensorEVT_t sensorEVT) {
    if (sensorEVT->sensorType == IDX_ALGO_WATCH_HANDUP) 
		{
			
        CWM_OS_dbgPrintf("GRD_START:%d\n",TEST_REPORT_NUM);
        for (int i = 0; i < TEST_REPORT_NUM ; i++)
				{
            sensorEVT->sensorType = IDX_ALGO_SLEEP;
            sensorEVT->fData[1] = i; //trigger count 0~59
            CWM_OS_dbgPrintf("R/A3: GRD_PROC:%d,%d\n",i+1,TEST_REPORT_NUM);                                                                                                                                                                                                                                                                                                                           
            //CWM_OS_dbgPrintf("#_#TAG=SLR,L SN(%d)IDX(10)DF*1(%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f)\n",i,sensorEVT->fData[0],sensorEVT->fData[1],sensorEVT->fData[2],sensorEVT->fData[3],sensorEVT->fData[4],sensorEVT->fData[5],sensorEVT->fData[6],sensorEVT->fData[7],sensorEVT->fData[8],sensorEVT->fData[9],sensorEVT->fData[10],sensorEVT->fData[11],sensorEVT->fData[12],sensorEVT->fData[13],sensorEVT->fData[14],sensorEVT->fData[15]);
						CWM_OS_dbgPrintf("#_#TAG=SLR,L SN(%d)IDX(10)DF*1(%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f)\n",i,sensorEVT->fData[0],sensorEVT->fData[1],sensorEVT->fData[2],sensorEVT->fData[3],sensorEVT->fData[4],sensorEVT->fData[5],sensorEVT->fData[6],sensorEVT->fData[7],sensorEVT->fData[8],sensorEVT->fData[9],sensorEVT->fData[10],sensorEVT->fData[11],sensorEVT->fData[12]);
						CWM_AP_SensorListen(sensorEVT);
        }
    }
    else 
		{
        CWM_AP_SensorListen(sensorEVT);
    }
}


int main(void)
{
		/* CWM接口 */
		OsAPI device_func = 
		{
			.malloc     =  CWM_OS_malloc,
			.free       =  CWM_OS_free,
			.GetTimeNs  =	 CWM_OS_GetTimeNs,
			.dbgOutput  =  CWM_OS_dbgOutput,
		};

		CustomSensorData csd;
	
    /* Initialize mems driver interface */
		stmdev_ctx_t dev_ctx;
    dev_ctx.write_reg = platform_write;
    dev_ctx.read_reg = platform_read;
    dev_ctx.handle = NULL;	
		
		/* Init test platform */
    platform_init();
	
    /* Wait sensor boot time */
    platform_delay(10);
		

		
		/* LSM6DSOW 参数设置 */	
		/* 检查设备ID */
    lsm6dso_device_id_get(&dev_ctx, &whoamI);
    if (whoamI != LSM6DSO_ID)
        while (1)
            ;
		printf("LSM6DSO Sensor ID： %d \r\n",LSM6DSO_ID);
		nrf_delay_ms(1000);		
		
    /* LSM6DSOW reset配置 */
    lsm6dso_reset_set(&dev_ctx, PROPERTY_ENABLE);
    do
    {
        lsm6dso_reset_get(&dev_ctx, &rst);
    } while (rst);			
		
		/* Disable I3C interface */
    lsm6dso_i3c_disable_set(&dev_ctx, LSM6DSO_I3C_DISABLE);
		

    /* 启用BDU块更新 */
    lsm6dso_block_data_update_set(&dev_ctx, PROPERTY_ENABLE);

    /* 加速度计量程 */
    lsm6dso_xl_full_scale_set(&dev_ctx, LSM6DSO_16g);
		
		/* 陀螺仪量程 */
    lsm6dso_gy_full_scale_set(&dev_ctx, LSM6DSO_2000dps);

    /* 设置输出 加速度计ODR 为26Hz */
    lsm6dso_xl_data_rate_set(&dev_ctx, LSM6DSO_XL_ODR_52Hz);
		
		/* 设置输出 陀螺仪ODR 为26Hz */
    lsm6dso_gy_data_rate_set(&dev_ctx, LSM6DSO_GY_ODR_52Hz);
		
		
    /* 设置 FIFO watermark (number of unread sensor data TAG + 6 bytes stored in FIFO) to 10 samples （set val = 20 -> COUNT = 10 )*/
    lsm6dso_fifo_watermark_set(&dev_ctx, 20);

    /* 设置 加速度计 FIFO ODR为26Hz */
    lsm6dso_fifo_xl_batch_set(&dev_ctx, LSM6DSO_XL_BATCHED_AT_52Hz);
		
		/* 设置 陀螺仪 FIFO ODR为26Hz */
    lsm6dso_fifo_gy_batch_set(&dev_ctx, LSM6DSO_GY_BATCHED_AT_52Hz);

    /* 将FIFO模式设置为流模式(即连续模式)   */
    lsm6dso_fifo_mode_set(&dev_ctx, LSM6DSO_STREAM_MODE);
					
		/* CWM FIFO 初始化 */
		CWM_CustomSensorInput_Fifo_Init(SIGNAL_SIMULATE_FIFO_COUNT);		
		
		/* SettingControl_t */
		SettingControl_t scl;
		
		/* 获取Lib版本信息 */
    memset(&scl, 0, sizeof(scl));
    scl.iData[0] = 1;
    CWM_SettingControl(SCL_GET_LIB_INFO, &scl);
    CWM_OS_dbgPrintf("version:%d.%d.%d.%d product:%d\n", 
											scl.iData[1],
											scl.iData[2], 
											scl.iData[3], 
											scl.iData[4], 
											scl.iData[5]);
											
		/* CWM_LibPreInit */
		CWM_LibPreInit(&device_func);									
																						
		/* 设置MCU芯片信息, 必须在CWM_LibPostInit()之前设置 */
    memset(&scl, 0, sizeof(scl));
    scl.iData[0] = 1;
    scl.iData[1] = 2; //0: mcu_auto_detect 2: skip_mcu_auto_detect
		scl.iData[2] = 109227821;
    CWM_SettingControl(SCL_CHIP_VENDOR_CONFIG, &scl);		

    /*感測器輸入ODR设定*/
    memset(&scl, 0, sizeof(scl));
    scl.iData[0] = 1;
    scl.iData[1] = 25;
    CWM_SettingControl(SCL_ALGO_PROC_CONFIG, &scl);
		
	  /*查看malloc max*/
    memset(&scl, 0, sizeof(scl));
    scl.iData[0] = 1;
    scl.iData[1] = 1;
    CWM_SettingControl(SCL_LIB_DEBUG, &scl);	
											
		/* CWM_LibPostInit初始化 */
    CWM_LibPostInit(CWM_AP_SensorListen);
		//CWM_LibPostInit(hook);	
											
		/* 获取芯片信息 */
		char chipInfo[64];
		memset(&scl, 0, sizeof(scl));
    scl.iData[0] = 1;
    scl.iData[1] = 1;
    scl.iData[2] = (int)chipInfo;
    scl.iData[3] = sizeof(chipInfo);
    scl.iData[4] = 0;
    scl.iData[5] = 0;
    scl.iData[6] = 0;
    CWM_SettingControl(SCL_GET_CHIP_INFO, &scl);
		
    CWM_OS_dbgPrintf("have_security = %d.%d ret_buff_size = %d  chipInfo = %s\n", 
											scl.iData[5], 
											scl.iData[6], 
											scl.iData[4], 
											chipInfo);
    CWM_OS_dbgPrintf("chip_settings = %d, %d, %d\n", 
											scl.iData[9], 
											scl.iData[10], 
											scl.iData[11]);

		/* 打开log输出 */
    memset(&scl, 0, sizeof(scl));
//    scl.iData[0] = 1;
//    scl.iData[3] = 1+4+8;
//    scl.iData[4] = 5;
//		scl.iData[6] = -1-1-2-8-16;
//		scl.iData[7] = -1;
		
		scl.iData[0] = 1;
    scl.iData[3] = 9;
    scl.iData[4] = 5;
		scl.iData[7] = -1;

		CWM_SettingControl(SCL_LOG, &scl);
		
		/* 使能 request_sensor */
    CWM_Sensor_Enable(IDX_REQUEST_SENSOR);									

		#ifdef base1001
		/* 开启1001模式 */
		memset(&scl, 0, sizeof(scl));
    scl.iData[0] = 1;
    scl.iData[1] = 1001;
		#endif
		
		#ifdef running1003
		memset(&scl, 0, sizeof(scl));
    scl.iData[0] = 1;
    scl.iData[1] = 1003;		
		#endif

		#ifdef Swim2001
		memset(&scl, 0, sizeof(scl));
    scl.iData[0] = 1;
    scl.iData[1] = 2001;
		#endif

		#ifdef Swim2002
		memset(&scl, 0, sizeof(scl));
    scl.iData[0] = 1;
    scl.iData[1] = 2002;
		#endif

		#ifdef Biking3001
		memset(&scl, 0, sizeof(scl));
    scl.iData[0] = 1;
    scl.iData[1] = 3001;
		#endif

		#ifdef Interval_Training4001
		memset(&scl, 0, sizeof(scl));
    scl.iData[0] = 1;
    scl.iData[1] = 4001;		
		#endif
		
		#ifdef FreeTraining5001
		memset(&scl, 0, sizeof(scl));
    scl.iData[0] = 1;
    scl.iData[1] = 5001;
		scl.iData[2] = 0;
		#endif
		
		#ifdef workout6001
		memset(&scl, 0, sizeof(scl));
    scl.iData[0] = 1;
    scl.iData[1] = 6001;
		scl.iData[2] = 718685710;
		#endif		
			
		CWM_SettingControl(SCL_SET_ACTIVITY_MODE, &scl);
		
		/* 时间设置 */
		memset(&scl, 0, sizeof(scl));
		scl.iData[0] = 1;	
		scl.iData[1] = 2022;	
		scl.iData[2] = 3;
		scl.iData[3] = 1;
		scl.iData[4] = 12;		
		scl.iData[5] = 0;
		scl.iData[6] = 0;
		CWM_SettingControl(SCL_DATE_TIME, &scl);
		
		
		#ifdef fake_HR
		/* 开启心率，基础卡路里计算*/
		memset(&scl, 0, sizeof(scl));
		scl.iData[0] = 1;
		scl.iData[1] = 1;
		scl.iData[2] = 1;
		CWM_SettingControl(SCL_ACTIVITY_CONFIG, &scl);
		#endif		
		
		#ifdef extra_info		
		/* 1001~1xxx 额外信息设置 */
		memset(&scl, 0, sizeof(scl));
		scl.iData[0] = 1;	
		scl.iData[2] = 2;	
		scl.iData[4] = 1;
		CWM_SettingControl(SCL_PEDO_CONFIG, &scl);
		#endif
		
		#ifdef extra_info		
		/* WM额外信息设置 */
		memset(&scl, 0, sizeof(scl));
		scl.iData[0] = 1;	
		scl.iData[1] = 1;	
		CWM_SettingControl(SCL_WM_CONFIG, &scl);
		#endif
		
		#ifdef sedentary	
		/* 久坐算法设置 */
		memset(&scl, 0, sizeof(scl));
		scl.iData[0] = 1;	
		scl.iData[1] = 20;	
		scl.iData[2] = 1;
		scl.iData[3] = 1;
		scl.iData[4] = 3;		
		scl.iData[5] = 1;
		CWM_SettingControl(SCL_SEDENTARY, &scl);
		
		//静态活动算法模式，低杂讯和高杂讯
		memset(&scl, 0, sizeof(scl));
		scl.iData[0] = 1;		
		scl.iData[1] = 0;	
		CWM_SettingControl(SCL_INACTIVITY_CONFIG, &scl);

		//静态活动算法模式设定：1=久坐
		memset(&scl, 0, sizeof(scl));
		scl.iData[0] = 1;		
		scl.iData[1] = 1;
		//scl.iData[1] = 0;	//关闭静态活动算法
		CWM_SettingControl(SCL_SET_INACTIVITY_MODE, &scl);

		CWM_Sensor_Enable(IDX_ALGO_INACTIVITY_OUTPUT);	
		#endif		
						
		#ifdef WatchHandUp
		memset(&scl, 0, sizeof(scl));
		scl.iData[0] = 1;
		scl.iData[1] = 4;
		scl.iData[2] = 4;
		scl.iData[3] = 4;
		CWM_SettingControl(SCL_HAND_UPDOWN_CONFIG, &scl);		
		CWM_Sensor_Enable(IDX_ALGO_WATCH_HANDUP);
		#endif
		
		#ifdef shake
		CWM_Sensor_Enable(IDX_ALGO_SHAKE);
		#endif
	
		#ifdef folling
		CWM_Sensor_Enable(IDX_ALGO_FALLING);		
		#endif		

		#ifdef sleeping
		memset(&scl, 0, sizeof(scl));
		scl.iData[0] = 1;
		scl.iData[2] = 0;
		scl.iData[3] = 0;
		scl.iData[4] = 0;
		CWM_SettingControl(SCL_SLEEP_CONFIG, &scl);
		
		//静态活动算法模式，低杂讯和高杂讯
		memset(&scl, 0, sizeof(scl));
		scl.iData[0] = 1;		
		scl.iData[1] = 0;	
		CWM_SettingControl(SCL_INACTIVITY_CONFIG, &scl);
		
		//静态活动算法模式设定：2=睡眠
		memset(&scl, 0, sizeof(scl));
		scl.iData[0] = 1;		
		scl.iData[1] = 2;
		//scl.iData[1] = 0;	//关闭静态活动算法
		CWM_SettingControl(SCL_SET_INACTIVITY_MODE, &scl);
		
		//req_sleeping data 模式，不打断睡眠
  	//memset(&scl, 0, sizeof(scl));
	  //scl.iData[0] = 1;
	  //scl.iData[1] = 0;
	  //CWM_SettingControl(SCL_REQ_SLEEPING_DATA, &scl);	

		CWM_Sensor_Enable(IDX_ALGO_INACTIVITY_OUTPUT);
		
		#endif	
				
		
		#ifdef Swim2002
		memset(&scl, 0, sizeof(scl));
    scl.iData[0] = 1;
    scl.iData[1] = 25;
		scl.iData[2] = 1;
		scl.iData[3] = 1;
		CWM_SettingControl(SCL_SWIM_CONFIG, &scl);
		#endif		
		
		#ifdef OFFBODY_DETECT
		CWM_Sensor_Enable(IDX_OFFBODY_DETECT);
		memset(&csd, 0, sizeof(CustomSensorData));
		csd.sensorType = CUSTOM_OFFBODY_DETECT;
		csd.fData[0] = 0;
		CWM_CustomSensorInput(&csd);	
		#endif	
		
		/* Enable ALGO输出*/
		CWM_Sensor_Enable(IDX_ALGO_ACTIVITY_OUTPUT);

	

  while(1)
  {	
			uint16_t num = 0;
			uint8_t wmflag = 0;
			lsm6dso_fifo_tag_t reg_tag;
			axis3bit16_t dummy;
			
			int64_t next_duration = CWM_GetNextActionDuration_ns();
					
			/* Read watermark flag */
			lsm6dso_fifo_wtm_flag_get(&dev_ctx, &wmflag);			
		  if (wmflag > 0)
			{		

						
								
				  /* Read number of samples in FIFO */
					lsm6dso_fifo_data_level_get(&dev_ctx, &num);
					//printf("num: %d\r\n",num);
					/*CWM FIFO START*/
					CWM_CustomSensorInput_Fifo_Start(CUSTOM_ACC,
																			     CWM_OS_GetTimeNs(),
																			     20 * 1000000);	// FIFO间隔时间：1s / 52 *	1000 * 1000000 = 19230770
					CWM_CustomSensorInput_Fifo_Start(CUSTOM_GYRO,
																					 CWM_OS_GetTimeNs(),
																			     20 * 1000000);		
					while(num--)
					{
						 lsm6dso_fifo_sensor_tag_get(&dev_ctx, &reg_tag);
						 switch(reg_tag)
							{
								case LSM6DSO_XL_NC_TAG:
									memset(data_raw_acceleration.u8bit, 0x00, 3 * sizeof(int16_t));
									lsm6dso_fifo_out_raw_get(&dev_ctx, data_raw_acceleration.u8bit);
									acceleration_mg[0] = lsm6dso_from_fs16_to_mg(data_raw_acceleration.i16bit[0]);
									acceleration_mg[1] = lsm6dso_from_fs16_to_mg(data_raw_acceleration.i16bit[1]);
									acceleration_mg[2] = lsm6dso_from_fs16_to_mg(data_raw_acceleration.i16bit[2]);

									acc_in[0] = (float)acceleration_mg[0] * 9.81f / 1000;
									acc_in[1] = (float)acceleration_mg[1] * 9.81f / 1000;
									acc_in[2] = (float)acceleration_mg[2] * 9.81f / 1000;
									
									//将数据传入LIB
									memset(&csd, 0, sizeof(csd));
									csd.sensorType = CUSTOM_ACC;
									csd.fData[0] = acc_in[0];
									csd.fData[1] = acc_in[1];
									csd.fData[2] = acc_in[2];
									CWM_CustomSensorInput(&csd);

									break;
									

								
								case LSM6DSO_GYRO_NC_TAG:
									memset(data_raw_angular_rate.u8bit, 0x00, 3 * sizeof(int16_t));
									lsm6dso_fifo_out_raw_get(&dev_ctx, data_raw_angular_rate.u8bit);
									angular_rate_mdps[0] = lsm6dso_from_fs2000_to_mdps(data_raw_angular_rate.i16bit[0]);
									angular_rate_mdps[1] = lsm6dso_from_fs2000_to_mdps(data_raw_angular_rate.i16bit[1]);
									angular_rate_mdps[2] = lsm6dso_from_fs2000_to_mdps(data_raw_angular_rate.i16bit[2]);

									gyr_in[0] = (float)angular_rate_mdps[0] * 3.1416f / 180 / 1000;
									gyr_in[1] = (float)angular_rate_mdps[1] * 3.1416f / 180 / 1000;
									gyr_in[2] = (float)angular_rate_mdps[2] * 3.1416f / 180 / 1000;
								
									//将数据传入LIB			 
									memset(&csd, 0, sizeof(csd));
									csd.sensorType = CUSTOM_GYRO;
									csd.fData[0] = gyr_in[0];
									csd.fData[1] = gyr_in[1];
									csd.fData[2] = gyr_in[2];
									CWM_CustomSensorInput(&csd);								
																						
									break;
									

								
								default:
									/* Flush unused samples */
									memset(dummy.u8bit, 0x00, 3 * sizeof(int16_t));
									lsm6dso_fifo_out_raw_get(&dev_ctx, dummy.u8bit);
									break;
								
							}
							/* CWM FIFO END*/
							CWM_CustomSensorInput_Fifo_End(CUSTOM_GYRO);
							CWM_CustomSensorInput_Fifo_End(CUSTOM_ACC);
							
							#ifdef fake_HR
							memset(&csd, 0, sizeof(CustomSensorData));
							csd.sensorType = CUSTOM_HEARTRATE;
							csd.fData[0] = rand() % 10 + 80;
							CWM_CustomSensorInput(&csd);
							#endif
						
							#ifdef fake_BARO
							memset(&csd, 0, sizeof(CustomSensorData));
							csd.sensorType = CUSTOM_BARO;
							csd.fData[0] = rand() % 10 + 1010;
							csd.fData[1] = 1;
							csd.fData[2] = 23;
							CWM_CustomSensorInput(&csd);
							#endif
	

		
						

							
					}

					/* 算法计算 */
					CWM_process();					


					//计算一轮FIFO输入的时间值
//					uint64_t processTime;
//					processTime = CWM_OS_GetTimeNs() / 1000000;
//					CWM_OS_dbgPrintf("TS = %llu(ms)\n",processTime);					
		}					
  }
}
	
			



static int32_t platform_write(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len)
{
		
		CWM_OS_i2cWrite(LSM6DSO_I2C_ADD_H, reg, 1, bufp, (uint16_t)len);
		return 0;
}


static int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len)
{
		CWM_OS_i2cRead(LSM6DSO_I2C_ADD_H, reg, 1, bufp, (uint16_t)len);
		return 0;
}


static void platform_delay(uint32_t ms)
{
    nrf_delay_ms(ms);
}


static void platform_init(void)
{
    board_init();
}



