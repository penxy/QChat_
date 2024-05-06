#include "WorkQtSend.h"
#include <string.h>

#include <iostream>
extern SemQt sem_qt;

WorkQtSend::WorkQtSend() : not_stop(true){
    mylog = Log(log_path);
}

void WorkQtSend::SetSend(){
    SendBuf = sem_qt.Get_SendSharedMemory();
}


void* WorkQtSend::Run(void* arg){
    while(not_stop){
        sem_qt.Sem_Wait();
        Send();
    }
    pthread_exit(NULL);
}

void WorkQtSend::Stop(){
    not_stop = false;
}

void WorkQtSend::Send(){
    int fd = open(pipe_path, O_WRONLY);
    int ret = write(fd, SendBuf, strlen(SendBuf));
    if(ret == -1){
        perror("Write error: ");
    }
    mylog.WriteLog("Send to Qt: " + std::string(SendBuf));
    memset(SendBuf, '\0', strlen(SendBuf));
    close(fd);
}

