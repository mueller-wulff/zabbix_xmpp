#ifndef ZABBIX_OBSERVER_HPP
#define ZABBIX_OBSERVER_HPP

#include <gloox/client.h>
#include <gloox/loghandler.h>
#include <gloox/messagehandler.h>
#include <gloox/connectionlistener.h>
#include <gloox/message.h>

#include "client/dbclient.h"
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "client/dbclient.h"

#include "ConfigParser.hpp"
#include "Bot.hpp"

#define BACKLOG 10

using namespace gloox;

namespace zabbix
{

class Observer
{
public:
    Observer( ConfigParser* _parser );

    void run();

private:
    ConfigParser* parser;
    mongo::DBClientConnection *c;
    pid_t pid;
    int status;
    int socketfd;
    int newfd;
    fd_set master;
    fd_set read_fds;
    int fdmax;
    socklen_t addrlen;
    struct sockaddr_storage remoteaddr;

    void observe();
    int create_and_bind( const char *port );
    void handleData();
    void processRequest( std::string request, int i );
    bool checkOrigin( std::string origin );
    std::string createAnswer();
    std::string getReports();
};

}

#endif
