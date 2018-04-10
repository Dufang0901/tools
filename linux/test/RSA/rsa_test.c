#include<stdio.h>
#include<string.h>
static int n = 3233;
static int e = 17;
static int d = 2753;
static char txt[] = "37|安步荣|男|14243119840524603X|1984-05-23||中国|大学毕业|||海南省洋浦经济开发区新英湾区港务公司宿舍港务公司宿舍临街公寓8楼802||||B2|2007-01-26|||0||4601000020014094737|460000213459|经营性道路货物运输驾驶员|2014-03-20|2020-03-19|海口市道路运输管理处";

#define CT 2048	//保存密文的空间大小
#define DT 2048 //保存解密后的明文的空间大小

int main()
{
	 int ciphertext[CT] = {0};	/*存储加密后的密文*/
	char decryptedtext[DT] = {0};
	 int tmp = 0;
	int i,r,j;
	/*打印原文*/
	printf("original text(%d bytes) in hex:\n",strlen(txt));
	printf("%s\n\n",txt);
	for(i = 0;i < strlen(txt);i++)
		printf("%02x",txt[i]);
	printf("\n");
	
	/*加密encryption*/
	for(i = 0;i < strlen(txt);i++)
	{
		tmp = ( int)txt[i];
		printf("tmp=%02x=%d   ",txt[i],tmp);
		r = 1;
		j = e + 1;
		while(j != 1)
		{
			r = r * tmp;
			r = r % n;
			j--;
		}
		if(i >= CT)
			printf("ciphertext buffer overflow.\n");
		else
			ciphertext[i] = r;
		printf("%x\n",ciphertext[i]);
		
	}
	printf("ciphertext is :\n");
	for(i = 0;i < strlen(txt);i++)
		printf("%x",ciphertext[i]);
	printf("\n\n");
	
	/*解密decryption*/
	for(i = 0;i < strlen(txt);i++)
	{
		tmp = ciphertext[i];
		r = 1;
		j = d + 1;
		while(j != 1)
		{
			r = r * tmp;
			r = r % n;
			j--;
		}
		if(i >= DT)
			printf("decryptedtext buffer overflow.\n");
		else
			decryptedtext[i] = r & 0xFF;
			
	}
	printf("decrypted text is :\n");
	printf("%s\n",decryptedtext);
	
}