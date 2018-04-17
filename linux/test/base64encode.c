#include<stdio.h>
typedef char Char;

const Char  Base64Code[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
/*********************************************************
    Function:       Base64Encode
    Description:    BASE����
    Param:          lpString Ҫ������ַ���ָ��
                    lpBuffer ������ŵ�λ��
                    sLen Ҫ������ַ�������
    Return:         ���ؽ������
    Other:          
*********************************************************/
static int Base64Encode(Char *lpString, Char *lpBuffer, int sLen)
{   
    int vLen = 0;  //�Ĵ����ֲ�����������  
    while(sLen > 0)      //���������ַ���  
    {
        *lpBuffer++ = Base64Code[(lpString[0] >> 2 ) & 0x3F];  //������λ����00111111�Ƿ�ֹ������Լ�  
        if(sLen > 2) //��3���ַ�  
        {
            *lpBuffer++ = Base64Code[((lpString[0] & 3) << 4) | (lpString[1] >> 4)];  
            *lpBuffer++ = Base64Code[((lpString[1] & 0xF) << 2) | (lpString[2] >> 6)];  
            *lpBuffer++ = Base64Code[lpString[2] & 0x3F];  
        }else  
        {
            switch(sLen)    //׷�ӡ�=��  
            {
                case 1:  
                    *lpBuffer ++ = Base64Code[(lpString[0] & 3) << 4 ];  
                    *lpBuffer ++ = '=';  
                    *lpBuffer ++ = '=';  
                    break;  
                case 2:  
                    *lpBuffer ++ = Base64Code[((lpString[0] & 3) << 4) | (lpString[1] >> 4)];  
                    *lpBuffer ++ = Base64Code[((lpString[1] & 0x0F) << 2) | (lpString[2] >> 6)];  
                    *lpBuffer ++ = '=';  
                    break;  
            }  
        }  
        lpString += 3;  
        sLen -= 3;  
        vLen +=4;  
    }  
    *lpBuffer = 0;  
    return vLen;  
}  

void main()
{
	Char enc[2];
    Char dec[2];
    int num = 5;
    enc[0] = (Char)num;
    
    Base64Encode(enc,dec,1);
    
    printf("after encoding, result is %c\n",dec[1]) ;
	
}
