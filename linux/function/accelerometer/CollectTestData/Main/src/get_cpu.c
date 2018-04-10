#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   //ͷ�ļ�
#include <assert.h>
#include "get_cpu.h"

int get_phy_mem(const pid_t p)
{
    char file[64] = {0};//�ļ���
  
    FILE *fd;         //�����ļ�ָ��fd
    char line_buff[256] = {0};  //��ȡ�еĻ�����
    sprintf(file,"/proc/%d/status",p);//�ļ��е�11�а�����

    fprintf (stderr, "current pid:%d\n", p);                                                                                                  
    fd = fopen (file, "r"); //��R���ķ�ʽ���ļ��ٸ���ָ��fd

    //��ȡvmrss:ʵ�������ڴ�ռ��
    int i;
    char name[32];//�����Ŀ����
    int vmrss;//����ڴ��ֵ��С
    for (i=0;i<VMRSS_LINE-1;i++)
    {
        fgets (line_buff, sizeof(line_buff), fd);
    }//������15��
    fgets (line_buff, sizeof(line_buff), fd);//��ȡVmRSS��һ�е�����,VmRSS�ڵ�15��
    sscanf (line_buff, "%s %d", name,&vmrss);
    fprintf (stderr, "====%s��%d====\n", name,vmrss);
    fclose(fd);     //�ر��ļ�fd
    return vmrss;
}

int get_rmem(pid_t p)
{
    return get_phy_mem(p);
}


int get_total_mem()
{
    char* file = "/proc/meminfo";//�ļ���
  
    FILE *fd;         //�����ļ�ָ��fd
    char line_buff[256] = {0};  //��ȡ�еĻ�����                                                                                                
    fd = fopen (file, "r"); //��R���ķ�ʽ���ļ��ٸ���ָ��fd

    //��ȡmemtotal:���ڴ�ռ�ô�С
    int i;
    char name[32];//�����Ŀ����
    int memtotal;//����ڴ��ֵ��С
    fgets (line_buff, sizeof(line_buff), fd);//��ȡmemtotal��һ�е�����,memtotal�ڵ�1��
    sscanf (line_buff, "%s %d", name,&memtotal);
    fprintf (stderr, "====%s��%d====\n", name,memtotal);
    fclose(fd);     //�ر��ļ�fd
    return memtotal;
}

float get_pmem(pid_t p)
{
    int phy = get_phy_mem(p);
      int total = get_total_mem();
      float occupy = (phy*1.0)/(total*1.0);
      fprintf(stderr,"====process mem occupy:%.6f\n====",occupy);
      return occupy;
}

float get_pmemTotal()
{
    char buf_ps[256]={0};
    char ps[256]={0};
    char result[256]={0};
    FILE *ptr;
    strcpy(ps, "free m | grep Mem");
    if((ptr=popen(ps, "r"))!=NULL)
    {
        while(fgets(buf_ps, 1024, ptr)!=NULL)
        {
           strcat(result, buf_ps);
           if(strlen(result)>1024)
               break;
        }
        pclose(ptr);
        ptr = NULL;
        //Mem:        247920        54500       193420          936            0
        int totalMem = 0;
        int usedMem = 0;
        float occupy;
        sscanf(result,"Mem: %d  %d",&totalMem, &usedMem);
        occupy = usedMem*1.0/totalMem;
        fprintf(stderr,"====process mem occupy: %d/%d = %.3f\n====",usedMem,totalMem, occupy);
        return occupy;
    }
    else
    {
        printf("popen %s error\n", ps);
        return -1;
    }
}

unsigned int get_cpu_process_occupy(const pid_t p)
{
    char file[64] = {0};//�ļ���
    process_cpu_occupy_t t;
  
    FILE *fd;         //�����ļ�ָ��fd
    char line_buff[1024] = {0};  //��ȡ�еĻ�����
    sprintf(file,"/proc/%d/stat",p);//�ļ��е�11�а�����

    fprintf (stderr, "current pid:%d\n", p);                                                                                                  
    fd = fopen (file, "r"); //��R���ķ�ʽ���ļ��ٸ���ָ��fd
    fgets (line_buff, sizeof(line_buff), fd); //��fd�ļ��ж�ȡ����Ϊbuff���ַ����ٴ浽��ʼ��ַΪbuff����ռ���

    sscanf(line_buff,"%u",&t.pid);//ȡ�õ�һ��
    char* q = get_items(line_buff,PROCESS_ITEM);//ȡ�ôӵ�14�ʼ����ʼָ��
    sscanf(q,"%u %u %u %u",&t.utime,&t.stime,&t.cutime,&t.cstime);//��ʽ����14,15,16,17��

    fprintf (stderr, "====pid%u:%u %u %u %u====\n", t.pid, t.utime,t.stime,t.cutime,t.cstime);
    fclose(fd);     //�ر��ļ�fd
    return (t.utime + t.stime + t.cutime + t.cstime);
}


unsigned int get_cpu_total_occupy()
{
    FILE *fd;         //�����ļ�ָ��fd
    char buff[1024] = {0};  //����ֲ�����buff����Ϊchar���ʹ�СΪ1024
    total_cpu_occupy_t t;
                                                                                                             
    fd = fopen ("/proc/stat", "r"); //��R���ķ�ʽ��stat�ļ��ٸ���ָ��fd
    fgets (buff, sizeof(buff), fd); //��fd�ļ��ж�ȡ����Ϊbuff���ַ����ٴ浽��ʼ��ַΪbuff����ռ���
    /*�����ǽ�buff���ַ������ݲ���format��ת��Ϊ���ݵĽ��������Ӧ�Ľṹ����� */
    char name[16];//��ʱ��������ַ���
    sscanf (buff, "%s %u %u %u %u", name, &t.user, &t.nice,&t.system, &t.idle);
    

    fprintf (stderr, "====%s:%u %u %u %u====\n", name, t.user, t.nice,t.system, t.idle);
    fclose(fd);     //�ر��ļ�fd
    return (t.user + t.nice + t.system + t.idle);
}

unsigned int get_cpu_idle_occupy()
{
    FILE *fd;         //�����ļ�ָ��fd
    char buff[1024] = {0};  //����ֲ�����buff����Ϊchar���ʹ�СΪ1024
    total_cpu_occupy_t t;
                                                                                                             
    fd = fopen ("/proc/stat", "r"); //��R���ķ�ʽ��stat�ļ��ٸ���ָ��fd
    fgets (buff, sizeof(buff), fd); //��fd�ļ��ж�ȡ����Ϊbuff���ַ����ٴ浽��ʼ��ַΪbuff����ռ���
    /*�����ǽ�buff���ַ������ݲ���format��ת��Ϊ���ݵĽ��������Ӧ�Ľṹ����� */
    char name[16];//��ʱ��������ַ���
    sscanf (buff, "%s %u %u %u %u", name, &t.user, &t.nice,&t.system, &t.idle);
    

    fprintf (stderr, "====%s:%u %u %u %u====\n", name, t.user, t.nice,t.system, t.idle);
    fclose(fd);     //�ر��ļ�fd
    return (t.idle);
}


float get_pcpu(pid_t p)
{
    unsigned int totalcputime1,totalcputime2;
      unsigned int procputime1,procputime2;
    totalcputime1 = get_cpu_total_occupy();
    procputime1 = get_cpu_process_occupy(p);
    usleep(500000);//�ӳ�500����
    totalcputime2 = get_cpu_total_occupy();
    procputime2 = get_cpu_process_occupy(p);
    float pcpu = 100.0*(procputime2 - procputime1)/(totalcputime2 - totalcputime1);
    fprintf(stderr,"====pcpu:%.6f\n====",pcpu);
    return pcpu;
}

float get_pcpuTotal()
{
    unsigned int totalcputime1,totalcputime2;
    unsigned int procputime1,procputime2;
    totalcputime1 = get_cpu_total_occupy();
    procputime1 = get_cpu_idle_occupy();
    sleep(1);//�ӳ�500����
    totalcputime2 = get_cpu_total_occupy();
    procputime2 = get_cpu_idle_occupy();
    
    float pcpu = ((totalcputime2 - totalcputime1) - (procputime2 - procputime1))*1.0/(totalcputime2 - totalcputime1);
    fprintf(stderr,"====pcpu:%.6f\n====",pcpu);
    return pcpu;
}

const char* get_items(const char* buffer,int ie)
{
    assert(buffer);
    char* p = buffer;//ָ�򻺳���
    int len = strlen(buffer);
    int count = 0;//ͳ�ƿո���
    if (1 == ie || ie < 1)
    {
        return p;
    }
    int i;
    
    for (i=0; i<len; i++)
    {
        if (' ' == *p)
        {
            count++;
            if (count == ie-1)
            {
                p++;
                break;
            }
        }
        p++;
    }

    return p;
}
