#ifndef ZABBIX_COMMANDHANDLER_HPP
#define ZABBIX_COMMANDHANDLER_HPP

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

class CommandHandler
{
public:
    CommandHandler( Client* _j, ConfigParser* _parser );

    void checkAuth( const Message& command );

private:
    void connectMongo();

    int searchCommand( std::string A[], int size, std::string target );

    void validateCommand( const Message& command );

    void initCommandArr();

    void sendHelp( const Message& command );

    std::string createHelpStr();

    void showCommands( const Message& command );

    int validateExecute( const Message& command );

    std::string executeScript( std::string script );

    std::string executeShell( std::string shell );

    int learnCommand( const Message& command );

    void reportIssue( const Message& command );

    void helpCommand( const Message& command );

    bool auth( const Message& command );

    std::string getFirst( std::string command );

    std::string getAppendix( std::string appendix );

    mongo::DBClientConnection *c;

    Client *j;

    ConfigParser *parser;

    std::string commands[COMMANDCOUNT];
};

}

#endif
