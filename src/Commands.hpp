#ifndef ZABBIX_COMMANDS_HPP
#define ZABBIX_COMMANDS_HPP

#include <gloox/client.h>
#include <gloox/loghandler.h>
#include <gloox/messagehandler.h>
#include <gloox/connectionlistener.h>
#include <gloox/message.h>

#include "client/dbclient.h"
#include <iostream>
#include <string>
#include <syslog.h>

#include "ConfigParser.hpp"

using namespace gloox;

namespace zabbix
{

class Commands
{

public:
    Commands( Client* _j, ConfigParser* _parser, mongo::DBClientConnection* _c );

    Client* j;

    ConfigParser* parser;

    mongo::DBClientConnection* c;

    std::string getFirst( std::string command );

    std::string getAppendix( std::string appendix );
};

}

#endif
