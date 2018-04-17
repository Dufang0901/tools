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
        //这个mutex主要是用来保证pthread_cond_wait的并发性 
	    printf("thread attempt on mutex.\n");
        pthread_mutex_lock(&mtx);  
		printf("thread got mutex.\n");
        while (head == NULL)  
        {  
            //这个while要特别说明一下，单个pthread_cond_wait功能很完善，为何  
            //这里要有一个while (head == NULL)呢？因为pthread_cond_wait里的线  
            //程可能会被意外唤醒，如果这个时候head != NULL，则不是我们想要的情况。  
            //这个时候，应该让线程继续进入pthread_cond_wait  
            // pthread_cond_wait会先解除之前的pthread_mutex_lock锁定的mtx，  
            //然后阻塞在等待对列里休眠，直到再次被唤醒(大多数情况下是等待的条件成立  
            //而被唤醒，唤醒后，该进程会先锁定先pthread_mutex_lock(&mtx);，再读取资源  
            //用这个流程是比较清楚的  
			
			printf("thread waiting...\n");
            pthread_cond_wait(&cond, &mtx); 
			printf("thread finished waiting\n");
			
            p = head;  
            head = head->n_next;  
            printf("Got %d from front of queue\n", p->n_number);  
            free(p);  
        }  
		printf("thread unlock mutex.\n");
        pthread_mutex_unlock(&mtx); //临界区数据操作完毕，释放互斥锁  
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
    //子线程会一直等待资源，类似生产者和消费者，但是这里的消费者可以是多个消费者，而  
    //不仅仅支持普通的单个消费者，这个模型虽然简单，但是很强大  
    pthread_create(&tid, NULL, thread_func, NULL);  
    sleep(1);  
    for (i = 0; i < 5; i++)  
    {  
        p = (struct node*)malloc(sizeof(struct node));  
        p->n_number = i;  
		
		printf("main attempt on mutex.\n");
        pthread_mutex_lock(&mtx); //需要操作head这个临界资源，先加锁，
		printf("main got mutex.\n");	
		
        p->n_next = head;  
        head = p;  
		
		printf("main release signal.\n");
        pthread_cond_signal(&cond);  
        pthread_mutex_unlock(&mtx); //解锁  
        sleep(1);  
    }  
    printf("thread 1 wanna end the line.So cancel thread 2.\n");  
    //关于pthread_cancel，有一点额外的说明，它是从外部终止子线程，子线程会在最近的取消点，退出  
    //线程，而在我们的代码里，最近的取消点肯定就是pthread_cond_wait()了。  
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