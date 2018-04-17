/*****************************************************************************
  FileName：    HteProtocol.c
  Author:       林锴
  Version:      1.0
  Date:         2016/9/9 9:13:51
  Description:  华途嵌入式通用协议相关定义
  History:
                <author>    <time>    <version>    <desc>
                 linca    2016.09.09     V1.0      create
*****************************************************************************/

#include "HteProtocol.h"

/***********************************************************************
    Function:       HteMsgCrc
    Description:    通用协议的检验值计算
    Param:          pInMsg      消息指针
                    inMsgLen    消息长度
    Return:         校验值
    Other:          none
***********************************************************************/
Uint16 HteMsgCrc(Uint8 *pInMsg, Uint16 inMsgLen)
{
    Uint16 crc = 0;
    Uint16 i;

    for( i = 0; i < inMsgLen; i++)
    {
        crc += pInMsg[i];
    }

    crc ^= 0xa596;

    return crc;
}

/***********************************************************************
    Function:       HteBdMsgHead
    Description:    通用协议构建消息头
    Param:          pInMsg      消息指针
                    inSrcDev
                    inSrcId
                    inDstDev
                    inDstId
                    inMsgSn
                    inMsgType
                    inMsgCmd
    Return:         校验值
    Other:          none
***********************************************************************/
void HteBdMsgHead(Uint8 *pInMsg,Uint8 inSrcDev,Uint32 inSrcId,Uint8 inDstDev,Uint32 inDstId,Uint32 inMsgSn,Uint8 inMsgType,Uint8 inMsgCmd)
{
    S_HteMsgHead *psHteMsgHead = (S_HteMsgHead *)pInMsg;

    psHteMsgHead->head[0] = HTE_MSG_HEAD0;
    psHteMsgHead->head[1] = HTE_MSG_HEAD1;

    psHteMsgHead->srcDevType = inSrcDev;
    psHteMsgHead->srcDevId[0] = (inSrcId >> 16) & 0xff;
    psHteMsgHead->srcDevId[1] = (inSrcId >> 8) & 0xff;
    psHteMsgHead->srcDevId[2] = inSrcId  & 0xff;

    psHteMsgHead->dstDevType = inDstDev;
    psHteMsgHead->dstDevId[0] = (inDstId >> 16) & 0xff;
    psHteMsgHead->dstDevId[1] = (inDstId >> 8) & 0xff;
    psHteMsgHead->dstDevId[2] = inDstId  & 0xff;


    psHteMsgHead->sn[0] = (inMsgSn >> 24) & 0xff;
    psHteMsgHead->sn[1] = (inMsgSn >> 16) & 0xff;
    psHteMsgHead->sn[2] = (inMsgSn >> 8) & 0xff;
    psHteMsgHead->sn[3] = inMsgSn  & 0xff;

    psHteMsgHead->type = inMsgType;
    psHteMsgHead->cmd = inMsgCmd;
}

/***********************************************************************
    Function:       HteBdMsg
    Description:    通用协议构建消息
    Param:          pInMsgHead  消息头指针
                    pInData
                    inDataLen
    Return:         校验值
    Other:          none
***********************************************************************/
void HteBdMsg(Uint8 *pInMsgHead,Uint8 *pInData,Uint16 inDataLen)
{
    S_HteMsgHead    *psHteMsgHead = (S_HteMsgHead *)pInMsgHead;
    Uint8           *pData = (Uint8 *)(psHteMsgHead + 1);
    Uint16          crc;
    Uint16          msgLen;

    if(pInData == NULL && inDataLen > 0)
    {
        return;
    }
    msgLen = inDataLen + HTE_MSG_HEAD_LEN;

    if(inDataLen > 0)
    {
        memcpy(pData,pInData,inDataLen);
    }
    psHteMsgHead->msgLen[0] = (msgLen >> 8) & 0xff;
    psHteMsgHead->msgLen[1] = msgLen & 0xff;
    psHteMsgHead->crc[0] = 0x00;
    psHteMsgHead->crc[1] = 0x00;

    crc = HteMsgCrc(pInMsgHead,msgLen);
    psHteMsgHead->crc[0] = (crc >> 8) & 0xff;
    psHteMsgHead->crc[1] = crc & 0xff;

}

/***********************************************************************
    Function:       HteMsgRdByte
    Description:    通用协议接收单字节消息
    Param:          pInRdMsg        保存接收消息的地址
                    inMsgMaxLen     消息的最大长度
                    pInRdLen        已读到的消息长度
                    inRdData        接收到的新的数据
    Return:         返回0表示正常接收
                    返回1表示消息接收完成
                    返回-1表示消息错误重新定位消息头
    Other:          返回1时应执行消息处理,处理完后消息长度执行置0操作
***********************************************************************/
Int32 HteMsgRdByte(Uint8 *pInRdMsg,Uint16 inMsgMaxLen,Uint16 *pInRdLen,Uint8 inRdData)
{
    S_HteMsgHead    *psHteMsgHead = (S_HteMsgHead *)pInRdMsg;
    Uint16          crc;
    Uint16          hteMsgLen;
    Int32           ret = 0;

   if(*pInRdLen == 0)
    {
        if(HTE_MSG_HEAD0 == inRdData)
        {
            /*接收到消息头0*/
            pInRdMsg[(*pInRdLen)++] = inRdData;
        }
        else
        {
            *pInRdLen = 0;
            ret = -1;
        }

    }
    else if(*pInRdLen == 1)
    {
        if(HTE_MSG_HEAD1 == inRdData)
        {
            /*接收到消息头1*/
            pInRdMsg[(*pInRdLen)++] = inRdData;
        }
        else
        {
            *pInRdLen = 0;
            ret = -1;
        }
    }
    else if(*pInRdLen < HTE_MSG_HEAD_LEN)
    {
        pInRdMsg[(*pInRdLen)++] = inRdData;

        /*已完整接收到消息头,确定消息长度*/
        if(*pInRdLen == HTE_MSG_HEAD_LEN)
        {
            hteMsgLen = (psHteMsgHead->msgLen[0] << 8) | psHteMsgHead->msgLen[1];
            if(hteMsgLen > inMsgMaxLen)
            {
                *pInRdLen = 0;
                ret = -1;
            }
            else if(hteMsgLen == HTE_MSG_HEAD_LEN)
            {
                /*单帧消息接收完成*/
                ret = 1;
            }
        }
    }
    else
    {
        pInRdMsg[(*pInRdLen)++] = inRdData;
        hteMsgLen = (psHteMsgHead->msgLen[0] << 8) | psHteMsgHead->msgLen[1];
        if(*pInRdLen == hteMsgLen)
        {
            /*单帧消息接收完成*/
            ret = 1;
        }
    }

    if(ret == 1)
    {
        /*单帧消息接收完成,进行校验*/
        crc = (psHteMsgHead->crc[0] << 8) | psHteMsgHead->crc[1];
        psHteMsgHead->crc[0] = 0x00;
        psHteMsgHead->crc[1] = 0x00;

        if(crc != HteMsgCrc(pInRdMsg, *pInRdLen))
        {
            *pInRdLen = 0;
            ret = -1;
        }
    }
    return ret;
}
