#ifndef _MEMORY_H_
#define _MEMORY_H_


#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define MAXMEMORYSIZE  1024

class SHMEMORY
{
    int m_shm_fd;
    int m_shm_id;
    uint8_t *m_memory;
    std::string m_name;
    int m_proj_jd;
    bool m_filemode;
    public:
        SHMEMORY(std::string pathname):m_name(pathname),m_filemode(true)
        {};
        SHMEMORY(std::string pathname,int proj_id):m_name(pathname),m_proj_jd(proj_id),m_filemode(false)
        {};
        ~SHMEMORY()
        {
            if(m_filemode)
            {
                munmap(m_memory, MAXMEMORYSIZE);
                // shm_unlink(m_name.c_str());
            }
            else 
            {
                shmdt(m_memory);
                // shmctl(m_shm_id,IPC_RMID,0);
            }
        };
        int init();
        int fliemodeinit();
        int keymodeinit();
        int set(std::vector<uint8_t> data);
        std::vector<uint8_t> get();
};


#endif
