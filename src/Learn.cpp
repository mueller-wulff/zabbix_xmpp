#include "Learn.hpp"

namespace zabbix
{

Learn::Learn( Client* _j, Config* _parser, mongo::DBClientConnection* _c )
    : Commands( _j, _parser, _c )
{
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
    c->insert( parser->commandsColl, p );

    std::string info;
    info.append( "\nI learned the following:\nCommand-Name: " );
    info.append( commandName.c_str() );
    info.append( "\nCommand-Type: " );
    info.append( commandType.c_str() );
    info.append( "\nCommand actually executed: " );
    info.append( commandCommand );

    syslog( LOG_INFO, "%s tought command %s", command.from().username().c_str(), commandName.c_str() );

    Message::MessageType type;
    Message msg( type,  command.from(), info );
    j->send( msg );
    return 0;
}


}
