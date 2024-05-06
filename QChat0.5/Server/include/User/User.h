#ifndef __USER_H
#define __USER_H

#include "UserInfo.h"
#include "FriendOp.h"

struct BaseInfo{//id and name
    int id;
    std::string name;
    BaseInfo(int id, const std::string& name) : id(id), name(name){}
};

/* 执行逻辑 */
class User : public UserInfo, public FriendOp{
public:
    User();
    virtual ~User() = default;
public:
    void User_Allow();
    void User_Del();

    BaseInfo User_FindID();//if id == -1, the user id not useful;

    bool User_Login(const std::string& pass);
    void User_Logout();
};



#endif