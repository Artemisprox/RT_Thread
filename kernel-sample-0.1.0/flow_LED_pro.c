#include <rtthread.h>

static struct rt_messagequeue mq;
static rt_uint8_t msg_pool[2048];

static rt_thread_t tid1 = RT_NULL;
static rt_thread_t tid2 = RT_NULL;

static void thread1_entry(void *parameter)
{
    rt_uint8_t buf = 0;

    while (1)
    {
        /* 从消息队列中接收消息ds7 */
        if (rt_mq_recv(&mq, &buf, sizeof(buf), RT_WAITING_FOREVER) == RT_EOK)
        {
           switch(buf)
					 {	 
						 case 1: rt_kprintf("LED1\n");break;
					   case 2: rt_kprintf("LED2\n");break;
					   case 3: rt_kprintf("LED3\n");break;
					 }
        }
    }
    
}

static void thread2_entry(void *parameter)
{
    rt_uint8_t cnt = 0;
	  rt_uint8_t result;
	
    while (1)
    {
        result = rt_mq_send(&mq, &cnt, 1);
            if (result != RT_EOK)
            {
                rt_kprintf("rt_mq_send ERR\n");
            }else{
							rt_kprintf("LED%d on \n",cnt);
						}
        if(cnt == 3)  cnt = 0;
        cnt++;
        rt_thread_mdelay(100);
    }
}

int flow_LED_pro(void)
{
    rt_err_t result;

    result = rt_mq_init(&mq,
                        "mqt",
                        &msg_pool[0],               
                        1,                          
                        sizeof(msg_pool),           
                        RT_IPC_FLAG_FIFO);          

    if (result != RT_EOK)
    {
        rt_kprintf("init message queue failed.\n");
        return -1;
    }

    tid1 = rt_thread_create("thread1",
                            thread1_entry, RT_NULL,
                            1024,
                            21, 5);
    
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);

    tid2 = rt_thread_create("thread2",
                            thread2_entry, RT_NULL,
                            1024,
                            20, 5);
    
    if (tid2 != RT_NULL)
        rt_thread_startup(tid2);

    return 0;
}

/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(flow_LED_pro,  flow_LED_pro);