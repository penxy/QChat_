#include "Client.h"
#include <functional>
#include <signal.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include <thread>
#include <iostream>
#include "QtSig.h"
#include <sys/wait.h>

#define F std::cout << "bug\n";

extern Log log_client;
extern Log log_server;

extern QtSig* user;


sem_t Client::sem_stop_client;
WorkQtRecv Client::obj_WorkQtRecv = WorkQtRecv();
WorkQtSend Client::obj_WorkQtSend = WorkQtSend();
WorkServerRecv Client::obj_WorkServerRecv = WorkServerRecv();
WorkServerSend Client::obj_WorkServerSend = WorkServerSend();

extern bool __login_ok;

void Client::StopSig(int sig){
    if(sig = SIGINT){
        log_client.WriteLog("StopSig");
        obj_WorkQtRecv.Stop();
        obj_WorkQtSend.Stop();
        obj_WorkServerRecv.Stop();
        obj_WorkServerSend.Stop();
        if(__login_ok)user->Save();
        sem_post(&sem_stop_client);
    }
    
}

Client::Client(){
    signal(SIGINT, StopSig);
    sem_init(&sem_stop_client, 0, 0);
    Connect();

    obj_WorkServerRecv.SetSock(sock);
    obj_WorkServerSend.SetSock(sock);
    obj_WorkQtRecv.SetRecv();
    obj_WorkQtSend.SetSend();
    obj_WorkServerRecv.SetRecv();
    obj_WorkServerSend.SetSend();
}
Client::~Client(){
    sem_destroy(&sem_stop_client);
}

void Client::Run(){      
    std::thread t1(&WorkQtRecv::Run, &obj_WorkQtRecv, nullptr);
    std::thread t2(&WorkQtSend::Run, &obj_WorkQtSend, nullptr);
    std::thread t3(&WorkServerSend::Run, &obj_WorkServerSend, nullptr);
    std::thread t4(&WorkServerRecv::Run, &obj_WorkServerRecv, nullptr);

    sem_wait(&sem_stop_client);
    t1.detach();
    t2.detach();
    t3.detach();
    t4.detach();
    log_client.WriteLog("Client Stop");
}

void Client::Connect(){
    struct sockaddr_in adr;
    memset(&adr, '\0', sizeof(adr));
    adr.sin_family = AF_INET;
    
    sock = socket(AF_INET, SOCK_STREAM, 0);

    adr.sin_family = AF_INET;
    adr.sin_addr.s_addr = inet_addr(SERVER_IP);
    adr.sin_port = htons(std::stoi(SERVER_PORT));

    if (connect(sock, (struct sockaddr *)&adr, sizeof(adr)) < 0) {
        log_server.WriteLog("Connect error");
        exit(EXIT_FAILURE);
    }
    log_server.WriteLog("Connect Server");
}
