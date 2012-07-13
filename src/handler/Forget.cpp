#include "Forget.hpp"

namespace zabbix
{

Forget::Forget( Client* _j, Config* _config, mongo::DBClientConnection* _c )
    : Commands( _j, _config, _c )
{
}

void Forget::forgetCommand( const Message& command )
{
    bool deleted = false;
    std::string answer;

    mongo::auto_ptr<mongo::DBClientCursor> cursor = c->query( config->commandsColl, mongo::BSONObj() );

    while( cursor->more() )
    {
        mongo::BSONObj currentCommand = cursor->next();
        std::string todel = getAppendix( command.body() );

        if( todel.compare( 0, todel.length(), currentCommand.getStringField( "name" ) ) == 0 )
        {
            c->remove( config->commandsColl, QUERY( "name" << currentCommand.getStringField( "name" ) ), true );
            deleted = true;
            answer.append( "I deleted " );
            answer.append( currentCommand.getStringField( "name" ) );
            answer.append( " from the database.");
            syslog( LOG_INFO, "%s deleted command %s", command.from().username().c_str(), currentCommand.getStringField( "name" ) );
            break;
        }
    }

    if( !deleted )
    {
        answer = "I did not found an entry with this name.";
    }

    Message::MessageType type;
    Message msg( type,  command.from(), answer );
    j->send( msg );
}

}
