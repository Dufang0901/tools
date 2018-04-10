/*****************************************************************************
  FileName:     PeripheralsAcce.h
  Author:       林锴
  Version:      1.0
  Date:         2017/2/28 14:35:44
  Description:  外设进程的加速度传感器操作
  History:  
                <author>    <time>    <version>    <desc>
                 linca    2017.02.28    V1.0       create
*****************************************************************************/
#ifndef _PERIPHERALSACCE_H_
#define _PERIPHERALSACCE_H_

#include "OsPack.h"

/*与加速度传感器相关的参数结构*/
#define PH_ACCE_GET_INTERVAL_MS                 200


typedef struct
{
    Int32               okFlag;                 /*正常标志位*/
    
    /*最后10次采集到的信息*/
    Int32               updateFlag;             /*更新标志*/
    float               acceX;
    float               acceY;
    float               acceZ;
}S_PhAcce;


void PhAccePthreadStart(S_PhAcce *psPhacce);

#endif
