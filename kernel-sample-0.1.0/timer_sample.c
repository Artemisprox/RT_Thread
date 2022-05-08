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
* 程序清单：定时器例程
*
* 这个例程会创建两个动态定时器，一个是单次定时，一个是周期性定时
* 并让周期定时器运行一段时间后停止运行
*/
#include <rtthread.h>

static rt_timer_t timer1;
static rt_timer_t timer2;

static void timer1out(void *parameter)
{
	static rt_uint16_t count = 0;
	rt_kprintf("timer1 : %d\n" , count);
	count ++;
	if(count >= 9)
	{
	rt_timer_stop(timer1);
	rt_kprintf("time1 stopped\n");
	}
}

static void timer2out(void *parameter)
{
	rt_kprintf("timer2\n");
}

int timer_sample(void)
{
	timer1 = rt_timer_create("timer1",timer1out,RT_NULL,10,RT_TIMER_FLAG_PERIODIC);
	if(timer1 != RT_NULL) rt_timer_start(timer1);
	
	timer2 = rt_timer_create("timer2",timer2out,RT_NULL,30,RT_TIMER_FLAG_ONE_SHOT);
	if(timer2 != RT_NULL) rt_timer_start(timer2);
	
	return 0;
}	

MSH_CMD_EXPORT(timer_sample, timer sample);
