#ifndef ZABBIX_EXECUTE_HPP
#define ZABBIX_EXECUTE_HPP

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
class Execute
{
public:
    Execute( Client* _j, ConfigParser* _parser, mongo::DBClientConnection* _c );

    int validateExecute( const Message& command );

private:
    std::string executeScript( std::string script );

    std::string executeShell( std::string shell );

    std::string getFirst( std::string command );

    std::string getAppendix( std::string appendix );

    Client* j;

    ConfigParser* parser;

    mongo::DBClientConnection* c;
};

}

#endif
