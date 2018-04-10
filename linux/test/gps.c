#include<stdio.h>
/*********************************************************
    Function:       GpsLoLaDmToD
    Description:    将经纬度消息从度分结构换算成纯度结构
    Param:          lolaDm
    Return:         lolaD
    Other:          由 ddmm.mmmm 格式转换 小数点格式 
*********************************************************/
double GpsLoLaDmToD(double lolaDm)
{
    double lolaDTemp;
    int lolaDmTemp = (int)lolaDm;

    lolaDmTemp /= 100;

    lolaDTemp = (lolaDm - (double)(lolaDmTemp * 100)) / 60 + (double)lolaDmTemp;
    
    return lolaDTemp;
    
}
/*********************************************************
    Function:       GpsLoLaDToDm
    Description:    将经纬度消息从纯度结构换算成度分结构
    Param:          lolaDm
    Return:         lolaD
    Other:          由 ddmm.mmmm 格式转换 小数点格式 
*********************************************************/
double GpsLoLaDToDm(double lolaD)
{
	double lolaDmTemp;
	int lolaDTemp = (int)lolaD;

	lolaDmTemp = lolaDTemp * 100 + (lolaD - lolaDTemp) * 60;
	return lolaDmTemp;
}

void main()
{
	double originalDm = 2309.73180;
	double originalD  = 113.4132;
	double d,dm;
	d = GpsLoLaDmToD(originalDm);
	dm = GpsLoLaDToDm(d);
	printf("originalDm = %f\n",originalDm);
	printf("after 2 transition = %f\n",dm);
	dm = GpsLoLaDToDm(originalD);
	d  = GpsLoLaDmToD(dm);
	printf("originalD=%f\n",originalD);
	printf("dm=%f\n",dm);
	printf("after 2 transition=%f\n",d);
}
