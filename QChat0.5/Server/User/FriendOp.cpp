#include "FriendOp.h"


//have a init signal string ">s?UUUGG" and ">s?UUU" and ">s?"



FriendOp::FriendOp() : friend_size(0){
    friends = new int[MAX_FRIENDS_SIZE];
}
FriendOp::~FriendOp(){
    delete[] friends;
}
void FriendOp::FriendOp_Allow(int youid){
    FIdGetIdx[youid] = friend_size;
    friends[friend_size] = youid;
    friend_size++;
}
void FriendOp::FriendOp_Del(int youid){
    std::swap(FIdGetIdx[youid], FIdGetIdx[friend_size - 1]);
    friend_size--;
}