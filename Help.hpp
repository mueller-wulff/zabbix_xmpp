#ifndef ZABBIX_HELP_HPP
#define ZABBIX_HELP_HPP

#include <gloox/client.h>
#include <gloox/loghandler.h>
#include <gloox/messagehandler.h>
#include <gloox/connectionlistener.h>
#include <gloox/message.h>

#include "client/dbclient.h"
#include <iostream>
#include <string>

#include "ConfigParser.hpp"

#define COMMANDCOUNT 5

using namespace gloox;

namespace zabbix
{

class Help
{

public:
    Help( Client* _j, ConfigParser* _parser, mongo::DBClientConnection* _c );

    void helpCommand( const Message& command );

    void sendHelp( const Message& command );

private:
    int searchCommand( std::string A[], int size, std::string target );

    std::string commands[COMMANDCOUNT];

    void initCommandArr();

    std::string createHelpStr();

    std::string getFirst( std::string command );

    std::string getAppendix( std::string appendix );

    Client* j;

    ConfigParser* parser;

    mongo::DBClientConnection* c;
};

}

#endif
