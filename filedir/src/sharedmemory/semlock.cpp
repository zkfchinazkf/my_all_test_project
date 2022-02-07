#include "semlock.h"

int SEMAPHORE::init()
{

    struct semid_ds semds;
    bool firstflag=true;
    key_t semkey = ftok(m_name.c_str(), m_proj_id);
    if(semkey == -1 )
    {
        perror("semkey fault:");
        return 1;
    }
    m_semid = semget(semkey,m_locknum,0666|IPC_CREAT|IPC_EXCL);
    if(m_semid == -1 )
    {
        firstflag=false;
        printf("%s exist \n",m_name.c_str());
        m_semid = semget(semkey,1,0666|IPC_CREAT);
        {
            if(m_semid == -1 )
            {
                perror("semget fault:");
                return 2;
            }
        }
        if(semctl(m_semid,0,IPC_STAT,&semds) != -1)
        {
            printf("sems->sem_nsems=%ld\n",semds.sem_nsems);
            m_locknum = semds.sem_nsems;
        }
        else 
        {
            perror("semctl fault:");
        }
    }


    union semun seminit;
    seminit.val=1;
    if(firstflag)
    {
        for(int nownum=0;nownum<m_locknum;nownum++)
        {
            if (semctl(m_semid,nownum,SETVAL,seminit) == -1)
            {
                fprintf(stderr, "semaphore_init failed\n");
                return 3;
            }
        }
    }
    
    m_sem_opa.sem_flg=SEM_UNDO;
    m_sem_opa.sem_num=0;

    return 0;
}

int SEMAPHORE::unlock()
{
    m_sem_opa.sem_op=1;
    m_sem_opa.sem_num=0;
    if (semop(m_semid,&m_sem_opa, IPC_SET) == -1)
    {
        perror("semaphore_v failed:");
        return 0;
    }
    return 1;
}

int SEMAPHORE::lock()
{
    m_sem_opa.sem_op=-1;
    m_sem_opa.sem_num=0;
    if (semop(m_semid,&m_sem_opa, IPC_SET) == -1)
    {
        perror("semaphore_p failed:");
        return 0;
    }
    return 1;
}

int SEMAPHORE::unlock(int semnum)
{
    if(semnum>=m_locknum)
    {
        printf("semnum=%d  > m_locknum=%d\n",semnum,m_locknum);
        return 1;
    }
    m_sem_opa.sem_num=semnum;
    m_sem_opa.sem_op=1;
    if (semop(m_semid,&m_sem_opa, IPC_SET) == -1)
    {
        perror("semaphore_v failed:");
        return 2;
    }
    return 0;
}

int SEMAPHORE::lock(int semnum)
{
    if(semnum>=m_locknum)
    {
        printf("semnum=%d  > m_locknum=%d\n",semnum,m_locknum);
        return 1;
    }
    m_sem_opa.sem_num=semnum;
    m_sem_opa.sem_op=-1;
    if (semop(m_semid,&m_sem_opa, IPC_SET) == -1)
    {
        perror("semaphore_p failed:");
        return 2;
    }
    return 0;
}


