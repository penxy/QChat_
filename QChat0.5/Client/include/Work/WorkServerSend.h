#ifndef __WORKSERVERSEND_H
#define __WORKSERVERSEND_H
#include "Log.h"
#include "SemServer.h"


class WorkServerSend{
public:
    WorkServerSend();
    ~WorkServerSend() = default;
public:
    void SetSend();
    void* Run(void* arg);
    static void Stop();
    void SetSock(int sock);
private:
    void Send();
    void Clear_S_Buf();
private:
    int sock;
    static bool non_stop;

    char* SendBuf;

    Log mylog;
    static constexpr char* log_path = "Log/WorkServerSend.log";
};




#endif