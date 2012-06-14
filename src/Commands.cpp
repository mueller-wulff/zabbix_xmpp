#include "Commands.hpp"

namespace zabbix
{

Commands::Commands( Client* _j, ConfigParser* _parser, mongo::DBClientConnection* _c )
{
    j = _j;
    c = _c;
    parser = _parser;

    openlog( "zabbix", LOG_PID, LOG_USER );
}


std::string Commands::getFirst( std::string command )
{
    size_t found = command.find_first_of( " " );
    if ( found < command.npos )
    {
        return command.substr( 0, found );
    }
    return command;
}

std::string Commands::getAppendix( std::string appendix )
{
    size_t found = appendix.find_first_of( " " );
    if ( found < appendix.npos )
    {
        return appendix.substr( found + 1, appendix.npos );
    }
    return "";
}

}
