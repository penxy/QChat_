#include "UserSock.h"
#include <sys/epoll.h>

#define debug(x) std::cout << #x << " = " << x << "\n";

char* UserSock::buf = new char[MAX_BUF_SIZE];
User* UserSock::user = new User[MAX_ID];

std::unordered_map<int, bool>UserSock::Online;
std::unordered_map<int, int>UserSock::IDGetSock;
std::unordered_map<int, int>UserSock::SockGetID;

int UserSock::sock;
int UserSock::idx;

std::queue<int>UserSock::ID_Queue;

void reset_oneshot(int sock, int epollfd);
void delfd(int sock, int epollfd);

/*   
    sock                sock of ClientSend
    idx                 目前操作的user[idx]
    user[idx].youid     对方
    user[idx].myid = idx;
*/

void UserSock::InitDebug(){
    user[0].myid = 0;
    user[0].myname = "Tom";
    user[0].mypassword = "12345";
    
    user[1].myid = 1;
    user[1].myname = "Jerry";
    user[1].mypassword = "12345";

    ID_Queue.pop();
    ID_Queue.pop();
}

UserSock::UserSock(){
    for(int i=0;i<MAX_ID;i++){
        user[i].myid = SockGetID[i] = IDGetSock[i] = -1;
        Online[i] = false;
        ID_Queue.push(i);
    }
#ifdef DEBUG
    InitDebug();
#endif
}
UserSock::~UserSock(){
    delete[] buf;
    delete[] user;    
}














/* socket recv */
void UserSock::Recv(void *info){
    sock = ((Info*)info)->c_sock;
    int epollfd = ((Info*)info)->epollfd;
    memset(buf, '\0', strlen(buf));

    while(1){
        int ret = recv(sock, buf, MAX_BUF_SIZE - 1, 0);
        if(ret < 0){
            if(errno == EAGAIN)reset_oneshot(sock, epollfd);
            return;  
        }else if(ret == 0){
            delfd(sock, epollfd);
            Logout();
            std::cout << "client disconnect\n";
            return;
        }else{
            std::cout << "Recv from Client: " + std::string(buf) << "\n";
        }

        if(ret < Min_Len){
            WriteLog("unknow len");
        }
        if(buf[Idx_SigFlag] == '>'){
            RecvSig();
        }else{
            SendText();
        }
    }
    delete info;
}

/* recv Sig from Client */
void UserSock::RecvSig(){
    char sig = buf[Idx_Sig];
    /* if online, idx = SockGetID[sock];else is first Login. */
    if(sig != CS_U_CREATE){
        if(Online[SockGetID[sock]])idx = SockGetID[sock];
        else idx = GetID(&buf[Idx_ID]);
    }

    auto R_Set_YouID = [&](){user[idx].youid = GetID(&buf[Idx_ID]);};
    auto R_Set_Name = [&](int Idx, int p = idx){user[p].Rname = GetName(&buf[Idx]);};//if is CS_U_CREATE, need p
    auto R_Set_Pass = [&](int Idx, int p = idx){user[p].pass = GetPass(&buf[Idx]);};

    switch(sig){
    case CS_F_ALLOW:{
        R_Set_YouID();
        /* allow */
        user[idx].User_Allow();
        /* beallow */
        user[user[idx].youid].youid = idx;
        user[user[idx].youid].User_Allow();

        SendSig(SC_F_BEALLOW);
        break;
    }
    case CS_F_REJECT:{
        SendSig(SC_F_BEREJECT);break;
    }
    case CS_F_ADD:{
        SendSig(SC_F_BEADD);
        break;
    }
    case CS_F_DEL:{
        R_Set_YouID();
        /* del */
        user[idx].User_Del();
        /* bedel */
        user[user[idx].youid].youid = idx;
        user[user[idx].youid].User_Del();

        SendSig(SC_F_BEDEL);
        break;
    }
    case CS_FIND_ID:{
        R_Set_YouID();
        
        //防止自己加自己
        if(user[idx].myid == user[idx].youid){
            SendSig(SC_FIND_ID_NO);
            break;
        }

        if(user[user[idx].youid].myid == -1){
            SendSig(SC_FIND_ID_NO);
        }else{
            SendSig(SC_FIND_ID_YES);
        }break;
    }
    case CS_U_LOGIN:{
        R_Set_YouID();
        R_Set_Pass(Idx_P_U);

        if(!user[idx].User_Login(user[idx].pass)){
            SendSig(SC_U_LOGIN_NO);
        }else{
            LoginYes();
            SendSig(SC_U_LOGIN_YES);
            //SendInBuffer();   don't need, because client will send CS_S_OK, and then send.  4.29
        }break;
    }
    case CS_S_OK:{
        if(user[idx].myid == -1)break;//error id

        if(user[idx].empty_buffer())SendSig(SC_S_EMPTY);
        else SendInBuffer();
        break;
    }
    case CS_U_CREATE:{
        if(ID_Queue.empty())SendSig(SC_U_CREATE_NO);
        else{            
            int idx = ID_Queue.front();
            R_Set_Name(Idx_N, idx);
            R_Set_Pass(Idx_P_N, idx);

            user[idx].myid = idx;
            user[idx].myname = user[idx].Rname;
            user[idx].mypassword = user[idx].pass;

            SendSig(SC_U_CREATE_YES);            
        }
        break;
    }

    default:{
        WriteLog("RecvSig error");break;
    }
    }
}

/*
    Send Sig, select Client_Recv or Client_Send
*/

void UserSock::SendSig(char sig){
    memset(buf, '\0', strlen(buf));
    int myid = user[idx].myid;
    strcpy(buf, Sig);
    Set_Sig(buf, sig);

    switch(sig){
    case SC_F_BEALLOW:{
        Sig_Append_ID(buf, myid);Send2CR();break;
    }
    case SC_F_BEREJECT:{
        Sig_Append_ID(buf, myid);Send2CR();break;
    }
    case SC_F_BEADD:{
        Sig_Append_ID(buf, myid);Sig_Append_Name(buf, user[idx].myname);Send2CR();break;
    }
    case SC_F_BEDEL:{
        Sig_Append_ID(buf, myid);
        Sig_Append_Name(buf, user[idx].myname);Send2CR();break;
    }
    case SC_FIND_ID_NO:{
        Send2CS();break;
    }
    case SC_FIND_ID_YES:{
        Sig_Append_Name(buf, user[user[idx].youid].myname);Send2CS();break;
    }
    case SC_U_LOGIN_NO:{
        Send2CS();break;
    }
    case SC_U_LOGIN_YES:{
        Sig_Append_Name(buf, user[user[idx].youid].myname);Send2CS();break;
    }
    case SC_S_EMPTY:{
        Send2CS();break;
    }
    case SC_U_CREATE_YES:{
        int id = ID_Queue.front();ID_Queue.pop();Sig_Append_ID(buf, id);Send2CS();break;
    }
    case SC_U_CREATE_NO:{
        Send2CS();break;
    }
    default:{
        WriteLog("UserSock.cpp, Unknow SendSig");
        break;
    }
    }
}



/* Send buf to his sock or Write in buffer */
void UserSock::Send2CR(){
    if(Online[user[idx].youid])send(IDGetSock[user[idx].youid], buf, strlen(buf), 0);
    else user[user[idx].youid].WriteInBuffer(buf), std::cout << "Write in buffer\n";

    std::cout << "Send to ClientR: " << buf << "\n";
    memset(buf, '\0', strlen(buf));
}
/* send to Client_Send */
void UserSock::Send2CS(){
    std::cout << "UserSock.cpp: Send to ClientS: " + std::string(buf) << "\n";
    send(sock, buf, strlen(buf), 0);
    memset(buf, '\0', strlen(buf));
}
/* when client LoginYes, Send it, and then client will send sig of "CS_S_OK", continue send it  */
void UserSock::SendInBuffer(){
    //read
    user[idx].Read();
    int ret = send(sock, user[idx].GetReadIdx(), user[idx].GetReadLen(), 0);
    user[idx].AfterRead();
    memset(buf, '\0', strlen(buf));
}
/* Send message */
void UserSock::SendText(){
    //const char* Text_UserInfoMess = "XXsxxUuuuYYMDHMS";
    user[idx].youid = Get_Text_ID(buf);//

    //turn uuu from youid to myid.
    std::string MyID = __10_64_string(idx, 3);
    for(int i=Idx_Text_ID;i<Idx_Text_ID+Len_ID;i++){
        buf[i] = MyID[i];
    }
    
    Send2CR();
}














void UserSock::LoginYes(){
    Online[idx] = true;
    SockGetID[sock] = idx;
    IDGetSock[idx] = sock;
}
void UserSock::Logout(){
    Online[idx] = false;
    SockGetID[sock] = -1;
    IDGetSock[idx] = -1;
}


void reset_oneshot(int sock, int epollfd){
    struct epoll_event event;
	event.data.fd = sock;
	event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
	epoll_ctl(epollfd, EPOLL_CTL_MOD, sock, &event);
}

void delfd(int sock, int epollfd){
    close(sock);
    epoll_ctl(epollfd, EPOLL_CTL_DEL, sock, 0);
}
