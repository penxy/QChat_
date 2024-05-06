#include "User.h"


User::User(){}


void User::User_Allow(){
    FriendOp_Allow(youid);
}
void User::User_Del(){
    FriendOp_Del(youid);
}
BaseInfo User::User_FindID(){
    if(myid == -1)return BaseInfo(-1, "");
    return BaseInfo(myid, myname);
}
bool User::User_Login(const std::string& pass){
    if(myid == -1)return false;
    std::cout << "password check: " << pass << " " << mypassword << "\n";

    if(pass != mypassword)return false;

    this->myid = myid;
    return true;
}
void User::User_Logout(){
    
}


