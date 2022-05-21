#include <rtthread.h>

#define THREAD_STACK_SIZE  1024
#define THREAD_PRIORITY    20
#define THREAD_TIMESLICE   5

static rt_thread_t tid1 = RT_NULL;
struct rt_semaphore sem_lock_LED,sem_LED1,sem_LED2,sem_LED3;
static rt_timer_t timer1;
rt_uint16_t Turn;

void KEY(void *parameter)
{
	rt_sem_take(&sem_lock_LED,RT_WAITING_FOREVER);
	switch(Turn)
	{
		case 0 : rt_sem_release(&sem_LED1);break;
		case 1 : rt_sem_release(&sem_LED2);break;
		case 2 : rt_sem_release(&sem_LED3);break;
    case 3 : rt_sem_release(&sem_LED1);break;		
	}
	rt_sem_release(&sem_lock_LED);
}	

void LED_type1(void* parameter)
{
  while(1)
	{
		rt_sem_take(&sem_LED1,RT_WAITING_FOREVER);
		rt_sem_take(&sem_lock_LED,RT_WAITING_FOREVER);
		Turn = 1 ;
		rt_kprintf("LED TYPE 1\n");
		rt_sem_release(&sem_lock_LED);
		rt_thread_mdelay(10);
	}
}

static rt_thread_t tid2 = RT_NULL;

void LED_type2(void* parameter)
{
  while(1)
	{
		rt_sem_take(&sem_LED2,RT_WAITING_FOREVER);
		rt_sem_take(&sem_lock_LED,RT_WAITING_FOREVER);
		Turn = 2;
		rt_kprintf("LED TYPE 2\n");
		rt_sem_release(&sem_lock_LED);
		rt_thread_mdelay(10);
	}
}

static rt_thread_t tid3 = RT_NULL;

void LED_type3(void* parameter)
{
  while(1)
	{
		rt_sem_take(&sem_LED3,RT_WAITING_FOREVER);
		rt_sem_take(&sem_lock_LED,RT_WAITING_FOREVER);
		Turn = 3 ;
		rt_kprintf("LED TYPE 3\n");
		rt_sem_release(&sem_lock_LED);
		rt_thread_mdelay(10);
	}
}

int Flow_LED(void)
{
	Turn = 0;
	
	rt_sem_init(&sem_lock_LED, "lock",     1,      RT_IPC_FLAG_FIFO);
	rt_sem_init(&sem_LED1, "LED1",     0,      RT_IPC_FLAG_FIFO);
	rt_sem_init(&sem_LED2, "LED2",     0,      RT_IPC_FLAG_FIFO);
	rt_sem_init(&sem_LED3, "LED3",     0,      RT_IPC_FLAG_FIFO);
	
	timer1 = rt_timer_create("timer1",KEY,RT_NULL,15,RT_TIMER_FLAG_PERIODIC);
	if(timer1 != RT_NULL) 
		rt_timer_start(timer1);
	
	tid1 = rt_thread_create("LED_type1",LED_type1,RT_NULL,THREAD_STACK_SIZE,THREAD_PRIORITY+1,THREAD_TIMESLICE);
	if (tid1 != RT_NULL)
		rt_thread_startup(tid1);
	
	tid2 = rt_thread_create("LED_type2",LED_type2,RT_NULL,THREAD_STACK_SIZE,THREAD_PRIORITY+1,THREAD_TIMESLICE);
	if (tid2 != RT_NULL)
		rt_thread_startup(tid2);
	
	tid3 = rt_thread_create("LED_type3",LED_type3,RT_NULL,THREAD_STACK_SIZE,THREAD_PRIORITY+1,THREAD_TIMESLICE);
	if (tid3 != RT_NULL)
		rt_thread_startup(tid3);
	
	
}

MSH_CMD_EXPORT(Flow_LED,flow_LED);