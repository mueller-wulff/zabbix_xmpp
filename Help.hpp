#ifndef ZABBIX_HELP_HPP
#define ZABBIX_HELP_HPP

#include "Commands.hpp"

#define COMMANDCOUNT 5

using namespace gloox;

namespace zabbix
{

class Help : public Commands
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

};

}

#endif
