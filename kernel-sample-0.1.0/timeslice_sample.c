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
 * 程序清单：相同优先级线程按照时间片轮番调度
 *
 * 这个例子中将创建两个线程，每一个线程都在打印信息
 * 
 */

#include <rtthread.h>

#define THREAD_STACK_SIZE	1024
#define THREAD_PRIORITY	    20
#define THREAD_TIMESLICE    8

static void thread_entry(void * parameter)
{
	rt_uint32_t value = 0;
	rt_uint32_t count = 0;
	
	value = (rt_uint32_t)parameter;
	while(1)
	{
		if( 0 == (count % 5))
		{
			rt_kprintf("thread%d : %d \n",value,count);
			
			if(count > 300)
				return ;
		}
		count ++;
	}	
	
}

int timeslice(void)
{
	rt_thread_t tid;
	tid = rt_thread_create("thread1",thread_entry,(void*)1,THREAD_STACK_SIZE,THREAD_PRIORITY,THREAD_TIMESLICE);
	if(tid != RT_NULL)
		rt_thread_startup(tid);
	
	tid = rt_thread_create("thread2",thread_entry,(void*)2,THREAD_STACK_SIZE,THREAD_PRIORITY,THREAD_TIMESLICE-5);
	if(tid != RT_NULL)
		rt_thread_startup(tid);
	return 0;
}

MSH_CMD_EXPORT(timeslice, timeslice);