#include "cwm_lib_api.h"
#include "config.h"

/* Variables Defines */
static const char* REQ_SENS_NAME[] = {"acc", "gyro", "mag", "baro", "temp", "hr", "gnss", "ob", "oc", "accany"};
static int m_req_sensor_state = 0;
volatile uint16_t LPM_FLAG = 0;

/* Function Implementations */
int getReqSensor(int index) 
{
	if (m_req_sensor_state & (1<<index))
			return 1;
	return 0;
}

void CWM_GET_LIB_INFO()
{

	SettingControl_t scl;
	memset(&scl, 0, sizeof(scl));
	scl.iData[0] = 1;
	CWM_SettingControl(SCL_GET_LIB_INFO, &scl);
	CWM_OS_dbgPrintf("version:%d.%d.%d.%d product:%d\n", 
										scl.iData[1],
										scl.iData[2], 
										scl.iData[3], 
										scl.iData[4], 
										scl.iData[5]);	
}

		
void CWM_CHIP_VENDOR_CONFIG()
{
	SettingControl_t scl;
	memset(&scl, 0, sizeof(scl));
	scl.iData[0] = 1;
	scl.iData[1] = 0; //0: mcu_auto_detect 2: skip_mcu_auto_detect
	scl.iData[2] = 109227821;
	CWM_SettingControl(SCL_CHIP_VENDOR_CONFIG, &scl);					
}


void CWM_CHIP_INFO()
{
	SettingControl_t scl;
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
	
}

void CWM_ODR_CONFIG()
{
	SettingControl_t scl;
	memset(&scl, 0, sizeof(scl));
	scl.iData[0] = 1;
	scl.iData[1] = CWM_ODR;
	//scl.iData[1] = 50;
	CWM_SettingControl(SCL_ALGO_PROC_CONFIG, &scl);	
}

void CWM_LOG_CONFIG()
{
	SettingControl_t scl;
	memset(&scl, 0, sizeof(scl));
	scl.iData[0] = 1;
	scl.iData[3] = 9;
	scl.iData[4] = 5;
	scl.iData[7] = -1;
	CWM_SettingControl(SCL_LOG, &scl);	
}

void CWM_LIB_DEBUG()
{
	SettingControl_t scl;
	memset(&scl, 0, sizeof(scl));
	scl.iData[0] = 1;
	scl.iData[1] = 1;
	CWM_SettingControl(SCL_LIB_DEBUG, &scl);	
}
void SET_CWM_ACTIVITY_MODE(int activity_mode)
{
	SettingControl_t scl;
	memset(&scl, 0, sizeof(scl));
	scl.iData[0] = 1;
	scl.iData[1] = activity_mode;	
	CWM_SettingControl(SCL_SET_ACTIVITY_MODE, &scl);
}
void SET_CWM_ACTIVITY_SUB_MODE(int activity_mode,int sub_mode_number)
{
	SettingControl_t scl;
	memset(&scl, 0, sizeof(scl));
	scl.iData[0] = 1;
	scl.iData[1] = activity_mode;	
	scl.iData[2] = sub_mode_number;
	CWM_SettingControl(SCL_SET_ACTIVITY_MODE, &scl);
}

void CWM_SWIM_CONFIG()
{
	SettingControl_t scl;
	memset(&scl, 0, sizeof(scl));
	scl.iData[0] = 1;
	scl.iData[1] = 25;
	scl.iData[2] = 1;
	scl.iData[3] = 1;
	CWM_SettingControl(SCL_SWIM_CONFIG, &scl);			
}

void CWM_TIME_INIT()
{
	SettingControl_t scl;
	memset(&scl, 0, sizeof(scl));
	scl.iData[0] = 1;	
	scl.iData[1] = 2022;	
	scl.iData[2] = 1;
	scl.iData[3] = 1;
	scl.iData[4] = 0;		
	scl.iData[5] = 0;
	scl.iData[6] = 0;
	CWM_SettingControl(SCL_DATE_TIME, &scl);	
}

void CWM_INACTIVITY_SENSOR_CONFIG()
{
	SettingControl_t scl;	
	memset(&scl, 0, sizeof(scl));
	scl.iData[0] = 1;		
	scl.iData[1] = 0;	//低杂讯
	CWM_SettingControl(SCL_INACTIVITY_CONFIG, &scl);	
}

void CWM_SEDENTARY_CONFIG()
{
	SettingControl_t scl;	
	memset(&scl, 0, sizeof(scl));
	scl.iData[0] = 1;	
	scl.iData[1] = 20;	
	scl.iData[2] = 1;//提醒功能
	scl.iData[3] = 1;//相隔提醒时间
	scl.iData[4] = 3;//解除久坐等级
	scl.iData[5] = 1;//行走模式解除
	CWM_SettingControl(SCL_SEDENTARY, &scl);		
}

void CWM_SLEEP_CONFIG()
{
	SettingControl_t scl;	
	memset(&scl, 0, sizeof(scl));
	scl.iData[0] = 1;	
	scl.iData[1] = 60;	
	scl.iData[2] = 2;//不使用心率
	scl.iData[3] = 0;//入睡难易度
	scl.iData[4] = 0;//出睡难易度
	CWM_SettingControl(SCL_SLEEP_CONFIG, &scl);		
}

void CWM_REQ_SLEEPING_DATA()
{
	SettingControl_t scl;			
	memset(&scl, 0, sizeof(scl));
	scl.iData[0] = 1;
	scl.iData[1] = 0;
	CWM_SettingControl(SCL_REQ_SLEEPING_DATA, &scl);		
}

void LowPowerMode(int lpm)
{
	SettingControl_t scl;			
	memset(&scl, 0, sizeof(scl));
	scl.iData[0] = 1;
	scl.iData[1] = lpm;
	CWM_SettingControl(SCL_LOW_POWER_MODE, &scl);		
}

void CWM_SET_INACTIVITY_MOD(int inactivity_mode)
{
	SettingControl_t scl;	
	memset(&scl, 0, sizeof(scl));
	scl.iData[0] = 1;		
	scl.iData[1] = inactivity_mode;		
	CWM_SettingControl(SCL_SET_INACTIVITY_MODE, &scl);		
}

void WATCH_HAND_UP_CONFIG()
{
	SettingControl_t scl;	
	memset(&scl, 0, sizeof(scl));
	scl.iData[0] = 1;
	scl.iData[1] = 4;//抬手反应范围(0: default, 3: 範圍大, 4, 5, 6: 範圍小)
	scl.iData[2] = 4;//放手反应范围(0: default, 3: 範圍大, 4, 5, 6: 範圍小)
	scl.iData[3] = 4;//反应速度(0: default, 3: 反應速度快, 4, 5, 6: 反應速度慢)
	CWM_SettingControl(SCL_HAND_UPDOWN_CONFIG, &scl);		
}

void OFF_BODY_STATE(int state)
{	
	CustomSensorData csd;
	memset(&csd, 0, sizeof(CustomSensorData));
	csd.sensorType = CUSTOM_OFFBODY_DETECT;
	csd.fData[0] = state;
	CWM_CustomSensorInput(&csd);		
}

void anymotion(void)
{
	CustomSensorData csd;
	memset(&csd, 0, sizeof(CustomSensorData));
	csd.sensorType = CUSTOM_ACC_ANY_MOTION;
	CWM_CustomSensorInput(&csd);
	CWM_process();	
}

void DataInput(int SensorTyp,float data_x,float data_y,float data_z)
{
	CustomSensorData csd;
	memset(&csd, 0, sizeof(csd));
	csd.sensorType = SensorTyp;
	csd.fData[0] = data_x;
	csd.fData[1] = data_y;
	csd.fData[2] = data_z;
	CWM_CustomSensorInput(&csd);	
}	

void Cwm_fifo_start(int SensorTyp,int DTus)
{
	
	CWM_CustomSensorInput_Fifo_Start(SensorTyp,CWM_OS_GetTimeNs(),DTus * 1000);
}

void Cwm_fifo_end(int SensorTyp)
{
	CWM_CustomSensorInput_Fifo_End(SensorTyp);
}


extern uint16_t once_time_FLAG;

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
						if(getReqSensor(0) == 0 && once_time_FLAG == 1 )
						{
							LPM_FLAG = 1;
							printf("LPM_FLAG=%d\n",LPM_FLAG);
						}
						else
						{
							LPM_FLAG = 0;
							printf("LPM_FLAG=%d\n",LPM_FLAG);
						}

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

#if TEST_HOOK
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
#endif

void cwm_init(void)
{
		/* CWM接口 */
		OsAPI device_func = 
		{
			.malloc     =  CWM_OS_malloc,
			.free       =  CWM_OS_free,
			.GetTimeNs  =	 CWM_OS_GetTimeNs,
			.dbgOutput  =  CWM_OS_dbgOutput,
		};		
		
#ifdef CWM_FIFO_COUNT			
		/* CWM FIFO 初始化 */
		CWM_CustomSensorInput_Fifo_Init(CWM_FIFO_COUNT);		
#endif
		/* 获取Lib版本信息 */	
		CWM_GET_LIB_INFO();
											
		/* CWM_LibPreInit */
		CWM_LibPreInit(&device_func);									
																						
		/* 设置MCU芯片信息, 必须在CWM_LibPostInit()之前设置 */	
		CWM_CHIP_VENDOR_CONFIG();
	
    /*感測器輸入ODR设定*/
		CWM_ODR_CONFIG();


#if CWM_DEBUG
		/*查看malloc max*/
		CWM_LIB_DEBUG();		
#endif

		/* CWM_LibPostInit初始化 */		
#if TEST_HOOK
		CWM_LibPostInit(hook);
#else
		CWM_LibPostInit(CWM_AP_SensorListen);
#endif
    										
		/* 获取芯片信息 */
		CWM_CHIP_INFO();
		
		/* 设置LOG输出 */
		CWM_LOG_CONFIG();

								
#if NORMAL  
		SET_CWM_ACTIVITY_MODE(CWM_ACTIVITY_NORMAL);
		
#endif
#if TREADMILL
		SET_CWM_ACTIVITY_MODE(CWM_ACTIVITY_TREADMILL);
#endif		
#if OUTDOOR_RUNNING
		SET_CWM_ACTIVITY_MODE(CWM_ACTIVITY_OUTDOOR_RUNNING);	
#endif
#if CLIMBING_STAIRS
		SET_CWM_ACTIVITY_MODE(CWM_ACTIVITY_CLIMBING_STAIRS);	
#endif
#if HIKING
		SET_CWM_ACTIVITY_MODE(CWM_ACTIVITY_HIKING);	
#endif
#if INDOOR_RUNNING
		SET_CWM_ACTIVITY_MODE(CWM_ACTIVITY_INDOOR_RUNNING);	
#endif
#if HYBRID_ACTIVITY
		SET_CWM_ACTIVITY_MODE(CWM_ACTIVITY_HYBRID_ACTIVITY);	
#endif
#if FITNESS_PEDOMETER
		SET_CWM_ACTIVITY_MODE(CWM_ACTIVITY_FITNESS_PEDOMETER);	
#endif
#if INDOOR_SWIMMING
		SET_CWM_ACTIVITY_MODE(CWM_ACTIVITY_INDOOR_SWIMMING);
		CWM_SWIM_CONFIG();
#endif
#if OPENWATER_SWIMMING
		SET_CWM_ACTIVITY_MODE(CWM_ACTIVITY_OPENWATER_SWIMMING);	
		CWM_SWIM_CONFIG();
#endif
#if OUTDOOR_BIKING
		SET_CWM_ACTIVITY_MODE(CWM_ACTIVITY_OUTDOOR_BIKING);	
#endif
#if BOXING
		SET_CWM_ACTIVITY_SUB_MODE(CWM_ACTIVITY_WORKOUT_MACHINE,wm_boxing);	
#endif
#if DUMBBELL
		SET_CWM_ACTIVITY_SUB_MODE(CWM_ACTIVITY_WORKOUT_MACHINE,wm_dumbbell);	
#endif
#if SKIPPING
		SET_CWM_ACTIVITY_SUB_MODE(CWM_ACTIVITY_WORKOUT_MACHINE,wm_skipping);	
#endif
#if ELLIPTICAL
		SET_CWM_ACTIVITY_SUB_MODE(CWM_ACTIVITY_WORKOUT_MACHINE,wm_elliptical);	
#endif
#if ROWING
		SET_CWM_ACTIVITY_SUB_MODE(CWM_ACTIVITY_WORKOUT_MACHINE,wm_rowing);	
#endif			


	/* 时间设置 */
	CWM_TIME_INIT();

	/* 静态活动算法模式，低杂讯和高杂讯 */
	CWM_INACTIVITY_SENSOR_CONFIG();
	
	if(INACTIVITY_MODE == CWM_INACTIVITY_SEDENTARY || INACTIVITY_MODE == CWM_INACTIVITY_SEDENTARY_NAP)
	{
		/* 设置久坐config */
		 CWM_SEDENTARY_CONFIG();
	}
	else if(INACTIVITY_MODE == CWM_INACTIVITY_SLEEPING || INACTIVITY_MODE == CWM_INACTIVITY_SLEEPING_STAND)
	{
		/* 设置睡眠config */
		 CWM_SLEEP_CONFIG();
	}
	/* 设置静态运动模式 */
	CWM_SET_INACTIVITY_MOD(INACTIVITY_MODE);
	

#if WatchHandUp
	WATCH_HAND_UP_CONFIG();
	CWM_Sensor_Enable(IDX_ALGO_WATCH_HANDUP);	
#endif
		
#if Folling
	CWM_Sensor_Enable(IDX_ALGO_FALLING);		
#endif			

	LowPowerMode(Lowpowermode);
		

	
	/* Enable ALGO输出*/
	CWM_Sensor_Enable(IDX_REQUEST_SENSOR);	
	CWM_Sensor_Enable(IDX_ALGO_ACTIVITY_OUTPUT);
	CWM_Sensor_Enable(IDX_ALGO_INACTIVITY_OUTPUT);	
	CWM_Sensor_Enable(IDX_OFFBODY_DETECT);
	
	OFF_BODY_STATE(offbody_state);
#if Lowpowermode	
	anymotion();
#endif
}


