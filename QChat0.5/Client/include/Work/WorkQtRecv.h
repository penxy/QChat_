#ifndef __WORKQTRECV_H
#define __WORKQTRECV_H

#include "Log.h"
#include <sys/select.h>
#include <sys/sem.h>
#include "SemQt.h"

extern SemQt sem_qt;

class WorkQtRecv{
public:
    WorkQtRecv();
    virtual ~WorkQtRecv() = default;
public:
    void SetRecv();
    void* Run(void* arg);
    void Stop();
private:
    void Recv();
private:
    char* RecvBuf;

    int sem_id;
    struct sembuf op;
    bool not_stop;

    static constexpr char* pipe_path = "/tmp/my_pipe";
    const char* Qt_path = "Qt/Test";

    Log mylog;
    static constexpr char* log_path = "Log/WorkQtRecv.log";
};

#endif