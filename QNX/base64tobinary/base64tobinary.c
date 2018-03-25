#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>  
#include <unistd.h>  
#include <fcntl.h>  
  
  
const char * base64char = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";  
  
char * base64_encode( const unsigned char * bindata, char * base64, int binlength )  
{  
    int i, j;  
    unsigned char current;  
  
    for ( i = 0, j = 0 ; i < binlength ; i += 3 )  
    {  
        current = (bindata[i] >> 2) ;  
        current &= (unsigned char)0x3F;  
        base64[j++] = base64char[(int)current];  
  
        current = ( (unsigned char)(bindata[i] << 4 ) ) & ( (unsigned char)0x30 ) ;  
        if ( i + 1 >= binlength )  
        {  
            base64[j++] = base64char[(int)current];  
            base64[j++] = '=';  
            base64[j++] = '=';  
            break;  
        }  
        current |= ( (unsigned char)(bindata[i+1] >> 4) ) & ( (unsigned char) 0x0F );  
        base64[j++] = base64char[(int)current];  
  
        current = ( (unsigned char)(bindata[i+1] << 2) ) & ( (unsigned char)0x3C ) ;  
        if ( i + 2 >= binlength )  
        {  
            base64[j++] = base64char[(int)current];  
            base64[j++] = '=';  
            break;  
        }  
        current |= ( (unsigned char)(bindata[i+2] >> 6) ) & ( (unsigned char) 0x03 );  
        base64[j++] = base64char[(int)current];  
  
        current = ( (unsigned char)bindata[i+2] ) & ( (unsigned char)0x3F ) ;  
        base64[j++] = base64char[(int)current];  
    }  
    base64[j] = '\0';  
    return base64;  
}  
  
int base64_decode( const char * base64, unsigned char * bindata )  
{  
    int i, j;  
    unsigned char k;  
    unsigned char temp[4];  
    for ( i = 0, j = 0; base64[i] != '\0' ; i += 4 )  
    {  
        memset( temp, 0xFF, sizeof(temp) );  
        for ( k = 0 ; k < 64 ; k ++ )  
        {  
            if ( base64char[k] == base64[i] )  
                temp[0]= k;  
        }  
        for ( k = 0 ; k < 64 ; k ++ )  
        {  
            if ( base64char[k] == base64[i+1] )  
                temp[1]= k;  
        }  
        for ( k = 0 ; k < 64 ; k ++ )  
        {  
            if ( base64char[k] == base64[i+2] )  
                temp[2]= k;  
        }  
        for ( k = 0 ; k < 64 ; k ++ )  
        {  
            if ( base64char[k] == base64[i+3] )  
                temp[3]= k;  
        }  
  
        bindata[j++] = ((unsigned char)(((unsigned char)(temp[0] << 2))&0xFC)) |  
                ((unsigned char)((unsigned char)(temp[1]>>4)&0x03));  
        if ( base64[i+2] == '=' )  
            break;  
  
        bindata[j++] = ((unsigned char)(((unsigned char)(temp[1] << 4))&0xF0)) |  
                ((unsigned char)((unsigned char)(temp[2]>>2)&0x0F));  
        if ( base64[i+3] == '=' )  
            break;  
  
        bindata[j++] = ((unsigned char)(((unsigned char)(temp[2] << 6))&0xF0)) |  
                ((unsigned char)(temp[3]&0x3F));  
    }  
    return j;  
}  
  
  
  
  
int main()  
{     
  
    //打开图片  
    FILE *fp = NULL;  
    unsigned int size;         //图片字节数  
    char *buffer;  
        char *buffer1;  
    size_t result;  
    char *ret1;   
    unsigned int length;  
  
    fp = fopen("tc2.jpg","rb");  
    if (NULL == fp)  
    {  
        printf("open_error");  
        exit(1);  
    }  
  
  
    //获取图片大小  
    fseek(fp, 0L, SEEK_END);  
    size = ftell(fp);  
    fseek(fp, 0L, SEEK_SET);  
  
    //分配内存存储整个图片  
    buffer = (char *)malloc((size/4+1)*4);  
    if (NULL == buffer)  
    {  
        printf("memory_error");  
        exit(2);  
    }  
  
    //将图片拷贝到buffer  
    result = fread(buffer, 1, size, fp);  
     if (result != size)    
    {    
        printf("reading_error");    
        exit (3);    
    }    
    fclose(fp);  
  
        //base64编码  
    buffer1 = (char *)malloc((size/4+1)*4);  
    if (NULL == buffer1)  
    {  
        printf("memory_error");  
        exit(2);  
    }  
    ret1 = base64_encode(buffer, buffer1, size);  
    length = strlen(buffer1);  
    printf("%s\n", buffer1);  
  
  
    free(buffer);  
    free(buffer1);  
    return 0;  
}  

