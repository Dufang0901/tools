#include<stdio.h>
typedef char Char;

const Char  Base64Code[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
/*********************************************************
    Function:       Base64Encode
    Description:    BASE编码
    Param:          lpString 要编码的字符串指针
                    lpBuffer 编码后存放的位置
                    sLen 要编码的字符串长度
    Return:         返回结果长度
    Other:          
*********************************************************/
static int Base64Encode(Char *lpString, Char *lpBuffer, int sLen)
{   
    int vLen = 0;  //寄存器局部变量，提速  
    while(sLen > 0)      //处理整个字符串  
    {
        *lpBuffer++ = Base64Code[(lpString[0] >> 2 ) & 0x3F];  //右移两位，与00111111是防止溢出，自加  
        if(sLen > 2) //够3个字符  
        {
            *lpBuffer++ = Base64Code[((lpString[0] & 3) << 4) | (lpString[1] >> 4)];  
            *lpBuffer++ = Base64Code[((lpString[1] & 0xF) << 2) | (lpString[2] >> 6)];  
            *lpBuffer++ = Base64Code[lpString[2] & 0x3F];  
        }else  
        {
            switch(sLen)    //追加“=”  
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
