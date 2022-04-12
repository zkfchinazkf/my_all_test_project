#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/prctl.h>

/*
    1.test pthread sched_priority and sched_mode demo,pthread priority set,set need use root
    SCHED_OTHER(0)：分时调度策略，线程默认调度策略，不区分优先级，该调度方式通过分时来完成的。当线程处于这种调度策略的时候，对线程进行优先级设置会失败。但高优先级的线程可以抢占处于该调度策略的线程。
    SCHED_FIFO(1)：实时调度策略，先进先出原则，这种调度方式有优先级之分，并且无时间片概念，处于该调度策略时，高优先级的进程将会一直占用CPU 直到有更高优先级的线程出现，将线程设置为该调度策略的时候需要超级用户模式。
    SCHED_RR(2)：实时调度策略，在SCHED_FIFO的基础上加入了时间片。于FIFO 不同，FIFO 即使有相同优先级的线程，也必须等到当前线程运行完毕后才能执行，RR可以使 相同优先级的线程都运行一段时间。
    SCHED_FIFO,SCHED_RR 优先级 1 - 99  越大优先级越高
*/

/*-------test pthread sched_priority and sched_mode demo 1-------------*/
/*--------------------------------start--------------------------------*/
void *pthreadfun(void *p)
{
    int ret = 0;
    /*Set pthread name*/
    prctl(PR_SET_NAME, "pthreadfunname");
    /*Get pthread name*/
    char namebuf[15];
    prctl(PR_GET_NAME,namebuf);
    printf("pthread name is :%s\n",namebuf);
    /* get pthread pthread ID*/
    pthread_t selfpth = pthread_self();
    /*set pthread priority and mode*/
    sched_param  param;
    param.sched_priority = 60;
    ret = pthread_setschedparam(selfpth,SCHED_RR,&param);
    if(ret)
    {
        printf("pthread_setschedparam fail is %s\n",strerror(errno));
    }
    /*get SCHED_RR mode max priority and min priority*/
    int maxpri = sched_get_priority_max(SCHED_RR);
    int minpri = sched_get_priority_min(SCHED_RR);
    printf("maxpri = %d,minpri = %d\n",maxpri,minpri);
    /*get pthread priority and mode*/
    sched_param  getparam;
    int          pori;
    ret = pthread_getschedparam(selfpth,&pori,&getparam);
    if(ret)
    {
        printf("pthread_getschedparam fail is %s\n",strerror(errno));
    }
    printf("pori is %d,getparam is %d\n",pori,getparam.sched_priority);
    while(1)
    {
        pthread_t selfpth = pthread_self();
        printf("selfpth = %lu\n",selfpth);
        sleep(1);
    }
    return NULL;
}
void *pthreadfun2(void *p)
{
    pthread_t selfpthread = pthread_self();
    pthread_attr_t selfattr;
    int rslt = pthread_getattr_np(selfpthread,&selfattr);
    if (rslt != 0) {
        printf("pthreadfun2 pthread_getattr_np is %s\n",strerror(errno));
    }
    /*pthread inheritance get*/
    int mode = PTHREAD_EXPLICIT_SCHED;
    int ret = pthread_attr_getinheritsched(&selfattr,&mode);
    if(ret)
    {
        printf("pthread_attr_getinheritsched fail is %s\n",strerror(errno));
    }
    /*pthread pri  get*/
    int policy = SCHED_RR;
    ret = pthread_attr_getschedpolicy(&selfattr, &policy);
    if(ret)
    {
        printf("pthread_attr_getschedpolicy fail is %s\n",strerror(errno));
    }
    /*pthread pri get*/
    sched_param  schparam;
    schparam.sched_priority = 0;
    ret = pthread_attr_getschedparam(&selfattr,&schparam);
    if(ret)
    {
        printf("pthread_attr_getschedparam fail is %s\n",strerror(errno));
    }
    int endpoli;
    sched_param endsch;
    ret = pthread_getschedparam(selfpthread,&endpoli,&endsch);
    if(ret)
    {
        printf("pthread_getschedparam fail is %s\n",strerror(errno));
    }
    printf("pthreadfun2 pori is %d,getparam is %d\n",endpoli,endsch.sched_priority);
    while(1)
    {
        /*pthread while(1) need do some */
        for(int idx=0;idx<1000000;idx++)
        {
            endpoli = idx * 300+1;
        }
    }
}
/*-------test pthread sched_priority and sched_mode demo 1-------------*/
/*--------------------------------end----------------------------------*/


int main(int argc,char **argv)
{
/*-------test pthread sched_priority and sched_mode demo 1-------------*/
/*--------------------------------start--------------------------------*/
    int ret;
    pthread_t pth1,pth2;
    pthread_attr_t selfattr;
    pthread_attr_init(&selfattr);
    /*pthread inheritance set*/
    ret = pthread_attr_setinheritsched(&selfattr, PTHREAD_EXPLICIT_SCHED);
    if(ret)
    {
        printf("pthread_attr_setinheritsched fail is %s\n",strerror(errno));
    }
    /*pthread mode set*/
    ret = pthread_attr_setschedpolicy(&selfattr, SCHED_RR);
    if(ret)
    {
        printf("pthread_attr_setschedpolicy fail is %s\n",strerror(errno));
    }
    /*pthread poriority set*/
    sched_param  schparam;
    schparam.sched_priority = 80;
    ret = pthread_attr_setschedparam(&selfattr,&schparam);
    if(ret)
    {
        printf("pthread_attr_setschedparam fail is %s\n",strerror(errno));
    }
    /*open next 2 line will lead to data occupation*/
    // pthread_create(&pth1,NULL,pthreadfun,NULL);
    // pthread_create(&pth2,&selfattr,pthreadfun2,NULL);
    printf("pth1 = %lu\n",pth1);
    printf("pth2 = %lu\n",pth2);
/*-------test pthread sched_priority and sched_mode demo 1-------------*/
/*--------------------------------end----------------------------------*/


    while(1);
    return 0;
}