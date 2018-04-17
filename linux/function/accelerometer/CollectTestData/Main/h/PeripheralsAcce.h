/*****************************************************************************
  FileName:     PeripheralsAcce.h
  Author:       ����
  Version:      1.0
  Date:         2017/2/28 14:35:44
  Description:  ������̵ļ��ٶȴ���������
  History:  
                <author>    <time>    <version>    <desc>
                 linca    2017.02.28    V1.0       create
*****************************************************************************/
#ifndef _PERIPHERALSACCE_H_
#define _PERIPHERALSACCE_H_

#include "OsPack.h"

/*����ٶȴ�������صĲ����ṹ*/
#define PH_ACCE_GET_INTERVAL_MS                 200


typedef struct
{
    Int32               okFlag;                 /*������־λ*/
    
    /*���10�βɼ�������Ϣ*/
    Int32               updateFlag;             /*���±�־*/
    float               acceX;
    float               acceY;
    float               acceZ;
}S_PhAcce;


void PhAccePthreadStart(S_PhAcce *psPhacce);

#endif
