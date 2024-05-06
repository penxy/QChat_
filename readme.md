### QChat聊天室

#### 主要功能

- **支持离线接收消息**
- **支持私聊**
- **添 / 删好友**
- **以及其它可扩展功能，详见0.5版本**

#### 开发环境

- #### **qtcreator**

- #### **VScode**

- #### **Ubuntu22.04**

#### 所用技术

- **C/C++, Python, Shell**
- **Qt**

- **Linux socket编程**
- **Linux 多线程，多进程及其通信(管道，信号, 共享内存)**
- **gdb、makefile、json**
- **SQLite**

   #### 项目结构

```
.
├── Client
│   ├── Base
│   │   ├── Log.cpp
│   │   ├── SemQt.cpp
│   │   ├── SemServer.cpp
│   │   ├── tool.cpp
│   │   └── ToStop.cpp
│   ├── Clear.sh
│   ├── include
│   │   ├── Base
│   │   │   ├── Log.h
│   │   │   ├── SemQt.h
│   │   │   ├── SemServer.h
│   │   │   ├── tool.h
│   │   │   └── ToStop.h
│   │   ├── Config
│   │   │   ├── config.h
│   │   │   ├── CS_Sig_config.h
│   │   │   └── QC_Sig_config.h
│   │   ├── User
│   │   │   ├── FriendLogic.h
│   │   │   ├── User.h
│   │   │   ├── UserInfo.h
│   │   │   ├── UserLogic.h
│   │   │   ├── UserSQL.h
│   │   │   ├── WorkQt.h
│   │   │   └── WorkServer.h
│   │   └── Work
│   │       ├── Client.h
│   │       ├── WorkQtRecv.h
│   │       ├── WorkQtSend.h
│   │       ├── WorkServerRecv.h
│   │       └── WorkServerSend.h
│   ├── Log
│   │   └── log.txt
│   ├── makefile
│   ├── Qt
│   │   └── Test
│   ├── sem
│   │   ├── sem_cq_recv
│   │   ├── sem_cq_send
│   │   ├── sem_cs_recv
│   │   ├── sem_cs_send
│   │   └── sem_set
│   ├── Shell
│   │   ├── StrOp.py
│   │   ├── StrOp.sh
│   │   └── up.sh
│   ├── show
│   ├── SQL
│   │   ├── ClearCSV.sh
│   │   ├── ClearDB.sh
│   │   ├── ClearDB.sql
│   │   ├── CreateTable.py
│   │   ├── CreateTable.sh
│   │   ├── Data
│   │   │   ├── FriendMessageTable.csv
│   │   │   ├── FriendTable.csv
│   │   │   ├── MyAddTable.csv
│   │   │   ├── MyBeAddTable.csv
│   │   │   ├── UserInfo.csv
│   │   │   └── UserInfoTable.csv
│   │   ├── DataBases
│   │   ├── read.py
│   │   ├── read.sh
│   │   ├── show.sh
│   │   ├── show.sql
│   │   ├── write.py
│   │   └── write.sh
│   ├── src
│   │   ├── Client.cpp
│   │   └── main.cpp
│   ├── s.sh
│   ├── User
│   │   ├── FriendLogic.cpp
│   │   ├── UserInfo.cpp
│   │   ├── UserLogic.cpp
│   │   ├── UserSQL.cpp
│   │   ├── WorkQt.cpp
│   │   └── WorkServer.cpp
│   └── Work
│       ├── WorkQtRecv.cpp
│       ├── WorkQtSend.cpp
│       ├── WorkServerRecv.cpp
│       └── WorkServerSend.cpp
├── Qt
│   ├── Config
│   │   ├── CQSigconfig.h
│   │   └── LimitConfig.h
│   ├── confirm.ui
│   ├── Dialog
│   │   ├── confirm.h
│   │   ├── f_add.h
│   │   ├── f_beadd.h
│   │   ├── f_findid.h
│   │   ├── f_talk.h
│   │   └── login.h
│   ├── DiaLogCpp
│   │   ├── confirm.cpp
│   │   ├── f_add.cpp
│   │   ├── f_beadd.cpp
│   │   ├── f_findid.cpp
│   │   ├── f_talk.cpp
│   │   └── login.cpp
│   ├── f_add.ui
│   ├── f_beadd.ui
│   ├── f_findid.ui
│   ├── f_talk.ui
│   ├── Log.cpp
│   ├── Log.h
│   ├── login.ui
│   ├── main.cpp
│   ├── Test.pro
│   ├── Test.pro.user
│   ├── tool.cpp
│   ├── tool.h
│   ├── User.cpp
│   ├── User.h
│   ├── widget.cpp
│   ├── widget.h
│   └── widget.ui
├── readme.md
└── Server
    ├── Base
    │   ├── buffer.cpp
    │   ├── log.cpp
    │   └── tool.cpp
    ├── include
    │   ├── Base
    │   │   ├── buffer.h
    │   │   ├── log.h
    │   │   └── tool.h
    │   ├── Config
    │   │   ├── config.h
    │   │   └── CS_Sig_config.h
    │   ├── src
    │   │   └── Server.h
    │   ├── User
    │   │   ├── FriendLogic.h
    │   │   ├── User.h
    │   │   ├── UserInfo.h
    │   │   └── UserLogic.h
    │   └── Work
    │       ├── ClientInfo.h
    │       ├── ClientLogic.h
    │       ├── ClientRecv.h
    │       ├── ClientSend.h
    │       ├── pool.h
    │       ├── readme.md
    │       └── SQL.h
    ├── Log
    │   ├── Log_Server.txt
    │   └── log.txt
    ├── makefile
    ├── src
    │   ├── main.cpp
    │   └── Server.cpp
    ├── s.sh
    ├── this.txt
    ├── User
    │   ├── FriendLogic.cpp
    │   ├── UserInfo.cpp
    │   └── UserLogic.cpp
    └── Work
        ├── ClientInfo.cpp
        ├── ClientLogic.cpp
        ├── ClientRecv.cpp
        ├── ClientSend.cpp
        ├── pool.cpp
        └── SQL.cpp

33 directories, 137 files





github.com/AlDanial/cloc v 1.90  T=0.04 s (3058.9 files/s, 161966.5 lines/s)
--------------------------------------------------------------------------------
Language                      files          blank        comment           code
--------------------------------------------------------------------------------
C++                              41            488            237           2299
C/C++ Header                     47            453            122           1117
Qt                                7              0              0            893
XML                               1              0              1            262
Python                            4             21              1             75
Bourne Shell                     10             64             10             64
ProGuard                          1              9              4             42
make                              2             18              0             40
SQL                               2              3              0             14
Bourne Again Shell                1              1              0              5
Markdown                          2              4              0              1
--------------------------------------------------------------------------------
SUM:                            118           1061            375           4812
--------------------------------------------------------------------------------
```

#### 各主要文件说明

```
Client[后端]	
	Work/WorkQtRecv			 接收前端管道数据
	Work/WorkQtSend			 发数据给前端管道
	Work/WorkServerRecv		 接收服务端数据(通过SIGIO信号执行中断接收)
	Work/WorkServerSend		 发数据给服务端
	
	Base/Log 				日志
	Base/SemServer 			包含一个信号量(用于WorkQtRecv唤醒WorkServerSend[表示数据准备好了, 可以发送]), 和 收/发 共享内存(存放数据)
	Base/SemQt     			包含一个信号量(用于WorkServerRecv唤醒WorkQtSend[表示数据准备好了, 可以发送]), 和 收/发 共享内存(存放数据)
		
	Shell/StrOp.*			字符串脚本操作
	SQL/*					一些脚本, 用于实现SQLite的一些操作, 以保存个人信息
	
	src/Client				创建4个线程, 分别是Work/*, 同时连接服务端
Qt[前端]
	Dialog/*				一些 QDialog 窗口
     User					一个类, 保留主要信息    
Server[服务端]
	 Base/buffer			实现离线收消息, 当一个用户发送给的目标不在线时, 就存在这个缓冲区中
	 					   当一个用户登录, 若缓冲区非空, 则通过二分查找到最合适的数据包个数, 一次发送给用户, 用户确认收到信号后, 继续执行上一步骤
	 Work/pool				线程池, 存在6个线程, 和一个消息队列, 当队列非空, 通过锁唤醒一个线程前往执行, 执行后进入休眠状态
```

### 遇到的困难

```
1.销毁 线程池 时, 我发现总是不能等到 线程池 的 线程 全部退出
解决：开始销毁 线程池 时, 有些线程甚至还没创建完
2.如何让 服务端 发送所有的缓冲区的数据包给用户
解决: 登录时开始发, 每次收到确认后继续发, 直到缓冲区空
3.Client如何知道 服务端 有消息发来
解决: 通过SIGIO信号
4.socket()返回-1
解决: 通过perror();发现是打开的文件过多, 再调用lsof -p <pid>发现打开了一堆的日志文件, 发现是User[], 同时User还继承自Log;
```

### 个人觉得的亮点

```
Base/buffer						 动态调整缓冲区大小, 节省空间
								维护一个前缀和数组, 选择发送的数据包个数时, 可以二分查找该数组, 有着不错的效率
								
								
```

### 心得

```
这个项目完全自己写的, 花了我一个多月的时间, 现在看来, 从头来写估计只需要一周, 也算是一个进步吧
第一次写相对大型的项目, 踩过不少坑
比如一开始定了很多功能, 一度写到7000行(可以参考0.5版本, 不过它甚至不能运行, 只是功能逻辑写好了)
后来发现太乱了, 删掉并重构后, 才有了目前的雏形, 同时也导致了功能较少
功能逻辑写好了, 再往里头添加功能并不难, 但是我该去学其他东西了, 也许以后有机会再做更新
																													2024.05.06
```

​	

