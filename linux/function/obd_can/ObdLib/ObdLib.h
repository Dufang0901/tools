#ifndef _OBD_LIB_H_
#define _OBD_LIB_H_

/*获取的数据类型*/
#define OBU_DATA_TEMP 	    	0x01	/*发动机冷却液温度，单位℃*/
#define OBU_DATA_RPM        	0x02	/*发动机转速，单位转/分*/
#define OBU_DATA_PRESSURE   	0x03	/*油管压力，单位KPa*/
#define OBU_DATA_SPEED      	0x04	/*车辆速度，单位Km/h*/
#define OBU_DATA_INLET_TEMP 	0x05	/*发动机进气口温度，单位℃*/
#define OBU_DATA_DUR_TIME   	0x06	/*自上次发动机启动的时间，单位s*/

/*OBD接口的返回值定义*/
#define OBD_OK					1
#define OBD_ERROR				-1
#define OBD_ERROR_TIMEOUT				-2


/*获取OBD数据的接口*/
/*********************************************************
    Function:       ObdInit
    Description:    初始化ELF327模块
    Param:          obdComDev   与ELF327模块通信的串口设备文件
    Return:         
    Other:          none
*********************************************************/ 
int ObdInit(const char *obdComDev);

/*********************************************************
    Function:       ObdDataGet
    Description:    获取obd数据
    Param:          type    获取的数据类型
    				value   保存获取到的数据的指针
    Return:         结果
    Other:          none
*********************************************************/ 
int ObdDataGet(int type,int *value);

/*********************************************************
    Function:       ObdClose
    Description:    关闭obd
    Param:          none
    Return:         结果
    Other:          none
*********************************************************/ 
void ObdClose(void);












#endif
