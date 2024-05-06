#include "WorkServerSend.h"
#include "SemServer.h"
#include <string.h>
#include <iostream>

bool WorkServerSend::non_stop = true;
extern SemServer sem_server;
WorkServerSend::WorkServerSend() : sock(-1){
    mylog = Log(log_path);
}
void WorkServerSend::SetSend(){
    SendBuf = sem_server.Get_SendSharedMemory();    
}

void* WorkServerSend::Run(void* arg){
    while(non_stop){
        sem_server.Wait();//_1
        if(non_stop)Send();
    }
    pthread_exit(NULL);
}
void WorkServerSend::Stop(){
    non_stop = false;
    /* wake up _1 */
    sem_server.Send2Pipe();
}
void WorkServerSend::SetSock(int sock){
    this->sock = sock;
}
void WorkServerSend::Send(){
    if(sock == -1){
        mylog.WriteLog("sock Send error");
        exit(EXIT_FAILURE);
    }
    send(sock, SendBuf, strlen(SendBuf), 0);
    mylog.WriteLog("Send to Server: " + std::string(SendBuf));
    Clear_S_Buf();
}
void WorkServerSend::Clear_S_Buf(){
    memset(SendBuf, '\0', strlen(SendBuf));
}
 
