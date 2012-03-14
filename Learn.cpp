#include "Learn.hpp"

namespace zabbix
{

Learn::Learn( Client* _j, ConfigParser* _parser, mongo::DBClientConnection* _c )
{
    j = _j;
    c = _c;
    parser = _parser;
}

int Learn::learnCommand( const Message& command )
{
    if( getAppendix( command.body() ).empty() )
    {
        /*
            send learn help
        */
        return -1;
    }
    std::string commandType    = getFirst( getAppendix( command.body() ) );
    std::string commandName    = getFirst( getAppendix( getAppendix( command.body() ) ) );
    std::string commandCommand = getAppendix( getAppendix( getAppendix( command.body() ) ) );

    mongo::BSONObjBuilder b;
    b.genOID();
    b.append( "command", commandCommand.c_str() );
    b.append( "name", commandName.c_str() );
    b.append( "type", commandType.c_str() );
    mongo::BSONObj p = b.obj();
    c->insert( "zabbix.commands", p );

    std::string info;
    info.append( "\nI learned the following:\nCommand-Name: " );
    info.append( commandName.c_str() );
    info.append( "\nCommand-Type: " );
    info.append( commandType.c_str() );
    info.append( "\nCommand actually executed: " );
    info.append( commandCommand );

    Message::MessageType type;
    Message msg( type,  command.from(), info );
    j->send( msg );
    return 0;
}

std::string Learn::getFirst( std::string command )
{
    size_t found = command.find_first_of( " " );
    if ( found < command.npos )
    {
        return command.substr( 0, found );
    }
    return command;
}

std::string Learn::getAppendix( std::string appendix )
{
    size_t found = appendix.find_first_of( " " );
    if ( found < appendix.npos )
    {
        return appendix.substr( found + 1, appendix.npos );
    }
    return "";
}

}
