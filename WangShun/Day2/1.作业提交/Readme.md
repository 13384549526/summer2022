# IPC能够挂起唤醒线程的原因
RT-Thread中能实现挂起的基本函数为 `rt_thread_suspend()`，唤醒的基本函数为`rt_thread_resume`；
RT-Thread的IPC有`信号量`、`互斥量`、`事件`、`邮箱`、`消息队列`
#
`当某个线程通过IPC请求资源的时候当前IPC没有资源可以请求时，此时发出请求的线程若设置了等待就会被挂起，转为挂起态（若没有设置等待就会返回错误），等到IPC有可用的资源时就会唤醒该线程将其重新设置为就绪态。`
# 线程切换的概念
线程切换发生在线程与线程之间，当线程在执行一些不必要的任务的时候就会主动交出CPU的使用权，或者当线程在请求一些资源时请求不到也会被迫放弃CPU使用权，所以线程切换是指`线程交出CPU使用权的过程`，此时需要将当前线程的状态包括堆栈的指针，局部变量等保存下来当线程切换回来后就可以继续运行
#
# 内存常见错误产生原因
1.若某块代码没有中有部分内存没有被释放，当该代码随着调用的次数的增加就会导致，内存中存在若干的小内存碎片，当偶一时刻系统要申请一大块内存的时候就会申请失败，但此时内存中可备使用的内存是大于申请的内存的，这样就造成了内存泄漏
#
2.当申请的内存被释放后，未将指针置空，再次释放若该块被其他线程使用，就会造成内存错误
#
3.申请和释放应该成对出现，当申请未释放就会造成内存泄漏
#
4.当使用指针访问一块固定大小内存的时 若不限制指针的递增的上限就会造成内存访问越界

