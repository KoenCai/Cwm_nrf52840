#include "cwm_lib_api.h"

/* Variables Defines */
static const char* REQ_SENS_NAME[] = {"acc", "gyro", "mag", "baro", "temp", "hr", "gnss", "ob", "oc", "accany"};
static int m_req_sensor_state = 0;

/* Function Implementations */
int getReqSensor(int index) {
    if (m_req_sensor_state & (1<<index))
        return 1;
    return 0;
}

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

#if 0
	  /*查看malloc max*/
    memset(&scl, 0, sizeof(scl));
    scl.iData[0] = 1;
    scl.iData[1] = 1;
    CWM_SettingControl(SCL_LIB_DEBUG, &scl);	
#endif

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
		scl.iData[0] = 1;
    scl.iData[3] = 9;
    scl.iData[4] = 5;
		scl.iData[7] = -1;

		CWM_SettingControl(SCL_LOG, &scl);
		
		/* 使能 request_sensor */
    CWM_Sensor_Enable(IDX_REQUEST_SENSOR);									

		#ifdef CWM_ACTIVITY_NORMAL
		/* 开启1001模式 */
		memset(&scl, 0, sizeof(scl));
    scl.iData[0] = 1;
    scl.iData[1] = 1001;
		#endif
		
		#ifdef CWM_ACTIVITY_OUTDOOR_RUNNING
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

		//静态活动算法模式设定：3=久坐+小睡
		memset(&scl, 0, sizeof(scl));
		scl.iData[0] = 1;		
		scl.iData[1] = 3;
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

void Cwm_fifo_start(int SensorTyp,int DTms)
{
	CWM_CustomSensorInput_Fifo_Start(SensorTyp,CWM_OS_GetTimeNs(),DTms * 1000000);
}

void Cwm_fifo_end(int SensorTyp)
{
	CWM_CustomSensorInput_Fifo_End(SensorTyp);
}

