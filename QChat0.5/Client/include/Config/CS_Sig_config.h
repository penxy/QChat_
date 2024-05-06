#ifndef __CS_SIG_CONFIG_H
#define __CS_SIG_CONFIG_H

//have a init signal string ">s?UUUGG" and ">s?UUU" and ">s?"
#define CS_F_ALLOW						'0'						//  同意申请					>s0uuu				[youid]
#define CS_F_REJECT						'1'						//  拒绝申请					>s1uuu				[youid]
#define CS_F_ADD						'2'						//  申请添加好友				>s2uuu(8n)			[youid][Sname]
#define CS_F_DEL						'3'						//	删除好友					>s3uuu				[youid]

#define CS_FIND_ID						'4'                     //  搜索ID						>s4uuu				[youid]

#define CS_G_RENAME						'5'                     //  重命名群					>s5UUUgg(8n)		[gid][Sname]
#define CS_G_SETLEADER					'6'                     //  设置Leader					>s6uuugguuu			[youid][gid][myid]     //after set leader, he can't recv Insert member table
#define CS_G_ALLOW						'7'                     //  同意入群					>s7uuugg			[youid][gid]
#define CS_G_REMOVE						'8'                     //  删除成员					>s8uuugg			[youid][gid]
#define CS_G_INVITE						'9'                     //  邀请入群					>s9uuugg			[youid][gid]
#define CS_G_REJECT						'a'				 		//	拒绝入群申请				>sauuugg			[youid][gid]
#define CS_G_CREATE						'b'                     //  创建群						>sb(8n)				[Sname]
#define CS_G_INSERT						'c'                     //  申请入群					>scUUUgg			[gid]
#define CS_G_DELETE						'd'						//	解散群						>sdUUUgg			[gid]
#define CS_G_QUIT						'e'                     //  离开群						>seUUUgg			[gid]

#define CS_FIND_GID						'f'                     //  搜索GID						>sfUUUgg			[gid]

#define CS_U_DELETE						'g'                     //  注销账号					>sg
#define CS_U_RENAME						'h'                     //  个人重命名					>shUUUGG(8n)		[Sname]
#define CS_U_PASS						'i'                     //  改密码						>siUUUGG(16p)(16P)	[pass][Sname]//old/new
#define CS_U_CREATE						'j'                     //  注册						>sjUUUGG(8n)(16p)	[Sname][pass]
#define CS_U_LOGIN						'k'                     //  登录						>skuuuGG(16p)		[MyId][Sname]

#define CS_S_OK							'l'						//  可以继续发送				>sl

//S -> Client_Send
#define SC_FIND_ID_YES					'0'						// 								>s0uuu(8n)				[youid][Rname]
#define SC_FIND_ID_NO					'1'						//								>s1

#define SC_G_CREATE_YES					'2'						//								>s2UUUgg			[gid]
#define SC_G_CREATE_NO					'3'						//								>s3

#define SC_FIND_GID_YES					'4'						//								>s4UUUgg			[gid]
#define SC_FIND_GID_NO					'5'						//								>s5

#define SC_U_CREATE_YES					'6'						//								>s6uuu				[MyId]
#define SC_U_CREATE_NO					'7'						//								>s7

#define SC_U_LOGIN_YES					'8'						//								>s8
#define SC_U_LOGIN_NO					'9'						//								>s9

#define SC_G_ALLOW_YES					'a'						//  邀请/同意成功(对方群数量没达到MAX_GROUP_SIZE)		>sa
#define SC_G_ALLOW_NO					'b'						//  													>sb

#define SC_F_ALLOW_YES					'c'						//	添加好友成功(对方群数量没达到MAX_FRIEND_SIZE)		>sc
#define SC_F_ALLOW_NO					'd'						//														>sd

#define SC_G_INVITE_YES					'e'						//	>se
#define SC_G_INVITE_NO					'f'						//	>sf

#define SC_U_SETPASS_YES				'g'
#define SC_U_SETPASS_NO					'h'

#define SC_S_EMPTY                      'Z'                     //缓存区空                      >sZ                
//RecvSig
//S -> Client_Recv



#define SC_F_BEALLOW					'A'						//被同意加好友					>sAuuu					[myid]
#define SC_F_BEREJECT					'B'						//被拒绝添加好友                >sBuuu					[myid]
#define SC_F_BEADD						'C'						//被加							>sCuuu(8n)				[myid][Sname]		//Sname : myname
#define SC_F_BEDEL						'D'						//被删							>sDuuu					[myid]
#define SC_F_RENAME						'E'						//对方改名						>sEuuu(8n)				[myid][Sname]
#define SC_F_DELETE						'F'						//对方注销						>sFuuu					[myid]

#define SC_G_RENAME						'G'						//群名改变						>sGUUUgg(8n)			[gid][Sname]
#define SC_G_LEADER						'H'						//群leader改变					>sHuuugg				[myid][gid]
#define SC_G_BEALLOW					'I'						//被同意进群					>sIuuuggxxuuu...(8n)	[myid][gid][memberID]
#define SC_G_BEREMOVE					'J'						//被t							>sJUUUgg				[gid]
#define SC_G_BEINVITE					'K'						//被邀请进群					>sKuuuggxxuuu...(8n)	[myid][gid][memberID]
#define SC_G_BEREJECT					'L'						//被拒绝进群					>sLUUUgg				[gid]
#define SC_G_BEINSERT					'M'						//有新申请入群(只发给Leader)	>sMuuugg				[youid][gid]
#define SC_G_MEMBERQUIT					'N'						//群成员离开					>sNuuugg				[youid][gid]

#define SC_G_NEWMEMBER                  'O'                     //新成员进群                 	>sOuuugg				[youid][gid]


#endif

/*

CS_F_ALLOW
    SC_F_ALLOW_YES
        SC_F_BEALLOW
    SC_F_ALLOW_NO
CS_F_REJECT
    SC_F_BEREJECT
CS_F_ADD
    SC_F_BEADD
CS_F_DEL
    SC_F_BEDEL

CS_FIND_ID
    SC_FIND_ID_YES[is_vaild]
    SC_FIND_ID_NO[!is_vaild]

CS_G_RENAME
    SC_G_RENAME
CS_G_SETLEADER
    SC_G_LEADER
CS_G_ALLOW
    SC_G_ALLOW_YES
        SC_G_BEALLOW
    SC_G_ALLOW_NO
CS_G_REMOVE
    SC_G_BEREMOVE
CS_G_INVITE
    SC_G_INVITE_YES
        SC_G_BEINVITE
    SC_G_INVITE_NO
CS_G_REJECT
    SC_G_BEREJECT
CS_G_CREATE
    SC_G_CREATE_YES
    SC_G_CREATE_NO
CS_G_INSERT
    SC_G_BEINSERT	[send to leader]
CS_G_DELETE
CS_G_QUIT
    SC_G_MEMBERQUIT

CS_FIND_GID
    SC_FIND_ID_YES
    SC_FIND_ID_NO

CS_U_DELETE
    SC_F_DELETE
CS_U_RENAME
    SC_F_RENAME
CS_U_PASS
    SC_U_SETPASS_YES
    SC_U_SETPASS_NO
CS_U_CREATE
    SC_U_CREATE_YES
    SC_U_CREATE_NO
CS_U_LOGIN
    SC_U_LOGIN_YES
    SC_U_LOGIN_NO

CS_S_OK

*/


