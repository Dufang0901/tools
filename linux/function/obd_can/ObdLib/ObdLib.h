#ifndef _OBD_LIB_H_
#define _OBD_LIB_H_

/*��ȡ����������*/
#define OBU_DATA_TEMP 	    	0x01	/*��������ȴҺ�¶ȣ���λ��*/
#define OBU_DATA_RPM        	0x02	/*������ת�٣���λת/��*/
#define OBU_DATA_PRESSURE   	0x03	/*�͹�ѹ������λKPa*/
#define OBU_DATA_SPEED      	0x04	/*�����ٶȣ���λKm/h*/
#define OBU_DATA_INLET_TEMP 	0x05	/*�������������¶ȣ���λ��*/
#define OBU_DATA_DUR_TIME   	0x06	/*���ϴη�����������ʱ�䣬��λs*/

/*OBD�ӿڵķ���ֵ����*/
#define OBD_OK					1
#define OBD_ERROR				-1
#define OBD_ERROR_TIMEOUT				-2


/*��ȡOBD���ݵĽӿ�*/
/*********************************************************
    Function:       ObdInit
    Description:    ��ʼ��ELF327ģ��
    Param:          obdComDev   ��ELF327ģ��ͨ�ŵĴ����豸�ļ�
    Return:         
    Other:          none
*********************************************************/ 
int ObdInit(const char *obdComDev);

/*********************************************************
    Function:       ObdDataGet
    Description:    ��ȡobd����
    Param:          type    ��ȡ����������
    				value   �����ȡ�������ݵ�ָ��
    Return:         ���
    Other:          none
*********************************************************/ 
int ObdDataGet(int type,int *value);

/*********************************************************
    Function:       ObdClose
    Description:    �ر�obd
    Param:          none
    Return:         ���
    Other:          none
*********************************************************/ 
void ObdClose(void);












#endif
