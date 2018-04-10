#include <pthread.h>  
#include <unistd.h>  
#include "stdio.h"  
#include "stdlib.h"  
static pthread_mutex_t mtx;  
static pthread_cond_t  cond;  

#if 0
struct node  
{  
    int n_number;  
    struct node *n_next;  
}*head = NULL;  
  
static void cleanup_handler(void *arg)  
{  
    printf("Cleanup handler of second thread.\n");  
    free(arg);  
    (void)pthread_mutex_unlock(&mtx);  
}  
static void *thread_func(void *arg)  
{  
    struct node *p = NULL;  
    pthread_cleanup_push(cleanup_handler, p);  
    while (1)  
    {  
        //���mutex��Ҫ��������֤pthread_cond_wait�Ĳ����� 
	    printf("thread attempt on mutex.\n");
        pthread_mutex_lock(&mtx);  
		printf("thread got mutex.\n");
        while (head == NULL)  
        {  
            //���whileҪ�ر�˵��һ�£�����pthread_cond_wait���ܺ����ƣ�Ϊ��  
            //����Ҫ��һ��while (head == NULL)�أ���Ϊpthread_cond_wait�����  
            //�̿��ܻᱻ���⻽�ѣ�������ʱ��head != NULL������������Ҫ�������  
            //���ʱ��Ӧ�����̼߳�������pthread_cond_wait  
            // pthread_cond_wait���Ƚ��֮ǰ��pthread_mutex_lock������mtx��  
            //Ȼ�������ڵȴ����������ߣ�ֱ���ٴα�����(�����������ǵȴ�����������  
            //�������ѣ����Ѻ󣬸ý��̻���������pthread_mutex_lock(&mtx);���ٶ�ȡ��Դ  
            //����������ǱȽ������  
			
			printf("thread waiting...\n");
            pthread_cond_wait(&cond, &mtx); 
			printf("thread finished waiting\n");
			
            p = head;  
            head = head->n_next;  
            printf("Got %d from front of queue\n", p->n_number);  
            free(p);  
        }  
		printf("thread unlock mutex.\n");
        pthread_mutex_unlock(&mtx); //�ٽ������ݲ�����ϣ��ͷŻ�����  
    }  
    pthread_cleanup_pop(0);  
    return 0;  
}  
int main(void)  
{  
    pthread_t tid;  
    int i;  
    struct node *p;  
	
	pthread_mutex_init(&mtx, NULL);  
    pthread_cond_init(&cond, NULL);  
    //���̻߳�һֱ�ȴ���Դ�����������ߺ������ߣ���������������߿����Ƕ�������ߣ���  
    //������֧����ͨ�ĵ��������ߣ����ģ����Ȼ�򵥣����Ǻ�ǿ��  
    pthread_create(&tid, NULL, thread_func, NULL);  
    sleep(1);  
    for (i = 0; i < 5; i++)  
    {  
        p = (struct node*)malloc(sizeof(struct node));  
        p->n_number = i;  
		
		printf("main attempt on mutex.\n");
        pthread_mutex_lock(&mtx); //��Ҫ����head����ٽ���Դ���ȼ�����
		printf("main got mutex.\n");	
		
        p->n_next = head;  
        head = p;  
		
		printf("main release signal.\n");
        pthread_cond_signal(&cond);  
        pthread_mutex_unlock(&mtx); //����  
        sleep(1);  
    }  
    printf("thread 1 wanna end the line.So cancel thread 2.\n");  
    //����pthread_cancel����һ������˵�������Ǵ��ⲿ��ֹ���̣߳����̻߳��������ȡ���㣬�˳�  
    //�̣߳��������ǵĴ���������ȡ����϶�����pthread_cond_wait()�ˡ�  
    pthread_cancel(tid);  
    pthread_join(tid, NULL);  
    printf("All done -- exiting\n");  
    return 0;  
}  
#endif



#if 1
void hander(void *arg)  
{  
    free(arg);  
	
    (void)pthread_mutex_unlock(&mtx); 
	printf("handler unlock mutex.\n");	
}  
void *thread1(void *arg)  
{  
	int ret;
    pthread_cleanup_push(hander, &mtx);  
    while(1)  
    {  
        printf("thread1 attempt on mutex.\n");  
        ret = pthread_mutex_lock(&mtx);
		printf("thread1 got mutex and begin waiting...\n");
        pthread_cond_wait(&cond, &mtx);  
        printf("thread1 applied the condition\n");  
		//sleep(10);  
        pthread_mutex_unlock(&mtx); 
		printf("thread1 unlock mutex.\n");
		
        
    }  
    pthread_cleanup_pop(0);  
}  
void *thread2(void *arg)  
{  
	int ret;
    while(1)  
    {  
        printf("thread2 attempt on mutex.\n");  
        ret = pthread_mutex_lock(&mtx);  
		printf("thread2 got mutex and begin waiting...\n");
        pthread_cond_wait(&cond, &mtx);  
        printf("thread2 applied the condition\n");  
        pthread_mutex_unlock(&mtx); 
		printf("thread2 unlock mutex.\n");	
       
    }  
}  
int main()  
{  
    pthread_t thid1,thid2;  
    printf("condition variable study!\n");  
    pthread_mutex_init(&mtx, NULL);  
    pthread_cond_init(&cond, NULL);  
    pthread_create(&thid1, NULL, thread1, NULL);  
    pthread_create(&thid2, NULL, thread2, NULL);  
   // sleep(1);  
    do  
    {  
        pthread_cond_signal(&cond); 
		printf("main release signal and sleep...\n");
		sleep(10);		
    }while(1);  
    sleep(20);  
    pthread_exit(0);  
    return 0;  
}  
#endif




#if 0
void  *thrd_1( void *arg)
{
	printf("thread1 tempt on mutex...\n");
	pthread_mutex_lock(&mtx);
	printf("thred_1..test1\n");
	sleep(2);
	printf("thred_1...test2\n");
	sleep(2);
	printf("thred_1..test3\n");
	sleep(2);
	printf("thred_1..test4\n");
	sleep(2);
	printf("thred_1..test5\n");
	sleep(2);
	printf("thred_1..test6\n");
	sleep(2);
	pthread_mutex_unlock(&mtx);
}


void *thrd_2(void *arg)
{
	printf("thread2 tempt on mutex...\n");
	pthread_mutex_lock(&mtx);
	sleep(2);
	printf("thred_2..test2\n");
	printf("thred_2..test1\n");
	sleep(2);
	printf("thred_2..test3\n");
	sleep(2);
	printf("thred_2..test4\n");
	sleep(2);
	printf("thred_2..test5\n");
	sleep(2);
	printf("thred_2..test6\n");
	sleep(2);
	pthread_mutex_unlock(&mtx);
}

void *thrd_3()
{
	printf("thrd_3....test1\n");
	sleep(2);
	printf("thrd_3....test2\n");
	sleep(2);
	printf("thrd_3....test3\n");
	sleep(2);
	printf("thrd_3....test4\n");
	sleep(2);
	printf("thrd_3....test5\n");
	sleep(2);
	printf("thrd_3....test6\n");
}
int main()
{
	int ret;
	pthread_t tid1, tid2, tid3;
	pthread_mutex_init(&mtx, NULL);  
	ret = pthread_create(&tid1,NULL,thrd_1, NULL);
	ret = pthread_create(&tid2,NULL,thrd_2, NULL);
	ret = pthread_create(&tid3,NULL,thrd_3, NULL);
	while(1){
		sleep(2);
	};
	return 0;
}
#endif