#include "SemServer.h"
#include "SemQt.h"
#include "QtSig.h"
#include "Client.h"
#include "Log.h"


static constexpr char* QtPath = "QT/Test";
static constexpr char* log_path_qt = "Log/Log_Qt.txt";
static constexpr char* log_path_server = "Log/Log_Server.txt";
static constexpr char* log_path_client = "Log/Log_Client.txt";
static constexpr char* log_path_user = "Log/Log_User.txt";

Log log_qt = Log(log_path_qt);
Log log_server = Log(log_path_server);
Log log_client = Log(log_path_client);
Log log_user = Log(log_path_user);


SemServer sem_server = SemServer();
SemQt sem_qt = SemQt();

QtSig *user;
pid_t QtId;


void Init(){
    Client* client = new Client();
    client->Run();
    delete client;
}

int main(int argc, char* argv[]){
    user = new QtSig();
    Init();
    //delete user;
    return 0;
}

//why sem_server and sem_qt 创建得比Client慢