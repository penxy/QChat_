#ifndef __SERVERSIG_H
#define __SERVERSIG_H

#include "CS_Sig_config.h"
#include "UserInfo.h"
#include "tool.h"
#include "SemServer.h"

extern SemServer sem_server;

class ServerSig : public UserInfo{
public:
    ServerSig();
    virtual ~ServerSig();
public:
    /* main to recv from qt, and then send something server */
    void Recv();
private:
    void RecvSig();
    void RecvText();
    /* wake up QtSig, and then QtSig send sig to qt */
    void WakeUpQt();
protected:
    void Send_U_Text();
    void Send_G_Text();
    void SendSig(char sig);
private:
    int nowIdx;
    char* RecvBuf;
    int oldLen;

    int seq;
    char* SendBuf;
    const char* Text_UserInfoMess = "sxxUuuuYYMDHMS";
    const char* Text_GroupInfoMess = "sxxGuuuggYYMDHMS";

    const char* SigUG = ">s?UUUGG";
    const char* SigU = ">s?UUU";
    const char* Sig = ">s?";
private:
    /* tool */
    int getNum(int l, int r);
    std::string getstr(int l, int r, bool ok = false);
    int GetID();
    int GetGID();

    /* recv the mess of server, and then can send something to qt */
protected:
    sem_t sem_recv_server;
};



#endif // MESSAGE_H
