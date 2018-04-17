#ifndef _STA_REPORT_ANALYSIS_H_
#define _STA_REPORT_ANALYSIS_H_

#define LOG_FILE_DIR     "./center"
#define TRIP_START_FLAG  "Route start succesful:"	/*线路启动的标识*/

typedef struct{
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
}S_Time;

typedef struct{
	char staCode[16];	/*站点编码*/
	char staName[50];	/*站点名称*/
}S_StaInfo;

/*一条报站记录*/
typedef struct{
	S_Time sTime;	/*报站时间*/
	char   serviceCode[2];	/*方向*/
	char   tripCode[20];	/*趟次号*/
	char   staCode[16];		/*站点编码*/
	char   staName[50];		/*站点名称*/
	char   type;			/*报站方式*/
}S_Record;

/*用于每趟报站情况的统计分析*/
typedef struct{
	char      tripCode[20];
	S_Time    startTime;				/*本趟开始时间*/
	S_Time    endTime;				/*本趟结束时间*/
	char      serviceCode[2];		/*方向*/
	int       staTotal;				/*总站数*/
	S_StaInfo staList[100];
	
	int       recordCnt;				/*本趟已报的站点个数*/
	S_Record  recordList[100];		/*已报的站点列表*/
	float     rate;					/*报站率*/
}S_Analysis;







#endif