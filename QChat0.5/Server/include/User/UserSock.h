#ifndef __USERSOCK_H
#define __USERSOCK_H

#include "User.h"
#include "CS_Sig_config.h"
#include "log.h"
#include "config.h"
#include "tool.h"
#include <unordered_map>
#include <queue>

#define DEBUG 1


class UserSock : public Log{
public:
    UserSock();
    ~UserSock();
public:
    static void Recv(void* info);
private:
    static void Send2CR();/* Send to Client_Recv */
    static void RecvSig();
    static void SendText();
    static void Send2CS();/* Send to Client_Send */
private:        
    static void SendSig(char sig);

private:
    /* whem Client Login, and SendSig CS_S_OK, Send it */
    static void SendInBuffer();



private:
    static void LoginYes();
    static void Logout();
private:
    /* sock of client */
    static int sock;
    static int idx;//目前所操作的id
    //std::queue<int>q;//the idx of buf
    static char* buf;
    //char* buf[BUF_NUM];
    static User* user;
private:
    static std::unordered_map<int, bool>Online;
    static std::unordered_map<int, int>IDGetSock;
    static std::unordered_map<int, int>SockGetID;
private:
    static constexpr char* Sig = ">s?";
    static void InitDebug();
private:
    static std::queue<int>ID_Queue;

//private:
    //pthread_mutex_t lock_queue;//protect queue
};


#endif