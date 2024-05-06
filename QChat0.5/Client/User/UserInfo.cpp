#include "UserInfo.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <syslog.h>
#include <fcntl.h>
#include <unistd.h>

#include <fstream>
#include <sstream>

void UserInfo::Sort_FriendID(){
    auto HeapSort = [&](auto HeapSort, int node, int all) -> void{
        int p = node;
        int left = (node << 1) +  1;
        int right = left + 1;
        if(left < all && FriendInfo[p].id < FriendInfo[left].id)p = left;
        if(right < all && FriendInfo[p].id < FriendInfo[right].id)p = right;

        if(p == node)return;
		std::swap(FIdGetIdx[FriendInfo[node].id], FIdGetIdx[FriendInfo[p].id]);
        std::swap(FriendInfo[node], FriendInfo[p]);
        HeapSort(HeapSort, p, all);
    };
    for(int i=FriendsSize/2;i>=0;i--){
        HeapSort(HeapSort, i, FriendsSize);
    }
    for(int i=FriendsSize-1;i>=0;i--){
		std::swap(FIdGetIdx[FriendInfo[0].id], FIdGetIdx[FriendInfo[i].id]);
		std::swap(FriendInfo[0], FriendInfo[i]);
        HeapSort(HeapSort, 0, i);
    }
}
void UserInfo::Sort_GroupGID(){
    auto HeapSort = [&](auto HeapSort, int node, int all) -> void{
        int p = node;
        int left = (node << 1) +  1;
        int right = left + 1;
        if(left < all && GroupInfo[p].gid < GroupInfo[left].gid)p = left;
        if(right < all && GroupInfo[p].gid < GroupInfo[right].gid)p = right;

        if(p == node)return;
		std::swap(GIdGetIdx[GroupInfo[node].gid], GIdGetIdx[GroupInfo[p].gid]);
        std::swap(GroupInfo[node], GroupInfo[p]);
        HeapSort(HeapSort, p, all);
    };
    for(int i=GroupsSize/2;i>=0;i--){
        HeapSort(HeapSort, i, GroupsSize);
    }
    for(int i=GroupsSize-1;i>=0;i--){
		std::swap(GIdGetIdx[GroupInfo[0].gid], GIdGetIdx[GroupInfo[i].gid]);
        std::swap(GroupInfo[0], GroupInfo[i]);
        HeapSort(HeapSort, 0, i);
    }
}
void UserInfo::UserInfo_DebugInit(){
    std::string __time[3] = {"2021-03-12 15:13:12", "2022-03-12 14:12:12", "2023-03-12 13:13:11"};

    auto AddG = [&](int x){
        GroupInfo[GroupsSize].gid = x;
        GIdGetIdx[GroupInfo[GroupsSize].gid] = GroupsSize;
        GroupInfo[GroupsSize].leaderId = myid;
        GroupInfo[GroupsSize].Size = 8;
        strcpy(GroupInfo[GroupsSize].name, (x % 2 == 0 ? "one_name" : "two_name"));
        for(int i=2;i<10;i++)GroupInfo[GroupsSize].MemberId.insert(i);

        for(int i=2;i<10;i++){
            GroupInfo[GroupsSize].Message.push_back(GMess(0, i, "Hello", __time[0]));
        }
        for(int i=0;i<5;i++){
            GroupInfo[GroupsSize].AllBeInsert.insert(49 + i);
        }
        GroupsSize++;
    };
    auto Add_Other_G = [&](int x){
        GroupInfo[GroupsSize].gid = x;
        GIdGetIdx[GroupInfo[GroupsSize].gid] = GroupsSize;
        GroupInfo[GroupsSize].leaderId = 1;
        GroupInfo[GroupsSize].Size = 9;
        strcpy(GroupInfo[GroupsSize].name, (x % 2 == 0 ? "one_name" : "two_name"));
        for(int i=2;i<10;i++)GroupInfo[GroupsSize].MemberId.insert(i);
        GroupInfo[GroupsSize].MemberId.insert(myid);

        for(int i=2;i<10;i++){
            GroupInfo[GroupsSize].Message.push_back(GMess(0, i, "Hello", __time[0]));
        }
        for(int i=0;i<5;i++){
            GroupInfo[GroupsSize].AllBeInsert.insert(49 + i);
        }
        GroupsSize++;
    };
    auto AddF = [&](int x){
        FriendInfo[FriendsSize].id = x;
        FIdGetIdx[FriendInfo[FriendsSize].id] = FriendsSize;
        strcpy(FriendInfo[FriendsSize].name, "李四");
        for(int i=0;i<10;i++)FriendInfo[FriendsSize].Message.push_back(FMess(0, "World", __time[0]));
        FriendsSize++;
    };
    myid = 0;
    Mypassword = "12345";
    strcpy(MyName, "张三");

    for(int i=0;i<6;i++)AddG(i);
    for(int i=12;i<15;i++)Add_Other_G(i);
    for(int i=1;i<10;i++)AddF(i);

    F_Add.push_back({1, "Tom1"});
    F_Add.push_back({2, "Tom2"});
    F_Add.push_back({3, "Tom3"});
    F_Add.push_back({4, "Tom4"});
    F_Add.push_back({5, "Tom5"});

    F_AddID.insert(1);
    F_AddID.insert(2);
    F_AddID.insert(3);
    F_AddID.insert(4);
    F_AddID.insert(5);

    F_BeAdd.push_back({1, "Tom1"});
    F_BeAdd.push_back({6, "Tom6"});
    F_BeAdd.push_back({7, "Tom7"});

    F_BeAddID.insert(1);
    F_BeAddID.insert(6);
    F_BeAddID.insert(7);

    G_Add.push_back({11, "Tom_jo"});

    G_AddID.insert(11);

    Notice.insert("Hello World");
    Notice.insert("Hello World1");
}








/*-------------------------------------------------SQL----------------------------------------------------*/
void WriteInCSV(std::ostringstream& os, const std::string& file_path){
    std::ofstream file(file_path);
    assert(file.is_open());
    file << os.str();
    file.close();
    os.str("");
}

void UserInfo::Save(){
    std::ostringstream os;
    os << "id,name\n";
    for(int i=0;i<FriendsSize;i++){
        os << FriendInfo[i].id << "," << FriendInfo[i].name << "\n";
    }
    WriteInCSV(os, file_path[0]);

    os << "id,type,time,message\n";
    for(int i=0;i<FriendsSize;i++){
        for(int j=0;j<FriendInfo[i].Message.size();j++)
            os << FriendInfo[i].id << "," << FriendInfo[i].Message[j].type << "," << FriendInfo[i].Message[j].time << "," << FriendInfo[i].Message[j].mess << "\n";
    }
    WriteInCSV(os, file_path[1]);

    os << "id,name\n";
    for(int i=0;i<F_Add.size();i++){
        os << F_Add[i].id << "," << F_Add[i].name << "\n";
    }
    WriteInCSV(os, file_path[2]);

    os << "id,name\n";
    for(int i=0;i<F_BeAdd.size();i++){
        os << F_BeAdd[i].id << "," << F_BeAdd[i].name << "\n";
    }
    WriteInCSV(os, file_path[3]);

    char cmd[64];
    snprintf(cmd, sizeof(cmd), "./SQL/write.sh %d", myid);
    system(cmd);
}

void UserInfo::ReadInCSV(int idx){//idx of file_path[]
    static char cmd[512];
    static char buffer[512];

    bool __first_line = false;//to ignore first line
    int line_idx = 0;

    //idx == 1
    FMess fmess;
    int id;
    //idx == 2, 3
    Add_Info add_info;


    std::ifstream in(file_path[idx]);
    std::string line;
    while(std::getline(in, line)){
        if(!__first_line){
            __first_line = true;
            continue;
        }

        //run the shell to get infomathion of string
        snprintf(cmd, 512, "./Shell/StrOp.sh %s %c", line.c_str(), ',');
        FILE* pipe = popen(cmd, "r");
        memset(cmd, '\0', strlen(cmd));

        
        line_idx = 0;
        while (fgets(buffer, sizeof(buffer), pipe) != NULL) {  
            buffer[strcspn(buffer, "\n")] = 0;
            if(idx == 0){//id name
                if(line_idx == 0){
                    FriendInfo[FriendsSize].id = std::stoi(buffer);
                    FIdGetIdx[id] == FriendsSize;
                    line_idx = 1;
                }else if(line_idx == 1){
                    strcpy(FriendInfo[FriendsSize].name, buffer);
                    FriendsSize++;
                    line_idx = 0;
                }else{
                    WriteLog("ReadInCSV error");
                }   
            }else if(idx == 1){//id type time mess
                if(line_idx == 1){
                    id = std::stoi(buffer);
                    line_idx++;
                }else if(line_idx == 2){
                    fmess.type = std::stoi(buffer);
                    line_idx++;
                }else if(line_idx == 3){
                    fmess.time = buffer;
                    line_idx++;
                }else if(line_idx == 4){
                    fmess.mess = buffer;
                    FriendInfo[FIdGetIdx[id]].Message.push_back(fmess);
                    line_idx = 0;
                }else{
                    WriteLog("ReadInCSV error");
                }
            }else if(idx == 2){//id name
                if(line_idx == 0){
                    add_info.id = std::stoi(buffer);
                    line_idx++;
                }else if(line_idx == 1){
                    add_info.name = buffer;
                    F_Add.push_back(add_info);
                    line_idx = 0;
                }else{
                    WriteLog("ReadInCSV error");
                }
            }else if(idx == 3){//id name
                if(line_idx == 0){
                    add_info.id = std::stoi(buffer);
                    line_idx++;
                }else if(line_idx == 1){
                    add_info.name = buffer;
                    F_BeAdd.push_back(add_info);
                    line_idx = 0;
                }else{
                    WriteLog("ReadInCSV error");
                }
            }
            memset(buffer, '\0', strlen(buffer));  
        }  

        int ret = pclose(pipe);
        assert(ret == 0);
    }
}
void UserInfo::Load(){
    //read db to csv
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "./SQL/read.sh %d", myid);
    system(cmd);

    for(int i=0;i<len_file_path;i++){
        ReadInCSV(i);
    }
}

void UserInfo::CreateSQL(){
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "./SQL/CreateTable.sh %d", myid);
    system(cmd);
}




void UserInfo::UserInfo_ClearInfo(){
    for(int i=0;i<FriendsSize;i++)FriendInfo[i].Clear();
    for(int i=0;i<GroupsSize;i++)GroupInfo[i].Clear();
    delete[] FriendInfo;
    delete[] GroupInfo;
}

void UserInfo::Remove_FriendInfo(){
    int idx = FIdGetIdx[youid];
    FriendInfo[idx].Clear();
    FIdGetIdx[FriendInfo[FriendsSize - 1].id] = idx;
    std::swap(FriendInfo[idx], FriendInfo[FriendsSize - 1]);
    FriendsSize--;
    Sort_FriendID();
}
void UserInfo::Remove_GroupInfo(){
    int idx = GIdGetIdx[gid];
    GroupInfo[idx].Clear();
    GIdGetIdx[GroupInfo[GroupsSize - 1].gid] = idx;
    std::swap(GroupInfo[idx], GroupInfo[GroupsSize - 1]);
    GroupsSize--;
    Sort_GroupGID();
}
void UserInfo::Remove_Add(){
    if(F_AddID.count(youid) == 0)return;
	int idx = -1;
    for(int i=0;i<(int)F_Add.size()&&idx==-1;i++){
        if(F_Add[i].id == youid)idx = i;
    }
    F_AddID.erase(youid);
    F_Add.erase(F_Add.begin() + idx);
}
void UserInfo::Remove_BeAdd(){
	if(F_BeAddID.count(youid) == 0)return;
    int idx = -1;
    for(int i=0;i<(int)F_BeAdd.size()&&idx==-1;i++){
        if(F_BeAdd[i].id == youid)idx = i;
    }
    F_BeAddID.erase(youid);
    F_BeAdd.erase(F_BeAdd.begin() + idx);
}
void UserInfo::Remove_AddG(){
	if(G_AddID.count(youid) == 0)return;
    int idx = -1;
    for(int i=0;i<(int)G_Add.size()&&idx==-1;i++){
        if(G_Add[i].id == youid)idx = i;
    }
    G_AddID.erase(youid);
    G_Add.erase(G_Add.begin() + idx);
}
bool UserInfo::Add_FriendInfo(){
    if(FriendsSize == MAX_FRIENDS_SIZE)return 0;
    FIdGetIdx[youid] = FriendsSize;
    FriendInfo[FriendsSize].id = youid;
    strcpy(FriendInfo[FriendsSize].name, Sname.c_str());
    FriendsSize++;
    Sort_FriendID();
    return 1;
}
bool UserInfo::Add_GroupInfo(){
    if(GroupsSize == MAX_GROUPS_SIZE)return 0;
    GIdGetIdx[gid] = GroupsSize;
    GroupInfo[GroupsSize].gid = gid;
    strcpy(GroupInfo[GroupsSize].name, Sname.c_str());
    GroupsSize++;
    Sort_GroupGID();
    return 1;
}
bool UserInfo::Add_F_Add(){
    if(F_AddID.count(youid))return 0;
    F_AddID.insert(youid);
    F_Add.push_back(Add_Info(youid, Sname));
    return 1;
}
bool UserInfo::Add_F_BeAdd(){
    if(F_BeAddID.count(youid))return 0;
    F_BeAddID.insert(youid);
    F_BeAdd.push_back(Add_Info(youid, Sname));
    return 1;
}
bool UserInfo::Add_G_Add(){
    if(G_AddID.count(gid))return 0;
    G_AddID.insert(gid);
    G_Add.push_back(Add_Info(gid, Sname));
    return 1;
}
void UserInfo::Add_Notice(const std::string& s){
    Notice.insert(s);
}















/*   Send    */
void UserInfo::UserInfo_SendFMess(){
    FriendInfo[FIdGetIdx[youid]].Message.push_back(FMess(1, message, get_time_10()));
}
void UserInfo::UserInfo_SendGMess(){
    GroupInfo[GIdGetIdx[gid]].Message.push_back(GMess(1, myid, message, get_time_10()));
}
bool UserInfo::UserInfo_Friend_Allow(){
    if(!Add_FriendInfo())return 0;
    Remove_BeAdd();
    return 1;
}
void UserInfo::UserInfo_Friend_Reject(){
    Remove_BeAdd();
}
bool UserInfo::UserInfo_Friend_Add(){
    return Add_F_Add();
}
void UserInfo::UserInfo_Friend_Del(){
    Remove_FriendInfo();
}
void UserInfo::UserInfo_Group_Rename(){
    int idx = GIdGetIdx[gid];
    memset(&GroupInfo[idx].name, '\0', sizeof(GroupInfo[idx].name));
    strcpy(GroupInfo[idx].name, Sname.c_str());
}
void UserInfo::UserInfo_Group_SetLeader(){
    int idx = GIdGetIdx[gid];
    GroupInfo[idx].MemberId.erase(youid);
    GroupInfo[idx].MemberId.insert(myid);
    GroupInfo[idx].leaderId = youid;
}
bool UserInfo::UserInfo_Group_AllowMember(){
    int idx = GIdGetIdx[gid];
    if(GroupInfo[idx].MemberId.count(youid))return 0;//已经在群了
    if(GroupInfo[idx].Size == MAX_MEMBER_SIZE)return 0;
    GroupInfo[idx].MemberId.insert(youid);
    GroupInfo[idx].AllBeInsert.erase(youid);
    GroupInfo[idx].Size++;
    return 1;
}
bool UserInfo::UserInfo_Group_DelMember(){
    if(myid == youid)return 0;//t自己
    int idx = GIdGetIdx[gid];
    GroupInfo[idx].MemberId.erase(youid);
    GroupInfo[idx].Size--;
    return 1;
}
bool UserInfo::UserInfo_Group_InviteMember(){//if youid's GroupsSize == MAX_GROUPS_SIZE ?
    int idx = GIdGetIdx[gid];
    int GIdx = -1;
    for(int i=0;i<GroupsSize&&GIdx == -1;i++) if(GroupInfo[i].gid == gid)GIdx = i;
    if(GIdx == -1){
        Add_Notice("you have not this group");
        return 0;
    }
    if(GroupInfo[GIdx].leaderId != myid){
        Add_Notice("you are not leader in this group");
        return 0;
    }
    if(GroupInfo[idx].MemberId.count(youid))return 0;
    if(GroupInfo[idx].Size == MAX_MEMBER_SIZE)return 0;
    GroupInfo[idx].MemberId.insert(youid);
    GroupInfo[idx].Size++;
    return 1;
}
bool UserInfo::UserInfo_Group_RejectMember(){
	int idx = GIdGetIdx[gid];
	if(GroupInfo[idx].MemberId.count(youid))return 0;
    GroupInfo[idx].AllBeInsert.erase(youid);
	return 1;
}
bool UserInfo::UserInfo_Group_Delete(){
    if(GroupInfo[GIdGetIdx[gid]].Size != 1)return 0;
    Remove_GroupInfo();
    return 1;
}

bool UserInfo::UserInfo_Group_CreateGroup(){
    if(GroupsSize == MAX_GROUPS_SIZE){
        Add_Notice("you groups too much, you can not create new group before quit some groups.");
        return 0;
    }return 1;
}
bool UserInfo::UserInfo_Group_InsertGroup(){
    if(GroupsSize == MAX_GROUPS_SIZE){
        Add_Notice("you groups too much, you can not insert into new group before quit some groups.");
        return 0;
    }
    return Add_G_Add();
}
bool UserInfo::UserInfo_Group_QuitGroup(){
    if(myid == GroupInfo[GIdGetIdx[gid]].leaderId){
        Add_Notice("you are the leaderId of group, you should set new leader before you quit the group.");
        return 0;
    }
    Remove_GroupInfo();
    return 1;
}
void UserInfo::UserInfo_User_Delete(){

}
void UserInfo::UserInfo_User_Rename(){
    memset(MyName, '\0', strlen(MyName));
    strcpy(MyName, Sname.c_str());
}
void UserInfo::UserInfo_User_ChangePassword(){}
void UserInfo::UserInfo_User_Create(){}

void UserInfo::UserInfo_User_Login(){
}
void UserInfo::UserInfo_User_Logout(){
    UserInfo_ClearInfo();
}
void UserInfo::UserInfo_Clear_FriendMess(){
    FriendInfo[FIdGetIdx[youid]].Message.clear();
}
void UserInfo::UserInfo_Clear_GroupMess(){
    GroupInfo[GIdGetIdx[gid]].Message.clear();
}
void UserInfo::UserInfo_Clear_NoticeMess(){
    Notice.erase(message);
}




/* Recv */
void UserInfo::UserInfo_RecvFMess(const FMess&t){
    FriendInfo[FIdGetIdx[youid]].Message.push_back(t);
}
void UserInfo::UserInfo_RecvGMess(const GMess&t){
    GroupInfo[GIdGetIdx[gid]].Message.push_back(t);
}

void UserInfo::UserInfo_Friend_BeAllow(){
    if(!Add_FriendInfo())return;
    Remove_Add();
    Add_Notice("The Message from Server: have new Friend allow you: " + std::to_string(youid));
}
void UserInfo::UserInfo_Friend_BeReject(){
    Add_Notice("The Message from Server: have new Friend reject you: " + std::to_string(youid));
    Remove_Add();
}
void UserInfo::UserInfo_Friend_BeAdd(){
    Add_Notice("The Message from Server: have new Friend want to add you: " + std::to_string(youid));
    Add_F_BeAdd();
}
void UserInfo::UserInfo_Friend_BeDel(){
    Add_Notice("The Message from Server: you be deleted by id: " + std::to_string(youid));
    Remove_FriendInfo();
}
void UserInfo::UserInfo_Friend_Rename(){
    int idx = FIdGetIdx[youid];
    memset(FriendInfo[idx].name, '\0', sizeof(FriendInfo[idx].name));
    strcpy(FriendInfo[idx].name, Rname.c_str());
};

void UserInfo::UserInfo_Friend_Delete(){
    Add_Notice("The Message from Server: you Friend delete himself, his id: " + std::to_string(youid));
    Remove_FriendInfo();
}




void UserInfo::UserInfo_Group_BeRename(){
    int idx = GIdGetIdx[gid];
    memset(GroupInfo[idx].name, '\0', sizeof(GroupInfo[idx].name));
    strcpy(GroupInfo[idx].name, Rname.c_str());
}
void UserInfo::UserInfo_Group_BeSetLeader(){
    Add_Notice("The Message from Server: the Group Leader is change, new Leader is: " + std::to_string(youid));
    int idx = GIdGetIdx[gid];
    GroupInfo[idx].leaderId = youid;
    GroupInfo[idx].MemberId.insert(id);
    GroupInfo[idx].MemberId.erase(youid);
}
void UserInfo::UserInfo_Group_BeAllowMember(const std::set<int>&memberId){
    Add_Notice("The Message from Server: you be allow to insert the group: " + std::to_string(gid));
    Add_GroupInfo();
    GroupInfo[GroupsSize - 1].MemberId = memberId;
    GroupInfo[GroupsSize - 1].MemberId.insert(myid);
    GroupInfo[GroupsSize - 1].leaderId = youid;
    GroupInfo[GroupsSize - 1].Size = GroupInfo[GroupsSize - 1].MemberId.size() + 1;
    Remove_AddG();
}
void UserInfo::UserInfo_Group_BeDelMember(){//be t
    Add_Notice("The Message from Server: you be removed from group gid: " + std::to_string(gid));
    Remove_GroupInfo();
}
void UserInfo::UserInfo_Group_BeInviteMember(const std::set<int>&memberId){
    Add_Notice("The Message from Server: you be invite to insert the group: " + std::to_string(gid));
    Add_GroupInfo();
    GroupInfo[GroupsSize - 1].MemberId = memberId;
    GroupInfo[GroupsSize - 1].MemberId.insert(myid);
    GroupInfo[GroupsSize - 1].leaderId = youid;
    GroupInfo[GroupsSize - 1].Size = GroupInfo[GroupsSize - 1].MemberId.size() + 1;
    Remove_AddG();
}
void UserInfo::UserInfo_Group_BeRejectMember(){
    Remove_AddG();
}
void UserInfo::UserInfo_Group_NewMemberRequest(){
    GroupInfo[GIdGetIdx[gid]].AllBeInsert.erase(youid);
}
void UserInfo::UserInfo_Group_MemberQuit(){
    int idx = GIdGetIdx[gid];
    GroupInfo[idx].MemberId.erase(id);
    GroupInfo[idx].Size--;
}

void UserInfo::UserInfo_User_CreateYes(){
    Add_Notice("The Message from Server: you create a account, and it id is: " + std::to_string(youid));
    myid = youid;
    strcpy(MyName, Sname.c_str());
    OK = 1;
}
void UserInfo::UserInfo_User_LoginYes(){
    OK = 1;
}
void UserInfo::UserInfo_Group_CreateYes(){
    Add_Notice("The Message from Server: you create a group, it's gid is: " + std::to_string(gid));
    Add_GroupInfo();
    GroupInfo[GroupsSize].leaderId = myid;
    GroupInfo[GroupsSize].Size = 1;
    OK = 1;
}
void UserInfo::UserInfo_User_SetPassWordYes(){
    Add_Notice("The Message from Server: you reset password.");
    OK = 1;
}

UserInfo::UserInfo() : Log(log_path), FriendsSize(0),  GroupsSize(0), OK(false){
    GroupInfo = new GroupInfoStruct[MAX_GROUPS_SIZE];
    FriendInfo = new FriendInfoStruct[MAX_FRIENDS_SIZE];
    //UserInfo_DebugInit();
}
UserInfo::~UserInfo(){
    delete[] GroupInfo;
    delete[] FriendInfo;
    UserInfo_ClearInfo();
}


