#ifndef _SEMLOCK_H_
#define _SEMLOCK_H_


#include <iostream>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/stat.h>
#include <error.h>


class SEMAPHORE
{
    private:
        union semun
        {
            int 			val;
            struct semid_ds *buf;
            unsigned short *array;
            struct seminfo *__buf;
        };
        std::string   m_name;
        int           m_proj_id;
        struct sembuf m_sem_opa;
        int           m_semid;
        int           m_locknum;
    public:
        SEMAPHORE(std::string pathname,int proj_id):m_name(pathname),m_proj_id(proj_id),m_locknum(1)
        {}
        SEMAPHORE(std::string pathname,int proj_id,int locknum):m_name(pathname),m_proj_id(proj_id),m_locknum(locknum)
        {}
        ~SEMAPHORE()
        {
            // semctl(m_semid,0,IPC_RMID);
        }
        int init();
        int unlock();
        int lock();
        int unlock(int semnum);
        int lock(int semnum);
};


#endif
