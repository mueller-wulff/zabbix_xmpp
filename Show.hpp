#ifndef ZABBIX_SHOW_HPP
#define ZABBIX_SHOW_HPP

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

class Show
{

public:
    Show( Client* _j, ConfigParser* _parser, mongo::DBClientConnection* _c );

    void showCommands( const Message& command );

private:
    Client* j;

    ConfigParser* parser;

    mongo::DBClientConnection* c;
};

}

#endif
