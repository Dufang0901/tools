#ifndef _STA_REPORT_ANALYSIS_H_
#define _STA_REPORT_ANALYSIS_H_

#define LOG_FILE_DIR     "./center"
#define TRIP_START_FLAG  "Route start succesful:"	/*��·�����ı�ʶ*/

typedef struct{
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
}S_Time;

typedef struct{
	char staCode[16];	/*վ�����*/
	char staName[50];	/*վ������*/
}S_StaInfo;

/*һ����վ��¼*/
typedef struct{
	S_Time sTime;	/*��վʱ��*/
	char   serviceCode[2];	/*����*/
	char   tripCode[20];	/*�˴κ�*/
	char   staCode[16];		/*վ�����*/
	char   staName[50];		/*վ������*/
	char   type;			/*��վ��ʽ*/
}S_Record;

/*����ÿ�˱�վ�����ͳ�Ʒ���*/
typedef struct{
	char      tripCode[20];
	S_Time    startTime;				/*���˿�ʼʱ��*/
	S_Time    endTime;				/*���˽���ʱ��*/
	char      serviceCode[2];		/*����*/
	int       staTotal;				/*��վ��*/
	S_StaInfo staList[100];
	
	int       recordCnt;				/*�����ѱ���վ�����*/
	S_Record  recordList[100];		/*�ѱ���վ���б�*/
	float     rate;					/*��վ��*/
}S_Analysis;







#endif