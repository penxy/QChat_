#ifndef __WORKSERVERRECV_H
#define __WORKSERVERRECV_H

#include "Log.h"
#include "SemServer.h"


class WorkServerRecv{
public:
    WorkServerRecv();
    virtual ~WorkServerRecv();
public:
    void SetRecv();
    void* Run(void* arg);
    static void Stop();
    void SetSock(int sock);
private:
    static void Recv();
    static void signal_handler(int sig);
private:
    static int sock;
    static sem_t sem_stop;
    static char* RecvBuf;

    static Log mylog;
    static constexpr char* log_path = "Log/WorkServerRecv.log";
};

#endif