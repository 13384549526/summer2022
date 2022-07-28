# RT-Thread夏令营Day3 IPC进程间通信 内存管理

![image-20220726165003233](Day3.assets/image-20220726165003233.png)

![image-20220726165234986](Day3.assets/image-20220726165234986.png)

### 信号量（sem）

![image-20220726165451181](Day3.assets/image-20220726165451181.png)

```c
//动态创建 
rt_sem_t rt_sem_create(const char *name,	//信号量名称
                        rt_uint32_t value,	//信号量初始值
                        rt_uint8_t flag);	//信号量标志,如下
```

![image-20220726171204638](Day3.assets/image-20220726171204638.png)

```c
//静态创建
rt_err_t rt_sem_init(rt_sem_t       sem,	//信号量对象的句柄
                    const char     *name,
                    rt_uint32_t    value,
                    rt_uint8_t     flag)
```

#### 获取信号量

```c
//1
rt_err_t rt_sem_take (rt_sem_t sem, 	//信号量对象的句柄
                      rt_int32_t time);	//指定的等待时间，单位是操作系统时钟节拍（OSTick）
//2
rt_err_t rt_sem_trytake(rt_sem_t sem);	//无等待获取
```

![image-20220726171820517](Day3.assets/image-20220726171820517.png)

#### 释放信号量

```c
rt_err_t rt_sem_release(rt_sem_t sem);
```

![image-20220726172204624](Day3.assets/image-20220726172204624.png)

![image-20220726172440331](Day3.assets/image-20220726172440331.png)

![image-20220726172603937](Day3.assets/image-20220726172603937.png)

![image-20220726172920707](Day3.assets/image-20220726172920707.png)

![image-20220726173104288](Day3.assets/image-20220726173104288.png)

```c
static struct rt_current_priority_semaphore *test_sem = RT_NULL;

void semaphore_test(void)
{
    /* release */
    rt_sem_release(test_sem);
}
MSH_CMD_EXPORT(semaphore_test,  semaphore_test);

void semaphore_dynamic(void)
{
    test_sem = rt_sem_create("test_sem", 0, RT_IPC_FLAG_FIFO);
    while(1)
    {
        rt_sem_take(test_sem, RT_WAITING_FOREVER);
        rt_kprintf("tack semaphore.\n");
    }
    rt_semdelete(test_sem);
}
void semaphore_thread_test(void)
{
    rt_thread_t dynamic_thread = RT_NULL;

    /* creat thread */
    dynamic_thread = rt_thread_create("semaphore", semaphore_dynamic, RT_NULL, 2048, 16, 500);

    rt_thread_startup(dynamic_thread);
}
INIT_APP_EXPORT(semaphore_thread_test);
```

![image-20220726205528284](Day3.assets/image-20220726205528284.png)

![image-20220726205559529](Day3.assets/image-20220726205559529.png)

信号量为5，每次执行，信号量-1直至为0.

![image-20220726210118489](Day3.assets/image-20220726210118489.png)

### 互斥锁（mutex）

![image-20220726204012182](Day3.assets/image-20220726204012182.png)

![image-20220726204033226](Day3.assets/image-20220726204033226.png)

![image-20220726204220108](Day3.assets/image-20220726204220108.png)

![image-20220726204641491](Day3.assets/image-20220726204641491.png)

![image-20220726204728471](Day3.assets/image-20220726204728471.png)

![image-20220726204944012](Day3.assets/image-20220726204944012.png)

![image-20220726205230135](Day3.assets/image-20220726205230135.png)

![image-20220726211326884](Day3.assets/image-20220726211326884.png)

![image-20220726211342863](Day3.assets/image-20220726211342863.png)

### 邮箱（mailbox）

![image-20220726215155324](Day3.assets/image-20220726215155324.png)

<font color="red">不拷贝数据，拷贝地址</font> 

<font color="red">比全局变量实时性高</font> 

![image-20220726215655694](Day3.assets/image-20220726215655694.png)

#### 创建/初始化邮箱

```c
//动态创建
rt_mailbox_t rt_mb_create (const char* name, 	//邮箱名称
                           rt_size_t size, 		//邮箱容量
                           rt_uint8_t flag);	//邮箱标志，它可以取如下数值： RT_IPC_FLAG_FIFO 或 RT_IPC_FLAG_PRIO

//RT_IPC_FLAG_FIFO 属于非实时调度方式，除非应用程序非常在意先来后到，并且你清楚地明白所有涉及到该邮箱的线程都将会变为非实时线程，方可使用 RT_IPC_FLAG_FIFO，否则建议采用 RT_IPC_FLAG_PRIO，即确保线程的实时性。


//静态创建
rt_err_t rt_mb_init(rt_mailbox_t mb,	//邮箱对象的句柄
                    const char* name,	//邮箱名称
                    void* msgpool,		//缓冲区指针 
                    rt_size_t size,		//邮箱容量
                    rt_uint8_t flag);	//邮箱标志，它可以取如下数值： RT_IPC_FLAG_FIFO 或 RT_IPC_FLAG_PRIO
```

![image-20220726220841264](Day3.assets/image-20220726220841264.png)

 ![image-20220726221332942](Day3.assets/image-20220726221332942.png)

![image-20220726222025553](Day3.assets/image-20220726222025553.png)

![image-20220726222419695](Day3.assets/image-20220726222419695.png)

![image-20220726222439948](Day3.assets/image-20220726222439948.png)

### 消息队列（messagequeue）

![image-20220726224100199](Day3.assets/image-20220726224100199.png)

![image-20220726224123294](Day3.assets/image-20220726224123294.png)

![image-20220726224156298](Day3.assets/image-20220726224156298.png)

```c
//动态创建
rt_mq_t rt_mq_create(const char* name, 		//消息队列的名称
                     rt_size_t msg_size,	//消息队列中一条消息的最大长度，单位字节
            		 rt_size_t max_msgs, 	//消息队列的最大个数
                     rt_uint8_t flag);		//消息队列采用的等待方式，它可以取如下数值： RT_IPC_FLAG_FIFO 或 RT_IPC_FLAG_PRIO

//静态创建
rt_err_t rt_mq_init(rt_mq_t mq, 			//消息队列对象的句柄
                    const char* name,		//消息队列的名称
                    void *msgpool, 			//指向存放消息的缓冲区的指针
                    rt_size_t msg_size,		//消息队列中一条消息的最大长度，单位字节
                    rt_size_t pool_size, 	//存放消息的缓冲区大小
                    rt_uint8_t flag);		//消息队列采用的等待方式，它可以取如下数值： RT_IPC_FLAG_FIFO 或 RT_IPC_FLAG_PRIO
```

![image-20220726224843422](Day3.assets/image-20220726224843422.png)

![image-20220726224903551](Day3.assets/image-20220726224903551.png)

![image-20220726224921320](Day3.assets/image-20220726224921320.png)

![image-20220726225027037](Day3.assets/image-20220726225027037.png)

### 事件组（event）

![image-20220726225857123](Day3.assets/image-20220726225857123.png)

![image-20220726225842347](Day3.assets/image-20220726225842347.png)

![image-20220726225915261](Day3.assets/image-20220726225915261.png)

![image-20220726230003079](Day3.assets/image-20220726230003079.png)

![image-20220726230020824](Day3.assets/image-20220726230020824.png)

![image-20220726230035210](Day3.assets/image-20220726230035210.png)

![image-20220726230142110](Day3.assets/image-20220726230142110.png)

![image-20220726230151279](Day3.assets/image-20220726230151279.png)

### 内存管理

![image-20220726234652787](Day3.assets/image-20220726234652787.png)

![image-20220726234707665](Day3.assets/image-20220726234707665.png)

![image-20220727000040518](Day3.assets/image-20220727000040518.png)

 ![image-20220727100830025](Day3.assets/image-20220727100830025.png)

![image-20220727103444349](Day3.assets/image-20220727103444349.png)

![image-20220727104603152](Day3.assets/image-20220727104603152.png)

![image-20220727104754628](Day3.assets/image-20220727104754628.png)

![image-20220727105008876](Day3.assets/image-20220727105008876.png)

![image-20220727105124596](Day3.assets/image-20220727105124596.png)

![image-20220727105244942](Day3.assets/image-20220727105244942.png)

![image-20220727105257136](Day3.assets/image-20220727105257136.png)

![image-20220727105934355](Day3.assets/image-20220727105934355.png)

![image-20220727110100570](Day3.assets/image-20220727110100570.png)

![image-20220727110209540](Day3.assets/image-20220727110209540.png)

![image-20220727110550664](Day3.assets/image-20220727110550664.png)

![image-20220727110612230](Day3.assets/image-20220727110612230.png)

![image-20220727110731787](Day3.assets/image-20220727110731787.png)

![image-20220727110853698](Day3.assets/image-20220727110853698.png)

![image-20220727110944159](Day3.assets/image-20220727110944159.png)

![image-20220727112013696](Day3.assets/image-20220727112013696.png)

关闭之后，无法使用动态创建方式，只可使用静态创建。

![image-20220727112035045](Day3.assets/image-20220727112035045.png)

![image-20220727112220080](Day3.assets/image-20220727112220080.png)

![image-20220727112251766](Day3.assets/image-20220727112251766.png)

![image-20220727112356521](Day3.assets/image-20220727112356521.png)

使用需要打开libc

![image-20220727112439553](Day3.assets/image-20220727112439553.png)

![image-20220727112456561](Day3.assets/image-20220727112456561.png)

### 内存池

![image-20220727112535650](Day3.assets/image-20220727112535650.png)

![image-20220727150348781](Day3.assets/image-20220727150348781.png)

![image-20220727150417664](Day3.assets/image-20220727150417664.png)

![image-20220727150620389](Day3.assets/image-20220727150620389.png)

![image-20220727150739074](Day3.assets/image-20220727150739074.png)

![image-20220727150854037](Day3.assets/image-20220727150854037.png)

![image-20220727150911547](Day3.assets/image-20220727150911547.png)

![image-20220727151004655](Day3.assets/image-20220727151004655.png)

![image-20220727151108637](Day3.assets/image-20220727151108637.png)

![image-20220727151134094](Day3.assets/image-20220727151134094.png)

 ![image-20220727151206632](Day3.assets/image-20220727151206632.png)

![image-20220727151404730](Day3.assets/image-20220727151404730.png)

![image-20220727151459626](Day3.assets/image-20220727151459626.png)

![image-20220727151624428](Day3.assets/image-20220727151624428.png)

![image-20220727151907191](Day3.assets/image-20220727151907191.png)

![image-20220727152031703](Day3.assets/image-20220727152031703.png)

![image-20220727152051931](Day3.assets/image-20220727152051931.png)

![image-20220727152213575](Day3.assets/image-20220727152213575.png)

![image-20220727152229538](Day3.assets/image-20220727152229538.png)

![image-20220727152256905](Day3.assets/image-20220727152256905.png)

![image-20220727152308220](Day3.assets/image-20220727152308220.png)

![image-20220727152324599](Day3.assets/image-20220727152324599.png)

![image-20220727152446720](Day3.assets/image-20220727152446720.png)

![image-20220727152739236](Day3.assets/image-20220727152739236.png)

![image-20220727152821444](Day3.assets/image-20220727152821444.png)

![image-20220727153106244](Day3.assets/image-20220727153106244.png)

![image-20220727153316294](Day3.assets/image-20220727153316294.png)

![image-20220727180831196](Day3.assets/image-20220727180831196.png)

![image-20220727180957053](Day3.assets/image-20220727180957053.png)

![image-20220727181353984](Day3.assets/image-20220727181353984.png)

![image-20220727181718961](Day3.assets/image-20220727181718961.png)

![image-20220727214412475](Day3.assets/image-20220727214412475.png)

![image-20220727214816296](Day3.assets/image-20220727214816296.png)

![image-20220727214837032](Day3.assets/image-20220727214837032.png)

![image-20220727214912152](Day3.assets/image-20220727214912152.png)

![image-20220727214934859](Day3.assets/image-20220727214934859.png)

![image-20220727214950619](Day3.assets/image-20220727214950619.png)

![image-20220727215112842](Day3.assets/image-20220727215112842.png)

![image-20220727215126525](Day3.assets/image-20220727215126525.png)

![image-20220727215255191](Day3.assets/image-20220727215255191.png)![image-20220727215255367](Day3.assets/image-20220727215255367.png)

![image-20220727224642581](Day3.assets/image-20220727224642581.png)

![image-20220727225309981](Day3.assets/image-20220727225309981.png)

![image-20220727225324937](Day3.assets/image-20220727225324937.png)

![image-20220727225339313](Day3.assets/image-20220727225339313.png)

### IPC讲解

![image-20220728000936022](Day3.assets/image-20220728000936022.png)

![image-20220728001034724](Day3.assets/image-20220728001034724.png)

强制挂起线程危险原因，挂起后无操作，应配合复位使用

