#ifndef ZABBIX_LEARN_HPP
#define ZABBIX_LEARN_HPP

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

class Learn
{

public:
    Learn( Client* _j, ConfigParser* _parser, mongo::DBClientConnection* _c );

    int learnCommand( const Message& command );

private:
    std::string getFirst( std::string command );

    std::string getAppendix( std::string appendix );

    Client* j;

    ConfigParser* parser;

    mongo::DBClientConnection* c;
};

}

#endif

