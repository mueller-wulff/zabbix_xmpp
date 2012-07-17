#ifndef ZABBIX_SERVER_HPP
#define ZABBIX_SERVER_HPP

#include <gloox/client.h>
#include <gloox/loghandler.h>
#include <gloox/messagehandler.h>
#include <gloox/connectionlistener.h>
#include <gloox/message.h>

#include "client/dbclient.h"
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <errno.h>
#include <sstream>


#include "client/dbclient.h"

#include "Config.hpp"
#include "Bot.hpp"


using namespace gloox;

namespace zabbix
{

class Server
{
public:
    Server( Config* _config );

    void run();

private:
    Config* config;
    mongo::DBClientConnection *c;
    pid_t pid;
    int status;
    SSL_CTX* ctx;
    char *cert;
    char *key;
    BIO *abio;
    BIO *client;
    SSL *ssl;
    char *host;

    void handleClient();
    std::string createAnswer401();
    std::string createAnswer200();
    bool configeq( std::string req );
    std::string decodeDigest( std::string digest );
    std::string getReports();
    void readError( int r );
};

}

#endif
