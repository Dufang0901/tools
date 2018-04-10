#include <stdio.h>
#include <string.h>
#include <stdlib.h>   
#include <time.h>  
#include <math.h>
#include <malloc.h>
#define MAX 1024
#define LEN sizeof(struct slink)


void sub(int a[MAX],int b[MAX] ,int c[MAX] );

struct slink
{ 
	int  bignum[MAX];
	/*bignum[MAX-2]用来标记正负号，1正，0负bignum[MAX-1]来标记实际长度*/
    struct slink *next;
};

/*/--------------------------------------自己建立的大数运算库-------------------------------------*/

void print(int a[MAX])
{
	int i;
	for(i=0;i<a[MAX-1];i++)
	   printf("%d",a[a[MAX-1]-i-1]);
	printf("\n\n");
	return;
}
int cmp(int a1[MAX],int a2[MAX])
{   
	int l1,l2;
	int i;
	l1=a1[MAX-1];
	l2=a2[MAX-1];
	if (l1>l2)
		return 1;
    if (l1<l2)
		return -1;
    for(i=(l1-1);i>=0;i--)
	{
		if(a1[i]>a2[i])
			return 1 ;
		if(a1[i]<a2[i])
			return -1;
	}
	return 0;
}

void mov(int a[MAX],int *b)
{
	int j;
	for(j=0;j<MAX;j++)
		b[j]=a[j];
	return ;
}


void mul(int a1[MAX],int a2[MAX],int *c)
{
	int i,j;
	int y;
	int x;
	int z;
	int w;
	int l1, l2;
	l1=a1[MAX-1];
	l2=a2[MAX-1];
	if (a1[MAX-2]=='-'&& a2[MAX-2]=='-')
		c[MAX-2]=0;
	else if (a1[MAX-2]=='-')
		c[MAX-2]='-';
	else if (a2[MAX-2]=='-')
		c[MAX-2]='-';
 	for(i=0;i<l1;i++)
 	{
 		for(j=0;j<l2;j++)
  		{
     		x=a1[i]*a2[j];
     		y=x/10;
     		z=x%10;
     		w=i+j;
     		c[w]=c[w]+z;
     		c[w+1]=c[w+1]+y+c[w]/10;
			c[w]=c[w]%10;
  		}
 	}
 	w=l1+l2;
 	if(c[w-1]==0)w=w-1;
 		c[MAX-1]=w;
	for (i=0;i<w-2;i++)
	{
		;
	}
 	return;
} 

void add(int a1[MAX],int a2[MAX],int *c)
{

	int i,l1,l2;
	int len,temp[MAX];
	int k=0;
	l1=a1[MAX-1];
	l2=a2[MAX-1];
	if((a1[MAX-2]=='-')&&(a2[MAX-2]=='-'))
	{
		c[MAX-2]='-';
	}
	else if (a1[MAX-2]=='-')
	{
		mov(a1,temp);
		temp[MAX-2]=0;
		sub(a2,temp,c);
		return;
	}
	else if (a2[MAX-2]=='-')
	{
		mov(a2,temp);
		temp[MAX-2]=0;
		sub(a1,temp,c);
		return;
	}

	if(l1<l2)len=l1;
	else len=l2;
	for(i=0;i<len;i++)
	{
		c[i]=(a1[i]+a2[i]+k)%10;
		k=(a1[i]+a2[i]+k)/10;
	}
	if(l1>len)
	{
		for(i=len;i<l1;i++)
		{
			c[i]=(a1[i]+k)%10;
			k=(a1[i]+k)/10;   
		}
		if(k!=0)
		{
			c[l1]=k;
			len=l1+1;
		}
		else len=l1;
	}
	else
	{
		for(i=len;i<l2;i++)
		{
			c[i]=(a2[i]+k)%10;
			k=(a2[i]+k)/10;   
		}
		if(k!=0)
		{
			c[l2]=k;
			len=l2+1;
		}
		else len=l2;
	}
	c[MAX-1]=len;
	return;
} 


void sub(int a1[MAX],int a2[MAX],int *c)
{
	int i,l1,l2;
	int len,t1[MAX],t2[MAX];
	int k=0;
	l1=a1[MAX-1];
	l2=a2[MAX-1];
	if ((a1[MAX-2]=='-') && (a2[MAX-2]=='-'))
	{
		mov(a1,t1);
		mov(a2,t2);
		t1[MAX-2]=0;
		t2[MAX-2]=0;
		sub(t2,t1,c);
		return;
	}
	else if( a2[MAX-2]=='-')
	{
		mov(a2,t2);
		t2[MAX-2]=0;
		add(a1,t2,c);
		return;
	}
	else if (a1[MAX-2]=='-')
	{
		mov(a2,t2);
		t2[MAX-2]='-';
		add(a1,t2,c);
		return;
	}

	if(cmp(a1,a2)==1)
	{

		len=l2;
		for(i=0;i<len;i++)
		{
			if ((a1[i]-k-a2[i])<0)
			{
				c[i]=(a1[i]-a2[i]-k+10)%10;
				k=1;
			}
			else 
			{
				c[i]=(a1[i]-a2[i]-k)%10;
				k=0;
			}
		}
		for(i=len;i<l1;i++)
		{
			if ((a1[i]-k)<0)
			{
				c[i]=(a1[i]-k+10)%10;
				k=1;
			}
			else 
			{
				c[i]=(a1[i]-k)%10;
				k=0;
			} 
		}
		if(c[l1-1]==0)/*使得数组C中的前面所以0字符不显示了，如1000-20=0980--->显示为980了*/
		{
			len=l1-1;
			i=2;
			while (c[l1-i]==0)/*111456-111450=00006，消除0后变成了6；*/
			{
				len=l1-i;
				i++;
			}
		}
		else 
		{
			len=l1;
		}
	}
	else	if(cmp(a1,a2)==(-1))
	{
		c[MAX-2]='-';
		len=l1;
		for(i=0;i<len;i++)
		{
			if ((a2[i]-k-a1[i])<0)
			{
				c[i]=(a2[i]-a1[i]-k+10)%10;
				k=1;
			}
			else 
			{
				c[i]=(a2[i]-a1[i]-k)%10;
				k=0;
			}
		}
		for(i=len;i<l2;i++)
		{
			if ((a2[i]-k)<0)
			{
				c[i]=(a2[i]-k+10)%10;
				k=1;
			}
			else 
			{
				c[i]=(a2[i]-k)%10;
				k=0;
			}   
		}
		if(c[l2-1]==0)
		{  
			len=l2-1;
			i=2;
			while (c[l1-i]==0)
			{
				len=l1-i;
				i++;
			}

		}
		else len=l2;
	}
	else if(cmp(a1,a2)==0)
	{
		len=1;
		c[len-1]=0;
	}
	c[MAX-1]=len;
	return;
}

void  mod(int a[MAX],int b[MAX],int  *c)/*/c=a mod b//注意：经检验知道此处A和C的数组都改变了。*/
{	
	int d[MAX];
	mov (a,d);
	while (cmp(d,b)!=(-1))/*/c=a-b-b-b-b-b.......until(c<b)*/
	{
		sub(d,b,c);
		mov(c,d);/*/c复制给a*/
	}	
	return ;
}


void  divt(int t[MAX],int b[MAX],int  *c ,int *w)/*//试商法//调用以后w为a mod b, C为a  div b;*/
{

	int a1,b1,i,j,m;/*w用于暂时保存数据*/
	int d[MAX],e[MAX],f[MAX],g[MAX],a[MAX];
	
	mov(t,a);
		for(i=0;i<MAX;i++)
	   e[i]=0;
	for(i=0;i<MAX;i++)
	   d[i]=0;
	for(i=0;i<MAX;i++) g[i]=0;
	a1=a[MAX-1];
    b1=b[MAX-1];
	if (cmp(a,b)==(-1))
	{
		c[0]=0;
		c[MAX-1]=1;
		mov(t,w);
		return;
	}
	else if (cmp(a,b)==0)
	{
		c[0]=1;
        c[MAX-1]=1;
		w[0]=0;
		w[MAX-1]=1;
		return;
	}
		m=(a1-b1);
    for(i=m;i>=0;i--)/*341245/3=341245-300000*1--->41245-30000*1--->11245-3000*3--->2245-300*7--->145-30*4=25--->25-3*8=1*/
	{
      for(j=0;j<MAX;j++)
        d[j]=0;
		d[i]=1;
		d[MAX-1]=i+1;
		mov(b,g);
        mul(g,d,e);

	
		while (cmp(a,e)!=(-1))
		{
			c[i]++;
			sub(a,e,f);
		    mov(f,a);/*f复制给g*/
		}

     for(j=i;j<MAX;j++)/*高位清零*/
        e[j]=0;
	
	}
	mov(a,w);
	if (c[m]==0) c[MAX-1]=m;
	else c[MAX-1]=m+1;

	return;
}

void mulmod(int a[MAX] ,int b[MAX] ,int n[MAX],int *m)/*解决 了 m=a*b mod n;*/
{
	int c[MAX],d[MAX];
	int i;
	for(i=0;i<MAX;i++)
		d[i]=c[i]=0;
	mul(a,b,c);
	divt(c,n, d,m);
	for(i=0;i<m[MAX-1];i++)
		printf("%d",m[m[MAX-1]-i-1]);
	printf("\nm  length is :  %d \n",m[MAX-1]);
}

/*接下来的重点任务是要着手解决 m=a^p  mod n的函数问题。*/

void expmod(int a[MAX] ,int p[MAX] ,int n[MAX],int *m)
{
	int t[MAX],l[MAX],temp[MAX]; /*/t放入2，l放入1；*/
	int w[MAX],s[MAX],c[MAX],b[MAX],i;
	for(i=0;i<MAX-1;i++)
		b[i]=l[i]=t[i]=w[i]=0;
	t[0]=2;t[MAX-1]=1;
	l[0]=1;l[MAX-1]=1;
	mov(l,temp);
	mov(a,m);
	mov(p,b);

	while(cmp(b,l)!=0)
	{
		for(i=0;i<MAX;i++)
		w[i]=c[i]=0;

		divt(b,t,w,c);/*// c=p mod 2  w= p /2*/
	
		mov(w,b);/*//p=p/2*/

		if(cmp(c,l)==0) /*/余数c==1*/
		{
			for(i=0;i<MAX;i++)
			w[i]=0;

			mul(temp,m,w);

			mov(w,temp);
	
			for(i=0;i<MAX;i++)
			w[i]=c[i]=0;

			divt(temp,n,w,c);/* /c为余c=temp % n，w为商w=temp/n */

			mov(c,temp);
		}

		for(i=0;i<MAX;i++)
		s[i]=0;

		mul(m,m,s);//s=a*a

		for(i=0;i<MAX;i++)
		c[i]=0;

		divt(s,n,w,c);/*/w=s/n;c=s mod n*/

		mov (c,m);

	}

	for(i=0;i<MAX;i++)
	s[i]=0;

	mul(m,temp,s);

	for(i=0;i<MAX;i++)
	c[i]=0;

	divt(s,n,w,c);

	mov (c,m);/*余数s给m*/

	m[MAX-2]=a[MAX-2];/*为后面的汉字显示需要，用第99位做为标记*/

	return;
	/*/k=temp*k%n;*/

}


int is_prime_san(int p[MAX] )
{
 
	int i,a[MAX],t[MAX],s[MAX],o[MAX]; 
	for(i=0;i<MAX;i++)
		s[i]=o[i]=a[i]=t[i]=0;
	t[0]=1;
	t[MAX-1]=1;
	a[0]=2;// { 2,3,5,7 }
	a[MAX-1]=1;

	sub(p,t,s);

	expmod ( a, s, p ,o);
	if ( cmp(o,t) != 0 ) 
	{
		return 0;
	}

	a[0]=3;
	for(i=0;i<MAX;i++)  
		o[i]=0;

	expmod ( a, s, p ,o);
	if ( cmp(o,t) != 0 ) 	   
	{
		return 0;
	}

	a[0]=5;
	for(i=0;i<MAX;i++)  
		o[i]=0;
	expmod ( a, s, p ,o);
	if ( cmp(o,t) != 0 ) 
	{	   
		return 0;
	}

	a[0]=7;
	for(i=0;i<MAX;i++)  
		o[i]=0;

	expmod ( a, s, p ,o);
	if ( cmp(o,t) != 0 ) 
	{
		return 0;
	}
	return 1;
}


int coprime(int e[MAX],int s[MAX]) /*//// 求两个大数之间是否互质////*/

{
    int a[MAX],b[MAX],c[MAX],d[MAX],o[MAX],l[MAX];
    int i;
	for(i=0;i<MAX;i++)
		l[i]=o[i]=c[i]=d[i]=0;
	o[0]=0;o[MAX-1]=1;
	l[0]=1;l[MAX-1]=1;
	mov(e,b);
	mov(s,a);
	do
	{
		if(cmp(b,l)==0)
		{
			return 1;
		}
		for(i=0;i<MAX;i++)
			c[i]=0;
		 divt(a,b,d,c);
		 mov(b,a);/*b--->a*/
		 mov(c,b);/*c--->b*/

	}while(cmp(c,o)!=0);
	/*	printf("Ihey are  not coprime!\n");*/
	return 0;
}


void prime_random(int *p,int *q)
{
	int i,k;
	int n;
	time_t t; 
	 p[0]=1;
	 q[0]=3;
	 
//	p[19]=1;
//	q[18]=2;
	
	p[MAX-1]=10;	//10,11
    q[MAX-1]=11;

 
	do
	{
		n++;
		t=time(NULL);
		srand((unsigned long)t);
		for(i=1;i<p[MAX-1]-1;i++)
		{
			k=rand()%10;
			p[i]=k;
		}
		k=rand()%10;
		while (k==0)
		{
			k=rand()%10;
		}
		p[p[MAX-1]-1]=k;

	}while((is_prime_san(p))!=1);
	printf("n=%d素数 p 为  : ",n);
	for(i=0;i<p[MAX-1];i++)
	{
		printf("%d ",p[p[MAX-1]-i-1]);
	}
	printf("\n");
	n=0;
	do
	{
		n++;
		t=time(NULL);
		srand((unsigned long)t);
		for(i=1;i<q[MAX-1];i++)
		{
			k=rand()%10;
			q[i]=k;
		}

	}while((is_prime_san(q))!=1);
		
	printf("n=%d素数 q 为 : ",n);
    for(i=0;i<q[MAX-1];i++)
	{
		printf("%d",q[q[MAX-1]-i-1]);
	}
    printf("\n");
	return;
}
	
void  erand(int e[MAX],int m[MAX])
{
	int i,k;
	time_t t;
	e[MAX-1]=5;
	printf("随机产生一个与(p-1)*(q-1)互素的 e :");
	do
	{
		t=time(NULL);
		srand((unsigned long)t);
		for(i=0;i<e[MAX-1]-1;i++)
		{
			k=rand()%10;
			e[i]=k;
		}
		while((k=rand()%10)==0)
			k=rand()%10;
		e[e[MAX-1]-1]=k;
	}while(coprime( e, m)!=1);
	for(i=0;i<e[MAX-1];i++)
	{
		printf("%d",e[e[MAX-1]-i-1]);
	}
	printf("\n\n");
	return ;
}

void rsad(int e[MAX],int g[MAX],int *d)
{
	int   r[MAX],n1[MAX],n2[MAX],k[MAX],w[MAX];
	int     i,t[MAX],b1[MAX],b2[MAX],temp[MAX];
	mov(g,n1);
	mov(e,n2);
	for(i=0;i<MAX;i++)
		k[i]=w[i]=r[i]=temp[i]=b1[i]=b2[i]=t[i]=0;

	b1[MAX-1]=0;b1[0]=0;/*/b1=0;*/
	b2[MAX-1]=1;b2[0]=1;/*/b2=1;*/
	while(1)
	{
		for(i=0;i<MAX;i++)
			k[i]=w[i]=0;
		divt(n1,n2,k,w);/*/k=n1/n2;*/
		for(i=0;i<MAX;i++)
			temp[i]=0;
		mul(k,n2,temp);/*/temp=k*n2;*/
		for(i=0;i<MAX;i++)
			r[i]=0;
		sub(n1,temp,r);

		if((r[MAX-1]==1) && (r[0]==0))/*/r=0*/
		{
			break;
		}
		else
		{
			mov(n2,n1);/*/n1=n2;*/
			mov( r,n2);/*/n2=r;*/
			mov(b2, t);/*/t=b2;*/
			for(i=0;i<MAX;i++)
				temp[i]=0;
			mul(k,b2,temp);/*/b2=b1-k*b2;*/
			for(i=0;i<MAX;i++)
				b2[i]=0;
			sub(b1,temp,b2);
			mov(t,b1);
		}
	}

	for(i=0;i<MAX;i++)
		t[i]=0;
	add(b2,g,t);
	for(i=0;i<MAX;i++)
		temp[i]=d[i]=0;
	divt(t,g,temp,d);
	printf("由以上的(p-1)*(q-1)和 e 计算得出的 d : ");
	for(i=0;i<d[MAX-1];i++)
		printf("%d",d[d[MAX-1]-i-1]);
	printf("\n\n");
}





/*/求解密密钥d的函数(根据Euclid算法)96403770511368768000*/
unsigned long  rsa(unsigned long p,unsigned long q,unsigned long e)  /*/求解密密钥d的函数(根据Euclid算法)*/
{
	unsigned long g,k,r,n1,n2,t;
	unsigned long b1=0,b2=1;

	g=(p-1)*(q-1);
	n1=g;
	n2=e;

	while(1)
	{
		k=n1/n2;
		r=n1-k*n2;
		if(r!=0)
		{
			n1=n2;
			n2=r;
			t=b2;
			b2=b1-k*b2;
			b1=t;
		}

		else
		{
			break;
		}

	}

	return (g+b2)%g;
}
/*/------------------------------------------导入导出公钥和私钥------------------------------------/*/
void loadpkey(int e[MAX],int n[MAX])  //导入公钥
{
	FILE *fp;
	char filename[25],str[MAX],ch;
	int i,k;
	for(i=0;i<MAX;i++)
		e[i]=n[i]=0;
	while(1)
	{
		printf("\n");
		printf("为导入(e,n)，请输入加密密钥对文件路径: \n");
		scanf("%s",filename);
		if((fp=fopen(filename,"r"))==NULL)
			printf("输入的文件不存在，请重新输入!\n");
		else 
			break;
	}
	k=0;
	while((ch=fgetc(fp))!=EOF)
	{	
		if(ch!=' ')
		{
			str[k]=ch;
			k++;
		}
		else
		{
			for(i=0;i<k;i++)
			{
				e[i]=str[k-i-1]-48;
			}
			e[MAX-1]=k;
			k=0;
		}
	}
	for(i=0;i<k;i++)
		n[i]=str[k-i-1]-48;
	n[MAX-1]=k;
	printf("\n加密密钥 e : ");
	for(i=0;i<e[MAX-1];i++)
		printf("%d",e[e[MAX-1]-i-1]);
	printf("\n");
	printf("\n    公钥 n : ");
	for(i=0;i<n[MAX-1];i++)
		printf("%d",n[n[MAX-1]-i-1]);
	printf("\n");
	fclose(fp);
	printf("\n导入(e,n)成功!\n");
	getchar();
}

void loadskey(int d[MAX],int n[MAX]) //导入私钥
{
	{
		FILE *fp;
		char filename[25],str[MAX],ch;
		int i,k;
		for(i=0;i<MAX;i++)
			d[i]=n[i]=0;
		while(1)
		{
			printf("为导入(d,n)，请输入解密密钥对文件的路径: \n");
			scanf("%s",filename);
			if((fp=fopen(filename,"r"))==NULL)
			{
				printf("输入的文件不存在，请重新输入!\n");   
			}
			else break;
		}
		k=0;
		while((ch=fgetc(fp))!=EOF)
		{	
			if(ch!=' ')
			{
				str[k]=ch;
				k++;
			}
			else
			{
				for(i=0;i<k;i++)
				{
					d[i]=str[k-i-1]-48;
				}
				d[MAX-1]=k;
				k=0;
			}
		}
		for(i=0;i<k;i++)
			n[i]=str[k-i-1]-48;
		n[MAX-1]=k;
		printf("\n解密密钥 d : ");
		for(i=0;i<d[MAX-1];i++)
			printf("%d",d[d[MAX-1]-i-1]);
		printf("\n");
		printf("\n    公钥 n : ");
		for(i=0;i<n[MAX-1];i++)
			printf("%d",n[n[MAX-1]-i-1]);
		printf("\n");
		fclose(fp);
		printf("\n导入(d,n)成功!\n");
		getchar();
	}
}

void savepkey(int e[MAX],int n[MAX])//导出公钥
{
	FILE *fp;
	int  i; 
	char  savefile[25],ch;
	printf("导出加密密钥(e,n)，存放的文件路径为: ");
	scanf("%s",savefile);
	printf("\n");
	fp=fopen(savefile,"w");
	for(i=0;i<e[MAX-1];i++)
	{
		ch=e[e[MAX-1]-i-1]+48;
		fputc(ch,fp);
	}
	ch=' ';
	fputc(ch,fp);
	for(i=0;i<n[MAX-1];i++)
	{
		ch=n[n[MAX-1]-i-1]+48;
		fputc(ch,fp);
	}
	fclose(fp);
	printf("\n保存(e,n)操作完成!\n");
}

void saveskey(int d[MAX],int n[MAX])//导出私钥
{
	FILE *fp;
	int  i; 
	char  savefile[25],ch;
	printf("导出解密密钥(d,n)，存放的文件路径为: ");
	scanf("%s",savefile);
	printf("\n");
	fp=fopen(savefile,"w");
	for(i=0;i<d[MAX-1];i++)
	{
		ch=d[d[MAX-1]-i-1]+48;
		fputc(ch,fp);
	}
	ch=' ';
	fputc(ch,fp);
	for(i=0;i<n[MAX-1];i++)
	{
		ch=n[n[MAX-1]-i-1]+48;
		fputc(ch,fp);
	}
	fclose(fp);
	printf("\n保存(d,n)操作完成!\n");
}

/*/------------------------------------------加密和解密的块------------------------------------/*/

void  printbig(struct slink *h)
{
	struct slink *p;
	int i;
	p=(struct slink * )malloc(LEN);
	p=h;
	if(h!=NULL)
		do 
		{
			for(i=0;i<p->bignum[MAX-1];i++)
				printf("%d",p->bignum[p->bignum[MAX-1]-i-1]);
			p=p->next;
		}

		while(p!=NULL);
		printf("\n\n");

 }

void  tencrypto(int e[MAX], int n[MAX])/*//对有需要的文件进行加密*/
{
	FILE *fp;
	int i,k,count,temp,c;
	char filename[25],ch,encryfile[25];
	struct slink  *p,*p1,*p2;
	struct slink  *h;
	h=p=p1=p2=(struct slink * )malloc(LEN);
	h=NULL;
	printf("\n输入需要加密的文件路径 : ");
	scanf("%s",filename);
	if((fp=fopen(filename,"r"))==NULL)
	{
		printf("Cannot open file !\n");
		exit(0);
	}
	printf("\n文件的原文内容：\n\n");
	count=0;
	while((ch=fgetc(fp))!=EOF)
	{  
		putchar(ch);
		c=ch;
		k=0;
		if(c<0)
		{
			c=abs(c);/*/把负数取正并且做一个标记*/
			p1->bignum[MAX-2]='0';
		}
		else
		{
			p1->bignum[MAX-2]='1';
		}

		while(c/10!=0)
		{
			temp=c%10;
			c=c/10;
			p1->bignum[k]=temp;
			k++;
		}
		p1->bignum[k]=c;
		p1->bignum[MAX-1]=k+1;
		count=count+1;
		if(count==1)
			h=p1;
		else p2->next=p1;
		p2=p1;
		p1=(struct slink * )malloc(LEN);
	}
	p2->next=NULL; 
	printf("\n");
	fclose(fp);

	//   printf("加密后文件的保存路径 : \n");
	//   scanf("%s",encryfile);
	//   fp=fopen(encryfile,"w");
	fp=fopen(filename,"w");
	p=p1=(struct slink * )malloc(LEN);
	p=h;
	printf("\n加密后文件中所形成密文：\n\n");
	if(h!=NULL)
	do 
	{ 
		expmod( p->bignum , e ,n ,p1->bignum);
		ch=p1->bignum[MAX-2];
		printf("%c",ch);
		fputc(ch,fp);
		if ((p1->bignum[MAX-1]/10) ==0)/*/判断p1->bignum[MAX-1]的是否大于十；*/
		{
			ch=0+48;
			printf("%c",ch);
			fputc(ch,fp);
			ch=p1->bignum[MAX-1]+48;
			printf("%c",ch);
			fputc(ch,fp);
		}
		else
		{
			ch=p1->bignum[MAX-1]/10+48;
			printf("%c",ch);
			fputc(ch,fp);
			ch=p1->bignum[MAX-1]%10+48;
			printf("%c",ch);
			fputc(ch,fp);
		}

		for(i=0;i<p1->bignum[MAX-1];i++)
		{
			printf("%d",p1->bignum[i]);
			ch=p1->bignum[i]+48;
			fputc(ch,fp);
		}
		p=p->next;
		p1=(struct slink * )malloc(LEN);
	}while(p!=NULL);

	printf("\n\n");
	fclose(fp);
	return;
}

void  tdecrypto(int d[MAX], int n[MAX])
{
	
	FILE *fp;   
	struct slink *h,*p1,*p2;
	char ch,encryfile[25],decryfile[25];
	int i,j,k,c,count,temp;
	printf("\n输入加密过的文件路径 : ");
	scanf("%s",encryfile);
	if((fp=fopen(encryfile,"r"))==NULL)
	{
		printf("此文件不存在!\n");
		exit(0);
	}
	printf("\n文件中密文内容：\n\n");
	i=0;
	j=3;
	count=0;
	h=p1=p2=(struct slink * )malloc(LEN);

	while((ch=fgetc(fp))!=EOF)
	{  
		putchar(ch);
		c=ch;	   
		if(j==3)
		{
			p1->bignum[MAX-2]=c;
			j--;
		}
		else if(j==2)
		{
			temp=c-48;
			j--;
		}
		else if(j==1)
		{
			p1->bignum[MAX-1]=temp*10+c-48;
			j--;
		}
		else if (j==0)
		{
			p1->bignum[i]=c-48;
			i++;
			if(i==p1->bignum[MAX-1])
			{ 
				i=0;
				j=3;
				count++;
				if (count==1)
					h=p1;
				else p2->next=p1;
				p2=p1;
				p1=(struct slink * )malloc(LEN);
			}
		}
	}
	p2->next=NULL; 
	printf("\n");
	fclose(fp);

	
	//	printf("解密后的明文文件保存路径 : \n");
	//    scanf("%s",decryfile);
	//    fp=fopen(decryfile,"w");
	fp=fopen(encryfile,"w");
	printf("\n解密密文后文件中的明文:\n\n");
	p2=(struct slink * )malloc(LEN);
	p1=h;
	k=0;
	if(h!=NULL)/*/temp为暂存ASIIC码的int值*/
	do
	{
		for(i=0;i<MAX;i++)
			p2->bignum[i]=0;
		expmod( p1->bignum , d ,n ,p2->bignum);		  
		temp=p2->bignum[0]+p2->bignum[1]*10+p2->bignum[2]*100;
		if (( p2->bignum[MAX-2])=='0')
		{
			temp=0-temp;
		}/*/转化为正确的ASIIC码，如-78-96形成汉字	*/	 
		ch=temp;/*  str[k]--->ch */
		printf("%c",ch);/*  str[k]--->ch */
		fputc(ch,fp);/*/写入文件str[k]--->ch*/
		k++;
		p1=p1->next;
		p2=(struct slink * )malloc(LEN);
	}while (p1!=NULL);
	printf("\n\n");
	fclose(fp);
	return;

}


 struct slink *input(void)/*/输入明文并且返回头指针，没有加密时候转化的数字*/
{   
	struct  slink *head;
	struct  slink *p1,*p2;
	int  i,n,c,temp;
	char  ch;
	n=0;
	p1=p2=(struct slink * )malloc(LEN);
	head=NULL;
	printf("\n请输入你所要加密的内容 : \n");
	while((ch=getchar())!='\n')
	{
		i=0;
		c=ch;
		if(c<0)
		{
			c=abs(c);/*/把负数取正并且做一个标记*/
			p1->bignum[MAX-2]='0';
		}
		else
		{
			p1->bignum[MAX-2]='1';
		}
		while(c/10!=0)
		{
			temp=c%10;
			c=c/10;
			p1->bignum[i]=temp;
			i++;
		}
		p1->bignum[i]=c;
		p1->bignum[MAX-1]=i+1;
		n=n+1;
		if(n==1)
			head=p1;
		else 
			p2->next=p1;
		p2=p1;
		p1=(struct slink * )malloc(LEN);
	}
	p2->next=NULL; 
	return(head);
}



struct slink *jiami(int  e[MAX],int  n[MAX],struct  slink *head)
{
	struct  slink *p;
	struct  slink *h;
	struct  slink *p1,*p2;
	int  m=0,i;
	printf("\n");
	printf("加密后形成的密文内容：\n");
	p1=p2=(struct slink* )malloc(LEN);
	h=NULL;
	p=head;
	if(head!=NULL)
		do 
		{

			expmod( p->bignum , e ,n ,p1->bignum);

			for(i=0;i<p1->bignum[MAX-1];i++)
			{
				printf("%d",p1->bignum[p1->bignum[MAX-1]-1-i]);
			}

			m=m+1;
			if(m==1)
				h=p1;
			else p2->next=p1;
			p2=p1;
			p1=(struct slink * )malloc(LEN);
			p=p->next;
		}	while(p!=NULL);
		p2->next=NULL;   
		p=h;
		printf("\n");

		return(h);
    
}

void jiemi(int d[MAX],int n[MAX],struct  slink *h)
{
	int   i,j,temp;
	struct slink *p,*p1;
	char ch[65535];
	p1=(struct slink* )malloc(LEN);
	p=h;
	j=0;

	if(h!=NULL)
		do 
		{
			for(i=0;i<MAX;i++)
				p1->bignum[i]=0;

			expmod( p->bignum , d ,n ,p1->bignum);

			temp=p1->bignum[0]+p1->bignum[1]*10+p1->bignum[2]*100;
			if (( p1->bignum[MAX-2])=='0')
			{
				temp=0-temp;
			}

			ch[j]=temp;
			j++;
			p=p->next;
		}while (p!=NULL);
		printf("\n");
		printf("解密密文后所生成的明文:\n");
		for(i=0;i<j;i++)
			printf("%c",ch[i]);
		printf("\n");
		return;
	
}


void menu()
{
	printf("               R--------产生密钥对      \n");
    printf("               S--------保存密钥对      \n");
	printf("               L--------载入密钥对      \n");
	printf("               E--------对文件加密      \n");
	printf("               D--------对文件解密      \n");
	printf("               T--------简单测试      \n");
	printf("               Q--------退出            \n");
	printf("请选择一种操作：");

}

/*/-------------------------------------------------主MAIN函数-----------------------------------/*/
int main()
{
	int  i;
	char c;
    int  p[MAX],q[MAX],n[MAX],d[MAX],e[MAX],m[MAX],p1[MAX],q1[MAX];
	struct slink *head,*h1,*h2;
	for(i=0;i<MAX;i++)
    	m[i]=p[i]=q[i]=n[i]=d[i]=e[i]=0;/*/简单初始化一下*/ 
	while (1)
	{
		menu();
		c=getchar();
		getchar();//接受回车符
		if ((c=='r') || (c=='R'))//操作r产生密钥对
		{        

		}
		else if((c=='l') || (c=='L'))
		{
			printf("选择导入密钥类型：加密密钥(P)还是解密密钥(S)?");
			c=getchar();
			getchar();
			if ((c=='p')||(c=='P'))
				loadpkey(e,n);
			else if((c=='s')||(c=='S'))
				loadskey(d,n);	
			printf("按任意键回主菜单…………");
			getchar();
		}
    	else if ((c=='e') || (c=='E'))
		{
		     tencrypto( e,  n);
			 printf("加密文件操作完成!\n");
			 printf("按任意键回主菜单…………");
			 getchar();
			 getchar();
		}
    	else if ((c=='d') || (c=='D'))
	   {
		    tdecrypto( d, n);
			printf("解密文件操作完成!\n");
			printf("按任意键回主菜单…………");
			getchar();
			getchar();
	   }
		else if ((c=='s')||(c=='S'))
		{

		}
		else if((c=='T') || (c=='t'))
		{

		}
		else if ((c=='Q') || (c=='q'))
			break;
	 }
	return 0;

}
