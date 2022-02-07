#include "memory.h"


int SHMEMORY::init()
{
    if(m_filemode)
    {
        return fliemodeinit();
    }
    else 
    {
        return keymodeinit();
    }
    return -1;
}


int SHMEMORY::fliemodeinit()
{
    m_shm_fd = shm_open(m_name.c_str(),O_RDWR| O_SYNC|O_CREAT|O_EXCL,0777);
    if(m_shm_fd == -1)
    {
        printf("%s exist \n",m_name.c_str());
        m_shm_fd = shm_open(m_name.c_str(),O_RDWR|O_CREAT,0777);
        if(m_shm_fd == -1)
        {
            perror("shm_open fault:");
            return 1;
        }
    }
    if(ftruncate(m_shm_fd,MAXMEMORYSIZE))
    {
        perror("ftruncate fault:");
        return 2;
    }
    m_memory = (uint8_t *)mmap(NULL, MAXMEMORYSIZE, PROT_READ|PROT_WRITE, MAP_SHARED,m_shm_fd, 0);
    return 0;
}

int SHMEMORY::keymodeinit()
{
    key_t shmkey = ftok(m_name.c_str(),m_proj_jd);
    if(shmkey == -1)
    {
        perror("ftok fault:");
        return 1;
    }

    m_shm_id = shmget(shmkey,MAXMEMORYSIZE,IPC_CREAT|0666|IPC_EXCL);
    if(m_shm_id == -1)
    {
        printf("name=%s,proj=%d exist \n",m_name.c_str(),m_proj_jd);
        m_shm_id = shmget(shmkey,MAXMEMORYSIZE,IPC_CREAT|0666); 
        if(m_shm_id == -1)
        {
            perror("shmget fault:");
            return 1;
        }
    }
    m_memory = (uint8_t *)shmat(m_shm_id,NULL,0);
    
    return 0;
}

int SHMEMORY::set(std::vector<uint8_t> data)
{
    memcpy(m_memory,data.data(),data.size());
    return 0;
}


std::vector<uint8_t> SHMEMORY::get()
{
    std::vector<uint8_t> retdata;
    retdata.resize(MAXMEMORYSIZE);
    memcpy(retdata.data(),m_memory,MAXMEMORYSIZE);
    return retdata;
}

