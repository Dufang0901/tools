#ifndef _GET_CPU_H_
#define _GET_CPU_H_

#define VMRSS_LINE 15//VMRSS������
#define PROCESS_ITEM 14//����CPUʱ�俪ʼ������

typedef struct        //����һ��occupy�Ľṹ��
{
        unsigned int user;  //��ϵͳ������ʼ�ۼƵ���ǰʱ�̣������û�̬������ʱ�䣬������ niceֵΪ�����̡�
        unsigned int nice;  //��ϵͳ������ʼ�ۼƵ���ǰʱ�̣�niceֵΪ���Ľ�����ռ�õ�CPUʱ��
        unsigned int system;//��ϵͳ������ʼ�ۼƵ���ǰʱ�̣����ں���̬������ʱ��
        unsigned int idle;  //��ϵͳ������ʼ�ۼƵ���ǰʱ�̣���IO�ȴ�ʱ������������ȴ�ʱ��iowait (12256) ��ϵͳ������ʼ�ۼƵ���ǰʱ�̣�IO�ȴ�ʱ��(since 2.5.41)
}total_cpu_occupy_t;

typedef struct
{
    pid_t pid;//pid��
    unsigned int utime;  //���������û�̬���е�ʱ�䣬��λΪjiffies
    unsigned int stime;  //�������ں���̬���е�ʱ�䣬��λΪjiffies
    unsigned int cutime;//���������߳����û�̬���е�ʱ�䣬��λΪjiffies
    unsigned int cstime;  //���������ں���̬���е�ʱ�䣬��λΪjiffies
}process_cpu_occupy_t;

int get_phy_mem(const pid_t p);         //��ȡռ�������ڴ�
int get_total_mem();                    //��ȡϵͳ���ڴ�
unsigned int get_cpu_total_occupy();    //��ȡ�ܵ�CPUʱ��
unsigned int get_cpu_idle_occupy();     //��ȡ���е�cpuʱ��
unsigned int get_cpu_process_occupy(const pid_t p);//��ȡ���̵�CPUʱ��
const char* get_items(const char* buffer,int ie);//ȡ�û�����ָ�������ʼ��ַ

float get_pcpu(pid_t p);//��ȡ����CPUռ��
float get_pcpuTotal();  //��ȡ����cpuռ����
float get_pmem(pid_t p);//��ȡ�����ڴ�ռ��
int get_rmem(pid_t p);  //��ȡ��ʵ�����ڴ�
float get_pmemTotal();    
    
#endif