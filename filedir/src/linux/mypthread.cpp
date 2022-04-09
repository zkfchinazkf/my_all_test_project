#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/prctl.h>

void *pthreadfun(void *p)
{
    /*Set pthread name*/
    prctl(PR_SET_NAME, "pthreadfunname");

    /*Get pthread name*/
    char namebuf[15];
    prctl(PR_GET_NAME,namebuf);
    printf("pthread name is :%s\n",namebuf);

    /* get pthread pthread ID*/
    pthread_t selfpth = pthread_self();

    int ret = 0;

    /*set pthread priority and mode*/
    sched_param  param;
    param.sched_priority = 10;
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
        printf("selfpth = %u\n",selfpth);
        sleep(3);
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

    /*pthread inheritance set and get*/
    int mode = PTHREAD_EXPLICIT_SCHED;
    int ret = pthread_attr_getinheritsched(&selfattr,&mode);
    if(ret)
    {
        printf("pthread_attr_getinheritsched fail is %s\n",strerror(errno));
    }
    mode = PTHREAD_EXPLICIT_SCHED;
    ret = pthread_attr_setinheritsched(&selfattr, mode);
    if(ret)
    {
        printf("pthread_attr_setinheritsched fail is %s\n",strerror(errno));
    }


    /*pthread pri set and get*/
    int policy = SCHED_RR;
    ret = pthread_attr_getschedpolicy(&selfattr, &policy);
    if(ret)
    {
        printf("pthread_attr_getschedpolicy fail is %s\n",strerror(errno));
    }
    policy = SCHED_RR;
    ret = pthread_attr_setschedpolicy(&selfattr, policy);
    if(ret)
    {
        printf("pthread_attr_setschedpolicy fail is %s\n",strerror(errno));
    }

    /*pthread pri set and get*/
    sched_param  schparam;
    schparam.sched_priority = 50;
    ret = pthread_attr_setschedparam(&selfattr,&schparam);
    if(ret)
    {
        printf("pthread_attr_setschedparam fail is %s\n",strerror(errno));
    }

    ret = pthread_attr_getschedparam(&selfattr,&schparam);
    if(ret)
    {
        printf("pthread_attr_getschedparam fail is %s\n",strerror(errno));
    }


    int endpoli;
    sched_param endsch;
    ret = pthread_attr_getschedpolicy(&selfattr, &endpoli);
    ret = pthread_attr_getschedparam(&selfattr,&endsch);

   
    printf("pori is %d,getparam is %d\n",endpoli,endsch.sched_priority);
    while(1)
    {
        // printf("my pori");
        sleep(1);
    }
}

int main(int argc,char **argv)
{
    pthread_t pth1,pth2,pth3;
    pthread_create(&pth1,NULL,pthreadfun,NULL);
    pthread_create(&pth2,NULL,pthreadfun2,NULL);
    printf("pth1 = %u\n",pth1);
    printf("pth2 = %u\n",pth2);
    while(1);
    return 0;
}