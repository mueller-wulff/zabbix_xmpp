#ifndef ZABBIX_OBSERVER_HPP
#define ZABBIX_OBSERVER_HPP

#include <gloox/client.h>
#include <gloox/loghandler.h>
#include <gloox/messagehandler.h>
#include <gloox/connectionlistener.h>
#include <gloox/message.h>

#include "client/dbclient.h"
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <errno.h>
#include <sstream>


#include "client/dbclient.h"

#include "ConfigParser.hpp"
#include "Bot.hpp"


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
    bool parseReq( std::string req );
    std::string decodeDigest( std::string digest );
    void dropRights();
    void observe();
    std::string getReports();
    void readError( int r );
};

}

#endif
