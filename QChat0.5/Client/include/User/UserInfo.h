#ifndef USERINFO_H
#define USERINFO_H
#include "CS_Sig_config.h"
#include "config.h"
#include "tool.h"

#include <set>
#include <vector>
#include <assert.h>
#include <unordered_map>
#include "Log.h"


struct FMess{
    bool type;//0 you->me  1 me->you
    std::string mess;
    std::string time;

    FMess() = default;
    FMess(int type, const std::string& mess, const std::string& time){
        this->type = type;this->mess = mess;this->time = time;
    }
};
struct GMess{
    bool type;//0 you->me  1 me->you
    int id;
    std::string mess;
    std::string time;

    GMess() = default;
    GMess(int type, int id, const std::string& mess, const std::string& time){
        this->type = type;this->id = id;this->mess = mess;this->time = time;
    }
};
struct Add_Info{
    int id;
    std::string name;

    Add_Info() = default;
    Add_Info(int id, const std::string& name){
        this->id = id;this->name = name;
    }
};

struct GroupInfoStruct{
    int gid;
    int Size;
    int leaderId;
    char name[MAX_GROUPNAME_SIZE];
    std::set<int>MemberId;//不包括leaderId
    std::set<int>AllBeInsert;
    std::vector<GMess>Message;//id mess time

    //void Sort(){std::sort(Message.begin(), Message.end(), [&](const GMess& a, const GMess& b){return a.time < b.time;});}

    void Clear(){
        Size = 0;
        MemberId.clear();
        AllBeInsert.clear();
        Message.clear();
        memset(name, '\0', sizeof(name));
    }
    void remove(int idx){
        assert(idx < (int)Message.size());
        Message.erase(Message.begin() + idx);
    }
};
struct FriendInfoStruct{
    int id;
    char name[MAX_USERNAME_SIZE];
    std::vector<FMess>Message;

    //void Sort(){std::sort(Message.begin(), Message.end(), [&](const FMess& a, const FMess& b){return a.time < b.time;});}

    void Clear(){        
        Message.clear();
        memset(name, '\0', sizeof(name));
    }
    void remove(int idx){
        assert(idx < (int)Message.size());
        Message.erase(Message.begin() + idx);
    }
};

class UserInfo : public Log{
public:
    UserInfo();
    virtual ~UserInfo();
public:
    void UserInfo_Connect();
    void UserInfo_DisConnect();
    void UserInfo_ClearInfo();//when Logout;
public:
#ifndef SERVEROK
    std::string Mypassword;
#endif
private:
#ifndef SERVEROK
    void UserInfo_DebugInit();
#endif
protected:
    /* text message */
    void UserInfo_SendFMess();
    void UserInfo_SendGMess();

    bool UserInfo_Friend_Allow();
    void UserInfo_Friend_Reject();
    bool UserInfo_Friend_Add();
    void UserInfo_Friend_Del();

    void UserInfo_Group_Rename();
    void UserInfo_Group_SetLeader();
    bool UserInfo_Group_AllowMember();
    bool UserInfo_Group_DelMember();
    bool UserInfo_Group_InviteMember();
    bool UserInfo_Group_RejectMember();
    bool UserInfo_Group_Delete();//must have only one member in group.

    bool UserInfo_Group_CreateGroup();
    bool UserInfo_Group_InsertGroup();
    bool UserInfo_Group_QuitGroup();

    void UserInfo_User_Delete();
    void UserInfo_User_Rename();
    void UserInfo_User_ChangePassword();
    void UserInfo_User_Create();
    void UserInfo_User_Login();
    void UserInfo_User_Logout();

    void UserInfo_Clear_FriendMess();
    void UserInfo_Clear_GroupMess();
    void UserInfo_Clear_NoticeMess();
protected:
    void UserInfo_RecvFMess(const FMess&);
    void UserInfo_RecvGMess(const GMess&);

    void UserInfo_Friend_BeAllow();
    void UserInfo_Friend_BeReject();
    void UserInfo_Friend_BeAdd();
    void UserInfo_Friend_BeDel();
    void UserInfo_Friend_Rename();
    void UserInfo_Friend_Delete();


    void UserInfo_Group_BeRename();
    void UserInfo_Group_BeSetLeader();
    void UserInfo_Group_BeAllowMember(const std::set<int>&memberId);
    void UserInfo_Group_BeDelMember();
    void UserInfo_Group_BeInviteMember(const std::set<int>&memberId);
    void UserInfo_Group_BeRejectMember();
    void UserInfo_Group_NewMemberRequest();
    void UserInfo_Group_MemberQuit();

    void UserInfo_User_CreateYes();
    void UserInfo_User_LoginYes();
    void UserInfo_Group_CreateYes();
    void UserInfo_User_SetPassWordYes();

protected:
    void Sort_FriendID();
    void Sort_GroupGID();

private:
    void Remove_FriendInfo();
    void Remove_GroupInfo();
    void Remove_Add();
    void Remove_BeAdd();
    void Remove_AddG();

    bool Add_FriendInfo();
    bool Add_GroupInfo();
    bool Add_F_Add();
    bool Add_F_BeAdd();
    bool Add_G_Add();

    void Add_Notice(const std::string& s);

public:
/*SQL*/
    void Save();// save in sql
    void Load();// read in sql
    void CreateSQL();
    void ReadInCSV(int idx);
public:
    int myid;

    /* 由信号初始化 */
    int gid;
    int youid;
    int id;//备用
    std::string Sname;//要发送的名字
    std::string password;
    std::string Rname;
    std::string message;
protected:
    GroupInfoStruct* GroupInfo;
    FriendInfoStruct* FriendInfo;
    int FriendsSize;
    int GroupsSize;
    char MyName[MAX_USERNAME_SIZE];//保存自己的名字
    bool OK;

    std::vector<Add_Info>F_Add;
    std::vector<Add_Info>F_BeAdd;
    std::vector<Add_Info>G_Add;

    std::set<int>F_AddID;
    std::set<int>F_BeAddID;
    std::set<int>G_AddID;

    std::set<std::string>Notice;
    std::unordered_map<int, int>FIdGetIdx;
    std::unordered_map<int, int>GIdGetIdx;
private:
    static constexpr char* log_path = "Log/log.txt";//must change
    int Logfd;


    const char* file_path[4] = {"SQL/Data/FriendTable.csv", "SQL/Data/FriendMessageTable.csv", "SQL/Data/MyAddTable.csv", "SQL/Data/MyBeAddTable.csv"};
    const int len_file_path = 4;
};



#endif // USERINFO_H
