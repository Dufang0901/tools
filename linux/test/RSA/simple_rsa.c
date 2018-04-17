#include<stdio.h>
#include<string.h>

/*************************************************
使用过的密钥对：n=3233   e=17    d=2753
				n=449257 e=55147 d=366523 编码后用long long 型保存
**************************************************/
int n = 25957;	//私钥和公钥使用的公共模数
int e = 1409;	//公钥
int d = 2689;	//私钥

/*******************************************************
	Function:     rsa_encrypt  
	Description: 使用RSA算法对输入的明文进行加密
	Param:    txt        输入的明文
			  ciphertext 用于保存加密后的密文的数组，长度不小于明文大小
			  count      用于保存被加密的明文的字节数
	return    -1 出错；1 成功
*******************************************************/
int rsa_encrypt(char txt[],int ciphertext[],int *count)
{
	if(txt == NULL || ciphertext == NULL)
	{
		printf("input NULL.\n");
		return -1;
	}	
	
	int tmp = 0;
	int i,j;
	int r;
	
	/*加密encryption*/
	for(i = 0;i < strlen(txt);i++)
	{
		r = 1;
		j = e + 1;
		tmp = (int)txt[i];
		
		while(j != 1)
		{
			r = r * tmp;
			r = r % n;
			j--;
		}
		
		ciphertext[i] = r;
	}
	*count = strlen(txt);
}

/*******************************************************
	Function:     rsa_decrypt  
	Description: 对输入的密文进行解密
	Param:    txt           用于保存解密后的明文的数组，长度不小于密文大小
			  decryptedtext 输入的密文
			  count         输入的密文长度
	return    -1 出错；1 成功
*******************************************************/
int rsa_decrypt(char txt[],int ciphertext[],int count)
{
	if(txt == NULL || ciphertext == NULL)
	{
		printf("input NULL.\n");
		return -1;
	}
	
	int tmp,r;
	int i,j;
	
	/*解密decryption*/
	for(i = 0;i < count;i++)
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
		txt[i] = r & 0xFF;
			
	}
}

int main()
{
	int encrypted[512];
	char decrypted[512];
	int count = 0;
	int i;
	
	char str[] = "37|安步荣|男|14243119840524603X|1984-05-23||中国|大学毕业|||海南省洋浦经济开发区新英湾区港务公司宿舍港务公司宿舍临街公寓8楼802||||B2|2007-01-26|||0||4601000020014094737|460000213459|经营性道路货物运输驾驶员|2014-03-20|2020-03-19|海口市道路运输管理处";

	
	//打印明文
	printf("the original text is:\n");
	printf("%s\n\n",str);
	
	memset(decrypted,0,sizeof(decrypted));
	
	rsa_encrypt(str, encrypted,&count);
	
	printf("%d bytes of text are encrypted:\n",count);
	for(i = 0;i < count;i++)
		printf("%x",encrypted[i]);
	printf("\n\n");
	
	rsa_decrypt(decrypted,encrypted,count);
	
	printf("decrypted text is :\n");
	printf("%s\n",decrypted);
	
	return 1;
	
}