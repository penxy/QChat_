/*
* author: pxy
* 2024-03-26 22:44:16 星期二
*/
#include "QtSig.h"
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

QtSig::QtSig(){
    SendBuf = sem_qt.Get_SendSharedMemory();
    RecvBuf = sem_qt.Get_RecvSharedMemory();
}
QtSig::~QtSig(){
    //...如果不是注销, 将数据写到数据库
}
void QtSig::Send_FText(){
    UserInfo_SendFMess();
    Send_U_Text();
}
void QtSig::Send_GText(){
    UserInfo_SendGMess();
    Send_G_Text();
}

void QtSig::FriendOfUser_AllowFriend(){
    if(!UserInfo_Friend_Allow())return;
    SendSig(CS_F_ALLOW);
}

void QtSig::FriendOfUser_RejectFriend(){
    UserInfo_Friend_Reject();
	SendSig(CS_F_REJECT);
}

void QtSig::FriendOfUser_AddFriend(){
    if(!UserInfo_Friend_Add())return;
    SendSig(CS_F_ADD);
}
void QtSig::FriendOfUser_DelFriend(){
    UserInfo_Friend_Del();
    SendSig(CS_F_DEL);
}
void QtSig::FriendOfUser_SearchUser(){
    SendSig(CS_FIND_ID);
}



/* ---------------------------------------GroupOfUser--------------------------------------- */
void QtSig::GroupOfUser_Rename(){    
    SendSig(CS_G_RENAME);
    UserInfo_Group_Rename();
}

void QtSig::GroupOfUser_SetLeader(){
    SendSig(CS_G_SETLEADER);
    UserInfo_Group_SetLeader();
}


void QtSig::GroupOfUser_AllowMember(){
    if(!UserInfo_Group_AllowMember())return;
    SendSig(CS_G_ALLOW);
}

void QtSig::GroupOfUser_DelMember(){
    if(!UserInfo_Group_DelMember())return;
    SendSig(CS_G_REMOVE);
}

void QtSig::GroupOfUser_InviteMember(){
    if(!UserInfo_Group_InviteMember())return;
    SendSig(CS_G_INVITE);
}

void QtSig::GroupOfUser_RejectMember(){
    UserInfo_Group_RejectMember();
    SendSig(CS_G_REJECT);
}

void QtSig::GroupOfUser_Delete(){
    if(!UserInfo_Group_Delete())return;
    SendSig(CS_G_DELETE);
}

void QtSig::GroupOfUser_CreateGroup(){
    if(!UserInfo_Group_CreateGroup())return;
    SendSig(CS_G_CREATE);
}


void QtSig::GroupOfUser_InsertGroup(){
    if(!UserInfo_Group_InsertGroup())return;
    SendSig(CS_G_INSERT);
}

void QtSig::GroupOfUser_QuitGroup(){
    if(!UserInfo_Group_QuitGroup())return;
    SendSig(CS_G_QUIT);
}

void QtSig::GroupOfUser_SearchGroup(){
    SendSig(CS_FIND_GID);
}
/* ----------------------------------User--------------------------------------- */


void QtSig::Delete(){
    UserInfo_User_Delete();
    SendSig(CS_U_DELETE);
    //一但调用Delete, 析构User, 重新new一个, 并返回到登录界面, 并删掉数据库对应的表格
    //直接退出程序和main.cpp, 不需要把东西写到数据库(此时数据库已清空) 
}
void QtSig::Rename(){
    UserInfo_User_Rename();
    SendSig(CS_U_RENAME);
}
void QtSig::ChangePassword(){
	UserInfo_User_ChangePassword();
    SendSig(CS_U_PASS);
}
void QtSig::Create(){
    UserInfo_User_Create();
    SendSig(CS_U_CREATE);
}
void QtSig::Login(){
    UserInfo_User_Login();
    SendSig(CS_U_LOGIN);
}
void QtSig::Logout(){
    UserInfo_User_Logout();
}

void QtSig::Clear_FriendMess(){
    UserInfo_Clear_FriendMess();
}
void QtSig::Clear_GroupMess(){
    UserInfo_Clear_GroupMess();
}
void QtSig::Remove_NoticeMess(){
    UserInfo_Clear_NoticeMess();
}




void QtSig::Wait_Server(){
    sem_wait(&sem_recv_server);
}


void QtSig::SendQTSig(char sig){
	Json::Value pack;
    std::string temp = "";
    temp.push_back(sig);

    pack["sig"] = temp;
    Json::Value arr(Json::arrayValue);
    Json::Value OneOfArr;

    switch(sig){
    case CQ_CREATEUSERYES:{
        pack["id"] = myid;break;
    }
    case CQ_CREATEGROUPYES:{
        pack["gid"] = gid;break;
    }
    case CQ_SEARCHIDYES:{
        pack["name"] = Rname;break;
    }
    case CQ_SEARCHGIDYES:{
        pack["name"] = Rname;break;
    }
    case CQ_FRIENDLSIT:{
        for(int i=0;i<FriendsSize;i++){
            OneOfArr["id"] = FriendInfo[i].id;
            OneOfArr["name"] = FriendInfo[i].name;
            //std::cout << "FriendInfo[i].name: " << FriendInfo[i].name << "\n";
            pack["arr"].append(OneOfArr);
        }break;
    }
    case CQ_FRIENDMESS:{
        //time mess
        int idx = FIdGetIdx[youid];
        for(const auto& temp : FriendInfo[idx].Message){
			OneOfArr["type"] = temp.type;
            OneOfArr["time"] = temp.time;
            OneOfArr["mess"] = temp.mess;
            pack["arr"].append(OneOfArr);
        }break;
    }
    case CQ_GROUPLIST:{
        for(int i=0;i<GroupsSize;i++){
            OneOfArr["gid"] = GroupInfo[i].gid;
            OneOfArr["name"] = GroupInfo[i].name;
            pack["arr"].append(OneOfArr);
        }break;
    }
    case CQ_GROUPMEMBER:{
        int idx = GIdGetIdx[gid];
        for(auto x : GroupInfo[idx].MemberId){
            OneOfArr["id"] = x;
            pack["arr"].append(OneOfArr);
        }break;
    }
    case CQ_GROUPMESS:{
        int idx = GIdGetIdx[gid];
        for(const auto&temp : GroupInfo[idx].Message){
            OneOfArr["type"] = temp.type;
            OneOfArr["id"] = temp.id;
            OneOfArr["mess"] = temp.mess;
            OneOfArr["time"] = temp.time;
            pack["arr"].append(OneOfArr);
        }break;
    }
    case CQ_MYADD:{
        for(const auto& temp : F_Add){
            OneOfArr["id"] = temp.id;
            OneOfArr["name"] = temp.name;
            pack["arr"].append(OneOfArr);
        }break;
    }
    case CQ_BEADD:{
        for(const auto& temp : F_BeAdd){
            OneOfArr["id"] = temp.id;
            OneOfArr["name"] = temp.name;
            pack["arr"].append(OneOfArr);
        }break;
    }
    case CQ_GMYADD:{
        for(const auto& temp : G_Add){
            OneOfArr["gid"] = temp.id;
            OneOfArr["name"] = temp.name;
            pack["arr"].append(OneOfArr);
        }break;
    }
    case CQ_NOTICE:{
        for(const auto& temp : Notice){
            OneOfArr["mess"] = temp;
            pack["arr"].append(OneOfArr);
        }break;
    }

    case CQ_MYGROUP:{
        for(int i=0;i<GroupsSize;i++){
            if(GroupInfo[i].leaderId != myid)continue;
            OneOfArr["gid"] = GroupInfo[i].gid;
            OneOfArr["name"] = GroupInfo[i].name;
            pack["arr"].append(OneOfArr);
        }break;
    }
    case CQ_MYGROUP_BEINSERT:{
        int idx = GIdGetIdx[gid];
        for(auto x : GroupInfo[idx].AllBeInsert){
            OneOfArr["id"] = x;
            pack["arr"].append(OneOfArr);
        }break;
    }
    case CQ_MYGROUP_MEMBER:{
        int idx = GIdGetIdx[gid];
        for(auto x : GroupInfo[idx].MemberId){
            OneOfArr["id"] = x;
            pack["arr"].append(OneOfArr);
        }break;
    }
    default:{
        break;
    }
    }
    Json::StreamWriterBuilder build;
    strcpy(SendBuf, Json::writeString(build, pack).c_str());

    sem_qt.Send2Pipe();    
}


void QtSig::RecvQTSig(){
    Json::Value pack;
    Json::Reader reader;

    assert(reader.parse(std::string(RecvBuf), pack));
    Clear_R_Buf();
    
    auto YouID = [&](){youid = pack["youid"].asInt();};
    auto MyID = [&](){myid = pack["myid"].asInt();};
    auto GID = [&](){gid = pack["gid"].asInt();};
    auto Mess = [&](){message = pack["mess"].asString();};
    auto Name = [&](){Sname = pack["name"].asString();};
    auto Pass = [&](){password = pack["pass"].asString();};

    char Sig = pack["sig"].asString()[0];
    Json::Value SendPack;
    switch(Sig){
    case QC_FALLOW:{
        YouID();Name();
        FriendOfUser_AllowFriend();break;
    }
    case QC_FREJECT: {
        YouID();Name();
        FriendOfUser_RejectFriend();break;
    }
    case QC_FADD:{
        YouID();
        Name();
        FriendOfUser_AddFriend();        
        break;
    }
    case QC_FDEL:{
        YouID();
        FriendOfUser_DelFriend();break;
    }
    case QC_USEARCHUSER:{//ID
        YouID();
        FriendOfUser_SearchUser();

        Wait_Server();
        if(OK)SendQTSig(CQ_SEARCHIDYES);
        else SendQTSig(CQ_SEARCHIDNO);
        break;
    }
    case QC_GRENAME:{
        GID();Name();
        GroupOfUser_Rename();break;
    }
    case QC_GSETLEADER:{
        GID();YouID();
        GroupOfUser_SetLeader();break;
    }
    case QC_GALLOWMEMBER:{
        GID();YouID();
        GroupOfUser_AllowMember();break;
    }
    case QC_GDELMAMBER:{
        GID();YouID();
        GroupOfUser_DelMember();break;
    }
    case QC_GINVITEMEMBER:{
        GID();YouID();
        GroupOfUser_InviteMember();break;
    }
    case QC_GREJECTMEMBER:{
        GID();YouID();
        GroupOfUser_RejectMember();break;
    }
    case QC_GCREATEGROUP:{//create Group
        Name();
        GroupOfUser_CreateGroup();

        Wait_Server();
        if(OK)SendQTSig(CQ_CREATEGROUPYES);
        else SendQTSig(CQ_CREATEGROUPNO);
        break;
    }
    case QC_GREQUESTINSERTGROUP:{
        GID();
        GroupOfUser_InsertGroup();break;
    }
    case QC_GDELETE:{
        GID();
        GroupOfUser_Delete();break;
    }
    case QC_GQUITGROUP:{//delete name, 4.2
        GID();
        GroupOfUser_QuitGroup();break;
    }
    case QC_GSEARCHGROUP:{
        GID();
        GroupOfUser_SearchGroup();

        Wait_Server();
        if(OK)SendQTSig(CQ_SEARCHGIDYES);
        else SendQTSig(CQ_SEARCHGIDNO);
        break;
    }
    case QC_USERDELETE:{//User delete
        Delete();break;
    }
    case QC_USERRENAME:{
        Name();
        Rename();break;
    }
    case QC_USERCHANGEPASSWORD:{//password
        Pass();Sname = pack["NewPass"].asString();ChangePassword();
        
        Wait_Server();
        if(OK)SendQTSig(CQ_SETPASSWORDYES);
        else SendQTSig(CQ_SETPASSWORDNO);
        break;
    }
    case QC_USERCREATE:{//ID Create
        Name();Pass();Create();

        Wait_Server();
        if(OK)SendQTSig(CQ_CREATEUSERYES);
        else SendQTSig(CQ_CREATEUSERNO);
        break;
    }
    case QC_USERLOGIN:{
        MyID();
        Pass();
        Login();

        Wait_Server();
        if(OK)SendQTSig(CQ_LOGINYES);
        else SendQTSig(CQ_LOGINNO);
        break;
    }
    case QC_FMESSAGECLEAR:{
        YouID();Clear_FriendMess();break;
    }
    case QC_GMESSAGECLEAR:{
        GID();Clear_GroupMess();break;
    }
    case QC_FSENDMESSAGE:{
        YouID();Mess();
		
        Send_U_Text();break;
    }
    case QC_GSENDMESSAGE:{
        GID();Mess();Send_G_Text();break;
    }
    case QC_REMOVENOTICE:{
        Mess();Remove_NoticeMess();break;
    }

    case QC_FRIENDLIST:{
        SendQTSig(CQ_FRIENDLSIT);break;
    }
    case QC_FRIENDMESS:{
        YouID();
        SendQTSig(CQ_FRIENDMESS);break;
    }
    case QC_GROUPLIST:{
        SendQTSig(CQ_GROUPLIST);break;
    }
    case QC_GROUPMEMBER:{
        GID();
        SendQTSig(CQ_GROUPMEMBER);break;
    }
    case QC_GROUPMESS:{
        GID();
        SendQTSig(CQ_GROUPMESS);break;
    }
    case QC_MYADD:{
        SendQTSig(CQ_MYADD);break;
    }
    case QC_BEADD:{
        SendQTSig(CQ_BEADD);break;
    }
    case QC_GMYADD:{
        SendQTSig(CQ_GMYADD);break;
    }
    case QC_NOTICE:{
        SendQTSig(CQ_NOTICE);break;
    }
    case QC_MYGROUP:{
        SendQTSig(CQ_MYGROUP);break;
    }
    case QC_MYGROUP_BEINSERT:{
        GID();
        SendQTSig(CQ_MYGROUP_BEINSERT);break;
    }
    case QC_MYGROUP_MEMBER:{
        GID();
        SendQTSig(CQ_MYGROUP_MEMBER);break;
    }
    case QC_USERLOGOUT:{
        //断开与Server的连接，并delete User;重新new一个
		break;
	}    
    default:{
        WriteLog("QtSig.cpp:497, SendQTSig error");
    }
    }
}



void QtSig::Clear_R_Buf(){
    memset(RecvBuf, '\0', strlen(RecvBuf));
}  




