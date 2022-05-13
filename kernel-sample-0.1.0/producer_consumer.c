/* 
 * Copyright (c) 2006-2018, RT-Thread Development Team 
 * 
 * SPDX-License-Identifier: Apache-2.0 
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2018-08-24     yangjie      the first version 
 */  

/*
 * 程序清单：生产者消费者例子
 *
 * 这个例子中将创建两个线程用于实现生产者消费者问题
 *（1）生产者线程将cnt值每次加1并循环存入array数组的5个成员内；
 *（2）消费者线程将生产者中生产的数值打印出来，并累加求和
 */
#include <rtthread.h>

#define THREAD_PRIORITY       6
#define THREAD_STACK_SIZE     512
#define THREAD_TIMESLICE      5

#define MAXSEM 5

rt_uint32_t array[MAXSEM];

static rt_uint32_t set , get ;

static rt_thread_t producer_tid = RT_NULL;
static rt_thread_t consumer_tid = RT_NULL;

struct rt_semaphore sem_lock;
struct rt_semaphore sem_empty , sem_full;

void producer_thread_entry(void* parameter)
{
	int cnt = 0;
	
	while(cnt < 10)
	{
		rt_sem_take(&sem_empty,RT_WAITING_FOREVER);
		rt_sem_take(&sem_lock,RT_WAITING_FOREVER);
		
		array[set % MAXSEM] = cnt+1;
		rt_kprintf("the producer generates a number: %d \n",array[set % MAXSEM]);
		set++;
		rt_sem_release(&sem_lock);
		
		rt_sem_release(&sem_full);
		cnt++;
		
		rt_thread_mdelay(20);
	}
}

void consumer_thread_entry(void * parameter)
{
	rt_uint32_t sum = 0;
	
	while(1)
	{
		rt_sem_take(&sem_full,RT_WAITING_FOREVER);
		
		rt_sem_take(&sem_lock,RT_WAITING_FOREVER);
		sum += array[get % MAXSEM];
		rt_kprintf("the consumer[%d] get a number\n",array[get % MAXSEM]);
		get ++;
		rt_sem_release(&sem_lock);
		rt_sem_release(&sem_empty);
		
		if(get == 10) break;
		
		rt_thread_mdelay(50);
	}	
	rt_kprintf("the consumer sum is : %d\n",sum);
	rt_kprintf("the consumer exit!\n");
}

int producer_consumer(void)
{
	set = 0;
	get = 0;
	
	rt_sem_init(&sem_lock, "lock",     1,      RT_IPC_FLAG_FIFO);
  rt_sem_init(&sem_empty, "empty",   MAXSEM, RT_IPC_FLAG_FIFO);
  rt_sem_init(&sem_full, "full",     0,      RT_IPC_FLAG_FIFO);
	
	producer_tid = rt_thread_create("producer",
                                    producer_thread_entry, RT_NULL,
                                    THREAD_STACK_SIZE,
                                    THREAD_PRIORITY - 1, THREAD_TIMESLICE);
    if (producer_tid != RT_NULL)
    {
        rt_thread_startup(producer_tid);
    }
    else
    {
        rt_kprintf("create thread producer failed");
        return -1;
    }

    consumer_tid = rt_thread_create("consumer",
                                    consumer_thread_entry, RT_NULL,
                                    THREAD_STACK_SIZE,
                                    THREAD_PRIORITY + 1, THREAD_TIMESLICE);
    if (consumer_tid != RT_NULL)
    {
        rt_thread_startup(consumer_tid);
    }
    else
    {
        rt_kprintf("create thread consumer failed");
        return -1;
    }

    return 0;
}

MSH_CMD_EXPORT(producer_consumer, producer_consumer sample);




