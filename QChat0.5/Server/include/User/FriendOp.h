#ifndef __FRIENDOP_H
#define __FRIENDOP_H
#include "config.h"
#include <unordered_map>
#include <string>


//have a init signal string ">s?UUUGG" and ">s?UUU" and ">s?"
#define CS_F_ALLOW						'0'						//  同意申请					>s0uuu				[youid]
#define CS_F_DEL						'3'						//	删除好友					>s3uuu				[youid]


class FriendOp{
public:
    FriendOp();
    virtual ~FriendOp();
protected:
    void FriendOp_Allow(int youid);
    void FriendOp_Del(int youid);
private:
    int* friends;
    int friend_size;
    std::unordered_map<int, int>FIdGetIdx;
};




#endif