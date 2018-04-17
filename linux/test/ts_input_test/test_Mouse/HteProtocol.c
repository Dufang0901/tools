/*****************************************************************************
  FileName��    HteProtocol.c
  Author:       ����
  Version:      1.0
  Date:         2016/9/9 9:13:51
  Description:  ��;Ƕ��ʽͨ��Э����ض���
  History:
                <author>    <time>    <version>    <desc>
                 linca    2016.09.09     V1.0      create
*****************************************************************************/

#include "HteProtocol.h"

/***********************************************************************
    Function:       HteMsgCrc
    Description:    ͨ��Э��ļ���ֵ����
    Param:          pInMsg      ��Ϣָ��
                    inMsgLen    ��Ϣ����
    Return:         У��ֵ
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
    Description:    ͨ��Э�鹹����Ϣͷ
    Param:          pInMsg      ��Ϣָ��
                    inSrcDev
                    inSrcId
                    inDstDev
                    inDstId
                    inMsgSn
                    inMsgType
                    inMsgCmd
    Return:         У��ֵ
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
    Description:    ͨ��Э�鹹����Ϣ
    Param:          pInMsgHead  ��Ϣͷָ��
                    pInData
                    inDataLen
    Return:         У��ֵ
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
    Description:    ͨ��Э����յ��ֽ���Ϣ
    Param:          pInRdMsg        ���������Ϣ�ĵ�ַ
                    inMsgMaxLen     ��Ϣ����󳤶�
                    pInRdLen        �Ѷ�������Ϣ����
                    inRdData        ���յ����µ�����
    Return:         ����0��ʾ��������
                    ����1��ʾ��Ϣ�������
                    ����-1��ʾ��Ϣ�������¶�λ��Ϣͷ
    Other:          ����1ʱӦִ����Ϣ����,���������Ϣ����ִ����0����
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
            /*���յ���Ϣͷ0*/
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
            /*���յ���Ϣͷ1*/
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

        /*���������յ���Ϣͷ,ȷ����Ϣ����*/
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
                /*��֡��Ϣ�������*/
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
            /*��֡��Ϣ�������*/
            ret = 1;
        }
    }

    if(ret == 1)
    {
        /*��֡��Ϣ�������,����У��*/
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
