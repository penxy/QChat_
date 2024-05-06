#ifndef __QTSIG_H
#define __QTSIG_H

#include "CS_Sig_config.h"
#include "ServerSig.h"
#include "tool.h"
#include "QC_Sig_config.h"
#include <jsoncpp/json/json.h>
#include "SemQt.h"

extern SemQt sem_qt;

class QtSig : public ServerSig{
public:
    QtSig();
    virtual ~QtSig();
private:
//主要逻辑在UserInfo, 这个是调用主要逻辑后，调用发送信号的函数
	/* Friend */
	void FriendOfUser_AllowFriend();
    void FriendOfUser_RejectFriend();
    void FriendOfUser_AddFriend();
    void FriendOfUser_DelFriend();
    void FriendOfUser_SearchUser();    
	/* Group */
	//LeaderID
    void GroupOfUser_Rename();
    void GroupOfUser_SetLeader();
    void GroupOfUser_AllowMember();
    void GroupOfUser_DelMember();
    void GroupOfUser_InviteMember();
    void GroupOfUser_RejectMember();
    void GroupOfUser_Delete();
    //Not LeaderID
    void GroupOfUser_CreateGroup();
    void GroupOfUser_InsertGroup();
    void GroupOfUser_QuitGroup();
    void GroupOfUser_SearchGroup();
	/*User*/
    void Delete();
    void Rename();
    void ChangePassword();
    void Create();
    void Login();
    void Logout();
	/*QT*/
	void FriendOfUser_ShowAllBeAdd();
    void FriendOfUser_ShowAllAdd();
    void FriendOfUser_ShowALLFriends();
    void GroupOfUser_ShowRequestInsert();
    void ShowAll_ServerMessage();
	/*QT Clear Message*/
    void Clear_FriendMess();
    void Clear_GroupMess();
    void Remove_NoticeMess();

    void Send_FText();
    void Send_GText();
private:
	void RecvUText();
    void RecvGText();

    /*  CQ  */
public:
    /* recv from Qt */
    void RecvQTSig();
private:
    /* 通过管道, 发送信号给QT  */
    void SendQTSig(char sig);
    void Clear_R_Buf();

    /* wait server send, and then send to qt */
    void Wait_Server();
private:
    char* RecvBuf;
    char* SendBuf;
};


#endif
