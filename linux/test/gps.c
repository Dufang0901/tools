#include<stdio.h>
/*********************************************************
    Function:       GpsLoLaDmToD
    Description:    ����γ����Ϣ�Ӷȷֽṹ����ɴ��Ƚṹ
    Param:          lolaDm
    Return:         lolaD
    Other:          �� ddmm.mmmm ��ʽת�� С�����ʽ 
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
    Description:    ����γ����Ϣ�Ӵ��Ƚṹ����ɶȷֽṹ
    Param:          lolaDm
    Return:         lolaD
    Other:          �� ddmm.mmmm ��ʽת�� С�����ʽ 
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
