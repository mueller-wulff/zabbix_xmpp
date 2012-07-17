#include "Show.hpp"

namespace zabbix
{
Show::Show( Client* _j, Config* _config, mongo::DBClientConnection* _c )
    : Commands( _j, _config, _c )
{

}

void Show::showCommands( const Message& command )
{
    std::string commandList;
    mongo::auto_ptr<mongo::DBClientCursor> cursor = c->query( config->commandsColl, mongo::BSONObj() );

    while( cursor->more() )
    {
        mongo::BSONObj p = cursor->next();
        commandList.append( "\n" );
        commandList.append( p.getStringField( "name" ) );
    }

    Message::MessageType type;
    Message msg( type,  command.from(), commandList.c_str() );
    j->send( msg );
}

}
