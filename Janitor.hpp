#ifndef ZABBIX_JANITOR_HPP
#define ZABBIX_JANITOR_HPP

#include <gloox/client.h>
#include <gloox/loghandler.h>
#include <gloox/messagehandler.h>
#include <gloox/connectionlistener.h>
#include <gloox/message.h>

#include "client/dbclient.h"
#include <iostream>
#include <string>

#include "ConfigParser.hpp"

using namespace gloox;

namespace zabbix
{

class Janitor
{

public:
    Janitor( Client* _j, ConfigParser* _parser, mongo::DBClientConnection* _c );

    void tidyUp();

private:
    void sendOK( std::string problem );

    Client* j;

    ConfigParser* parser;

    mongo::DBClientConnection* c;
};

}

#endif
