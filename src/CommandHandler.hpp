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

#include "Config.hpp"
#include "Show.hpp"
#include "Execute.hpp"
#include "Learn.hpp"
#include "Help.hpp"
#include "Report.hpp"
#include "Forget.hpp"
#include "Janitor.hpp"

#define COMMANDCOUNT 6

using namespace gloox;

namespace zabbix
{

class CommandHandler
{
public:
    CommandHandler( Client* _j, Config* _parser );

    ~CommandHandler();

    void checkAuth( const Message& command );

    void tidyUp();

private:
    void connectMongo();

    int searchCommand( std::string A[], int size, std::string target );

    void validateCommand( const Message& command );

    void initCommandArr();

    bool auth( const Message& command );

    std::string getFirst( std::string command );

    std::string getAppendix( std::string appendix );

    mongo::DBClientConnection *c;

    Client *j;

    Show *show;

    Execute *execute;

    Learn *learn;

    Report *report;

    Help *help;

    Forget *forget;

    Janitor *janitor;

    Config *parser;

    std::string commands[COMMANDCOUNT];
};

}

#endif
