#ifndef __WORKQTSEND_H
#define __WORKQTSEND_H
#include "Log.h"
#include "SemQt.h"

extern SemQt sem_qt;
/*
    to Send the pack to Qt
*/

class WorkQtSend{
public:
    WorkQtSend();
    virtual ~WorkQtSend() = default;
public:
    void SetSend();
    void* Run(void* arg);
    void Stop();
private:
    void Send();
private:
    const int TimeOut = 5;
    static constexpr char* pipe_path = "/tmp/my_pipe";

    char *SendBuf;
    bool not_stop;

    Log mylog;
    static constexpr char* log_path = "Log/WorkQtRecv.log";
};


#endif