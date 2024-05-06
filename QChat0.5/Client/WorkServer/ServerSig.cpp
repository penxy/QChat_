#include <unistd.h>
#include "SemServer.h"
#include "SemQt.h"
#include "ServerSig.h"

/* not use  */
//#define NON_USE_CS_OK 1

#ifndef SERVEROK
#define SERVEROK 1
#endif

#define debug(x) std::cout << #x << " = " << x << "\n";

/* don't send CS_S_OK if recv the SC_S_EMPTY */
bool __buffer_empty = false;
bool __login_ok = false;

ServerSig::ServerSig() : nowIdx(0), oldLen(0), seq(0){
    RecvBuf = sem_server.Get_RecvSharedMemory();
    SendBuf = sem_server.Get_SendSharedMemory();

    memset(RecvBuf, '\0', strlen(RecvBuf));
    memset(SendBuf, '\0', strlen(SendBuf));
    sem_init(&sem_recv_server, 0, 0);
}
ServerSig::~ServerSig(){
    sem_destroy(&sem_recv_server);
}

void ServerSig::WakeUpQt(){
    /* wake up QtSig, and then QtSig send sig to qt */
    sem_post(&sem_recv_server);
}

void ServerSig::Recv(){
    //debug(nowIdx);
    if(RecvBuf[nowIdx] == '\0' || nowIdx >= strlen(RecvBuf)){
        nowIdx = 0;
        memset(RecvBuf, '\0', strlen(RecvBuf));     
        return;
    }
    if(RecvBuf[nowIdx] == '>')
        RecvSig();
    else
        RecvText();
#ifndef NON_USE_CS_OK
    if(!__login_ok)return;
    if(!__buffer_empty){//not empty()
        SendSig(CS_S_OK);
        __buffer_empty = false;
    }
#endif
}

/*  sxxUuuuYYMDHMS| ... |     sxxGuuuggYYMDHMS| ... |  */
/* recv the text */
void ServerSig::RecvText(){
    int len = getNum(1, 2);//sxx...
    if(RecvBuf[nowIdx + 3] == 'U'){
        youid = getNum(4, 6);
        UserInfo_RecvFMess(FMess(0, std::string(&RecvBuf[nowIdx + 16], &RecvBuf[nowIdx + len - 2]), get_time(getNum(7, 8), __64_10(RecvBuf[nowIdx + 9]), __64_10(RecvBuf[nowIdx + 10]), __64_10(RecvBuf[nowIdx + 11]), __64_10(RecvBuf[nowIdx + 12]), __64_10(RecvBuf[nowIdx + 13]))));
    }else if(RecvBuf[nowIdx + 3] == 'G'){
        gid = getNum(7, 8);
        UserInfo_RecvGMess(GMess(0, getNum(4, 6), std::string(&RecvBuf[nowIdx + 18], &RecvBuf[nowIdx + len - 2]), get_time(getNum(9, 10), __64_10(RecvBuf[nowIdx + 11]), __64_10(RecvBuf[nowIdx + 12]), __64_10(RecvBuf[nowIdx + 13]), __64_10(RecvBuf[nowIdx + 14]), __64_10(RecvBuf[nowIdx + 15]))));
    }
    nowIdx += len;
    Recv();
}

/* recv the sig */
void ServerSig::RecvSig(){
    char sig = RecvBuf[nowIdx + 2];
    switch(sig){
        case SC_F_BEALLOW: {//被同意加好友
            //>sAuuu
            youid = GetID();
            UserInfo_Friend_BeAllow();
            oldLen = 6;
            break;
        }
        case SC_F_BEREJECT: {//被拒绝添加好友
            //>sBuuu                                                                                                                        //new
            youid = GetID();
            UserInfo_Friend_BeReject();
            oldLen = 6;
            break;
        }
        case SC_F_BEADD: {//被加
            //>sCuuu(8n)
            youid = GetID();
            Rname = getstr(6, 13);
            UserInfo_Friend_BeAdd();
            oldLen = 14;
            break;
        }
        case SC_F_BEDEL: {//被删
            //>sDuuu
            youid = GetID();
            UserInfo_Friend_BeDel();
            oldLen = 6;
            break;
        }
        case SC_F_RENAME:{//对方改名
            //>s4uuunnnnnnnn
            youid = GetID();
            Rname = getstr(6, 13);
            UserInfo_Friend_Rename();
            oldLen = 14;
            break;
        }
        case SC_F_DELETE:{//对方注销
            //>s5uuu
            youid = GetID();
            UserInfo_Friend_Delete();
            oldLen = 6;
            break;
        }
        case SC_G_RENAME:{//群名改变
            //>s6UUUggnnnnnnnn
            gid = GetGID();
            Rname = getstr(8, 15);
            UserInfo_Group_BeRename();
            oldLen = 16;
            break;
        }

        case SC_G_LEADER:{//群leader改变
            //>s8uuugguuu
            id = GetID();
            gid = GetGID();
            youid = getNum(8, 10);
            UserInfo_Group_BeSetLeader();
            oldLen = 11;
            break;
        }
        case SC_G_BEALLOW:{//被同意进群 (S have a table save a person number of group, if(< MAX_GROUPS_SIZE), Send SC_NEWMEMBER, else, ignore it)
            //>s9uuuggxxuuu...nnnnnn..
            if(GroupsSize == MAX_GROUPS_SIZE){
                WriteLog("SC_GBEALLOW error");
                break;
            }
            gid = GetGID();
            int GroupMemberNum = getNum(8, 9);
            int LenOfMemberId = 3 * GroupMemberNum;
            Rname = getstr(LenOfMemberId + 9, LenOfMemberId + 16);
            youid = GetID();

            std::set<int>temp;
            for(int i=1,cnt;i<=GroupMemberNum;i++){
                cnt = 3 * (i - 1);
                temp.insert(getNum(cnt + 10, cnt + 12));
            }
            UserInfo_Group_BeAllowMember(temp);
            oldLen = LenOfMemberId + 18;
            break;
        }
        case SC_G_BEREMOVE:{//被t
            //>saUUUgg
            gid = GetGID();
            UserInfo_Group_BeDelMember();
            oldLen = 8;
            break;
        }
        case SC_G_BEINVITE:{//被邀请进群
            //>s9uuuggxxuuu...nnnnnn..
            if(GroupsSize == MAX_GROUPS_SIZE){
                WriteLog("SC_GBEALLOW error");
                break;
            }
            gid = GetGID();
            int GroupMemberNum = getNum(8, 9);
            int LenOfMemberId = 3 * GroupMemberNum;
            Rname = getstr(LenOfMemberId + 9, LenOfMemberId + 16);
            youid = GetID();

            std::set<int>temp;
            for(int i=1,cnt;i<=GroupMemberNum;i++){
                cnt = 3 * (i - 1);
                temp.insert(getNum(cnt + 10, cnt + 12));
            }
            UserInfo_Group_BeAllowMember(temp);
            oldLen = LenOfMemberId + 18;
            break;
        }
        case SC_G_BEREJECT:{//被拒绝进群
            //>scUUUgg
            gid = GetGID();
            UserInfo_Group_BeRejectMember();
            oldLen = 8;
            break;
        }
        case SC_G_BEINSERT:{//有新申请入群(S send it to leader)
            //>sduuugg
            youid = GetID();
            gid = GetGID();
            UserInfo_Group_NewMemberRequest();
            oldLen = 8;
            break;
        }
        case SC_G_MEMBERQUIT:{//群成员离开
            //>seuuugg
            youid = GetID();
            gid = GetGID();
            UserInfo_Group_MemberQuit();
            oldLen = 8;
            break;
        }
        case SC_U_CREATE_YES:{//注册成功
            //>spuuu
            youid = GetID();
            oldLen = 6;
            UserInfo_User_CreateYes();
            CreateSQL();                //创建一个数据库

            WakeUpQt();
            break;
        }
        case SC_U_CREATE_NO:{//注册失败
            //>sq
            OK = false;
            oldLen = 3;
            
            WakeUpQt();
            break;
        }
        case SC_U_LOGIN_YES:{//登录成功 //user->Load()
            //>s8nnnnnnnn
            __login_ok = true;
            strcpy(MyName, &RecvBuf[3]);
            UserInfo_User_LoginYes();

            //python
            Load();//Load in SQL

            WakeUpQt();
            oldLen = 11;break;
        }
        case SC_U_LOGIN_NO:{
            //>sn
            OK = false;
            oldLen = 3;
            
            WakeUpQt();
            break;
        }
        case SC_G_CREATE_YES:{//建群成功
            //>sgUUUgg
            gid = GetGID();
            UserInfo_Group_CreateYes();

            WakeUpQt();
            oldLen = 8;
            break;
        }
        case SC_G_CREATE_NO: {//建群失败
            //>so
            OK = false;
            oldLen = 3;
            
            WakeUpQt();
            break;
        }
        case SC_U_SETPASS_YES:{//设置密码成功
            //>sh
            UserInfo_User_SetPassWordYes();

            WakeUpQt();
            oldLen = 3;break;
        }
        case SC_U_SETPASS_NO:{//设置密码失败
            //>si            
            OK = false;
            oldLen = 3;
            
            WakeUpQt();
            break;
        }
        case SC_FIND_ID_YES:{//ID查找成功
            //>sjnnnnnnnn
            Rname = getstr(3, 10, true);
            OK = true;
            oldLen = 11;
            
            WakeUpQt();
            break;
        }
        case SC_FIND_ID_NO:{//ID查找失败
            //>sk
            OK = false;
            oldLen = 3;
            
            WakeUpQt();
            break;
        }
        case SC_FIND_GID_YES:{//GID YES
            //>slnnnnnnnn
            OK = true;
            Rname = getstr(3, 10, true);
            oldLen = 11;
            
            WakeUpQt();
            break;
        }
        case SC_FIND_GID_NO:{//GIO NO
            //>sm
            ;
            OK = false;
            oldLen = 3;
            
            WakeUpQt();
            break;
        }
        case SC_G_ALLOW_YES:{
            OK = true;
            oldLen = 3;

            WakeUpQt();
            break;
        }
        case SC_G_ALLOW_NO:{
            OK = false;oldLen = 3;

            WakeUpQt();
            break;
        }
        case SC_F_ALLOW_YES:{
            OK = true;oldLen = 3;

            WakeUpQt();
            break;
        }
        case SC_F_ALLOW_NO:{
            OK = false;oldLen = 3;

            WakeUpQt();
            break;
        }
        case SC_G_INVITE_YES:{
            OK = true;oldLen = 3;

            WakeUpQt();
            break;
        }
        case SC_G_INVITE_NO:{
            OK = false;oldLen = 3;

            WakeUpQt();
            break;
        }
        case SC_S_EMPTY:{
            oldLen += 3;
            __buffer_empty = true;
            break;
        }
        default:{
            WriteLog("Message.cpp:325 unknow sig");
        }
    }
    nowIdx += oldLen;
    Recv();
}
/* -------------------------------SMessage-------------------------------------- */




/* return a sig which need be send in QtSig.cpp */
void ServerSig::SendSig(char sig){
#ifndef SERVEROK
    return;
#endif
    int p = 3;
    auto InitSigUG = [&](){//>s?uuugg
        strcpy(SendBuf, SigUG);
    };
    auto InitSigU = [&](){//>s?uuu
        strcpy(SendBuf, SigU);
    };
    auto InitSig = [&](){//>s?
        strcpy(SendBuf, Sig);
    };
    
    auto SetID = [&](int id){
        if(id == -1){
            p += 3;
            return;
        }
        strcpy(&SendBuf[p], __10_64_string(id, 3).c_str());p += 3;
    };
    auto SetGID = [&](int gid){
        if(gid == -1){
            p += 2;
            return;
        }
        strcpy(&SendBuf[p], __10_64_string(gid, 2).c_str());p += 2;
    };
    auto SetName = [&](const std::string& name){
		strcpy(&SendBuf[p], Sname.c_str());
		for(int i=p+Sname.size();i<p+8;i++){
			SendBuf[i] = ' ';
		}
		p += 8;
    };
    auto SetPass = [&](const std::string& password){
        strcpy(&SendBuf[p], password.c_str());
		for(int i=p+password.size();i<p+16;i++){
			SendBuf[i] = ' ';
		}
		p += 16;
    };

    switch(sig){
        case CS_F_ALLOW:{//同意申请
            //>s0uuu
            InitSigU();
            SetID(youid);break;
        }
        case CS_F_REJECT:{//拒绝申请
            //>s1uuu
            InitSigU();
            SetID(youid);break;
        }
        case CS_F_ADD:{//申请添加好友
            //>s2uuuGG(8n)
            std::cout << "ServerSig.cpp 423, CS_F_ADD" << "\n";
            InitSigUG();
            SetID(youid);SetGID(-1);SetName(Sname);

            //F_Add.push_back()
            break;
        }
        case CS_F_DEL:{//删除好友
            //>s3uuu
            InitSigU();
            SetID(youid);break;
        }
        case CS_FIND_ID:{//搜索ID
            //s4uuu
            InitSigU();
            SetID(youid);break;
        }
        case CS_G_RENAME:{//重命名群
            //>s7UUUggnnnnnnnn
            InitSigUG();
            SetID(-1);SetGID(gid);SetName(Sname);break;
        }
        case CS_G_SETLEADER:{//设置Leader
            //>s8uuugguuu
            InitSigUG();
            SetID(youid);SetGID(gid);SetID(myid);break;
        }
        case CS_G_ALLOW:{//同意入群
            //>s9uuugg
            InitSigUG();
            SetID(youid);SetGID(gid);break;
        }
        case CS_G_REMOVE:{//删除成员
            //>sauuugg
            InitSigUG();
            SetID(youid);SetGID(gid);break;
        }
        case CS_G_INVITE:{//邀请进群
            //>sbuuugg
            InitSigUG();
            SetID(youid);SetGID(gid);break;
        }
        case CS_G_REJECT:{//拒绝入群申请
            //>scuuugg
            InitSigUG();
            SetID(youid);SetGID(gid);break;
        }
        case CS_G_CREATE:{//创建群
            //>sdnnnnnnnn
            InitSig();
            SetName(Sname);break;
        }
        case CS_G_INSERT:{//申请入群
            //>seuuugg
            InitSigUG();
            SetID(myid);
            SetGID(gid);break;
        }
        case CS_G_QUIT:{//离开群
            //>sfuuugg
            InitSigUG();
            SetID(myid);
            SetID(gid);break;
        }
        case CS_FIND_GID:{//搜索GID
            //>sgUUUgg
            InitSigUG();
            SetID(-1);SetGID(gid);break;
        }
        case CS_U_DELETE:{//账号注销
            //>shuuu
            InitSigU();
            SetID(myid);break;
        }
        case CS_U_RENAME:{//个人重命名
            //>siuuunnnnnnnn
            InitSigU();
            SetID(myid);
            SetName(Sname);break;
        }
        case CS_U_PASS:{//改密码
            //>sjppppppppppppppppPPPPPPPPPPPPPPPP
            InitSig();
            SetPass(password);
            SetPass(Sname);break;
        }
        case CS_U_CREATE:{//注册
            //>sknnnnnnnnpppppppppppppppp
            InitSig();
            SetName(Sname);
            SetPass(password);            
            break;
        }
        case CS_U_LOGIN:{//登录
            //>sluuupppppppppppppppp
            InitSigU();
            SetID(myid);
            SetPass(password);break;
        }
        case CS_G_DELETE:{//解散群
            //>smUUUgg
            InitSigUG();
            SetID(-1);
            SetGID(gid);break;
        }
		case CS_S_OK:{
        #ifdef NON_USE_CS_OK
            memset(SendBuf, '\0', strlen(SendBuf));
			return;
        #endif
            InitSig();
			break;
		}
        default:{
            WriteLog("Message.cpp:488 Send Sig error");
        }
    }

    SendBuf[1] = (seq + '0');seq ^= 1;
    SendBuf[2] = sig;
    sem_server.Send2Pipe();
}

//sxx
void ServerSig::Send_U_Text(){
    /* 更新本地消息列表 */
    UserInfo_SendFMess();

    //if(Len_error)长度错误, 重新发送
    int nowIdx = 0;
    int message_len = message.size();
    SendBuf[nowIdx++] = __10_64_char(seq);seq ^= 1;
    /* 总长度 */
    strcpy(&SendBuf[nowIdx], __10_64_string(message_len + 4 + strlen(Text_UserInfoMess), 2).c_str());nowIdx += 2;

    SendBuf[nowIdx++] = 'U';
    strcpy(&SendBuf[nowIdx], __10_64_string(youid, 3).c_str());nowIdx += 3;
    strcpy(&SendBuf[nowIdx], get_time_64().c_str());nowIdx += 7;
    SendBuf[nowIdx++] = '|';
    SendBuf[nowIdx++] = ' ';
    strcpy(&SendBuf[nowIdx], message.c_str());nowIdx += message_len;
    SendBuf[nowIdx++] = ' ';
    SendBuf[nowIdx++] = '|';

    sem_server.Send2Pipe();
}

void ServerSig::Send_G_Text(){
    /* 更新本地消息列表 */
    UserInfo_SendGMess();

    //if(Len_error)长度错误, 重新发送
    int nowIdx = 0;
    int message_len = message.size();
    SendBuf[nowIdx++] = __10_64_char(seq);seq ^= 1;
    strcpy(&SendBuf[nowIdx], __10_64_string(message_len + 4 + strlen(Text_GroupInfoMess), 2).c_str());nowIdx += 2;
    SendBuf[nowIdx++] = 'G';
    strcpy(&SendBuf[nowIdx], __10_64_string(myid, 3).c_str());nowIdx += 3;
    strcpy(&SendBuf[nowIdx], __10_64_string(gid, 2).c_str());nowIdx += 2;
    strcpy(&SendBuf[nowIdx], get_time_64().c_str());nowIdx += 7;
    SendBuf[nowIdx++] = '|';
    SendBuf[nowIdx++] = ' ';
    strcpy(&SendBuf[nowIdx], message.c_str());nowIdx += message_len;
    SendBuf[nowIdx++] = ' ';
    SendBuf[nowIdx++] = '|';

    sem_server.Send2Pipe();
}







/* tool */
std::string ServerSig::getstr(int l, int r, bool ok){    
    assert(l <= r);
    std::string s = "";
    for(int i=l;i<=r;i++){
        if(RecvBuf[nowIdx + i] == ' ')break;
        s.push_back(RecvBuf[nowIdx + i]);
    }
    if(ok)return s;
    return std::to_string(__64_10(s));
}
int ServerSig::getNum(int l, int r){
    return __64_10(getstr(l, r, 1));
}
int ServerSig::GetID(){
    return getNum(3, 5);
}
int ServerSig::GetGID(){
    return getNum(6, 7);
}
