#ifndef ZABBIX_COMMANDHANDLER_HPP
#define ZABBIX_COMMANDHANDLER_HPP

#include <gloox/client.h>
#include <gloox/loghandler.h>
#include <gloox/messagehandler.h>
#include <gloox/connectionlistener.h>
#include <gloox/message.h>

#include "client/dbclient.h"
#include <iostream>
#include <map>
#include <string>
#define COMMANDCOUNT 4

using namespace gloox;

namespace zabbix
{

class CommandHandler
{
public:
    CommandHandler( Client* _j );

    void connectMongo();

    int searchCommand( std::string A[], int size, std::string target );

    void validateCommand( const Message& command );

    void initCommandArr();

    void sendHelp( const Message& command );

    std::string createHelpStr();

    void showCommands( const Message& command );

    void validateExecute( const Message& command );

    std::string executeScript( std::string script );

    std::string executeShell( std::string shell );

    std::string getCommand( std::string command );

    std::string getAppendix( std::string appendix );


private:
    mongo::DBClientConnection *c;

    Client *j;

    std::string commands[COMMANDCOUNT];

};

}

#endif
