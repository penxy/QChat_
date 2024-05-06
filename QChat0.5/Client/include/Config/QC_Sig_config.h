

#ifndef __QC_SIG_CONFIG_H
#define __QC_SIG_CONFIG_H



/* QC之间 id 都是数字 */
#define QC_FALLOW						'0'						//  同意申请					youid name
#define QC_FREJECT						'1'						//  拒绝申请					youid name
#define QC_FADD							'2'						//  申请添加好友			    youid
#define QC_FDEL							'3'						//	删除好友					youid

#define QC_USEARCHUSER					'4'                     //  搜索ID						youid

#define QC_GRENAME						'7'                     //  重命名群					gid name
#define QC_GSETLEADER					'8'                     //  设置Leader              gid youid
#define QC_GALLOWMEMBER					'9'                     //  同意入群					gid youid
#define QC_GDELMAMBER					'a'                     //  删除成员					gid youid
#define QC_GINVITEMEMBER				'b'                     //  邀请入群					gid youid
#define QC_GREJECTMEMBER				'c'				 		//	拒绝入群申请				gid youid
#define QC_GCREATEGROUP					'd'                     //  创建群						name
#define QC_GREQUESTINSERTGROUP			'e'                     //  申请入群					gid
#define QC_GDELETE						'm'						//	解散群					    gid
#define QC_GQUITGROUP					'f'                     //  离开群						gid

#define QC_GSEARCHGROUP					'g'                     //  搜索GID					    gid

#define QC_USERDELETE					'h'                     //  注销账号
#define QC_USERRENAME					'i'                     //  个人重命名					name
#define QC_USERCHANGEPASSWORD			'j'                     //  改密码						password newpassword
#define QC_USERCREATE					'k'                     //  注册						name password
#define QC_USERLOGIN					'l'                     //  登录						myid password



#define QC_FMESSAGECLEAR                'E'                     //  清空好友消息                 youid
#define QC_GMESSAGECLEAR                'n'                     //  清空群消息                   gid
#define QC_FSENDMESSAGE                 'o'                     //  发给好友消息                 youid mess
#define QC_GSENDMESSAGE                 'p'                     //  发给群消息                   gid mess
#define QC_REMOVENOTICE					'q'						//  删除Notice                   mess


#define QC_FRIENDLIST					'r'						// id name
#define QC_FRIENDMESS					's'						// id - Message

#define QC_GROUPLIST                    't'						//  gid name
#define QC_GROUPMEMBER                  'u'						//	id name
#define QC_GROUPMESS                    'v'						//	gid - Message

#define QC_MYADD						'w'						//  newFriendid - name
#define QC_BEADD						'x'						//	BenewFriendid - name
#define QC_GMYADD						'y'						//  newGroupgid - name

#define QC_NOTICE						'z'						//	ServerMessage

#define QC_MYGROUP						'A'						// gid name
#define QC_MYGROUP_BEINSERT             'B'                     // beinsert_id
#define QC_MYGROUP_MEMBER               'C'                     // id

#define QC_USERLOGOUT					'D'						//  退出登录

/* Client给Qt的消息 */

#define CQ_CREATEUSERYES				'0'						//注册成功					myid
#define CQ_CREATEUSERNO					'1'						//注册失败
#define CQ_LOGINYES						'2'						//登录成功
#define CQ_LOGINNO						'3'						//登录失败
#define CQ_CREATEGROUPYES				'4'						//建群成功					gid
#define CQ_CREATEGROUPNO				'5'						//建群失败
#define CQ_SETPASSWORDYES				'6'						//设置密码成功
#define CQ_SETPASSWORDNO				'7'						//设置密码失败
#define CQ_SEARCHIDYES					'8'						//ID查找成功				name
#define CQ_SEARCHIDNO					'9'						//ID查找失败
#define CQ_SEARCHGIDYES					'a'						//GID YES				    name
#define CQ_SEARCHGIDNO					'b'						//GID NO


#define CQ_FRIENDLSIT					'c'						// id name
#define CQ_FRIENDMESS					'd'						// id - Message

#define CQ_GROUPLIST                    'e'						//  gid name
#define CQ_GROUPMEMBER                  'f'						//	id name
#define CQ_GROUPMESS                    'g'						//	gid - Message

#define CQ_MYADD						'h'						//  newFriendid - name
#define CQ_BEADD						'i'						//	BenewFriendid - name
#define CQ_GMYADD						'j'						//  newGroupgid - name

#define CQ_NOTICE						'k'						//	ServerMessage

#define CQ_MYGROUP						'l'						// gid name
#define CQ_MYGROUP_BEINSERT             'm'                     // beinsert_id
#define CQ_MYGROUP_MEMBER               'n'                     // id



#endif

/*
#ifndef SERVEROK
    #define SERVEROK 1
#endif
*/



