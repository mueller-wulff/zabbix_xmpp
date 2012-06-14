#include "Execute.hpp"

namespace zabbix
{
Execute::Execute( Client* _j, Config* _config, mongo::DBClientConnection* _c )
    : Commands( _j, _config, _c )
{

}

int Execute::validateExecute( const Message& command )
{
    bool executed = false;
    std::string appendix = getAppendix( command.body() );
    if( appendix.empty() )
    {
        /*
            send execute help
        */
        return -1;
    }

    mongo::auto_ptr<mongo::DBClientCursor> cursor = c->query( config->commandsColl, mongo::BSONObj() );
    while( cursor->more() )
    {
        mongo::BSONObj p = cursor->next();
        if ( appendix.compare( 0, appendix.length(), p.getStringField( "name" ) ) == 0 )
        {
            Message::MessageType type;
            std::string commandType = p.getStringField("type");
            if( commandType.compare( 0, commandType.length(), "script" ) == 0 )
            {
                Message msg( type,  command.from(), executeScript( p.getStringField( ( "command" ) ) ) );
                j->send( msg );
                executed = true;
            }
            if( commandType.compare( 0, commandType.length(), "shell" ) == 0 )
            {
                Message msg( type,  command.from(), executeShell( p.getStringField( ( "command" ) ) ) );
                j->send( msg );
                executed = true;
                syslog( LOG_INFO, "%s executed command %s ( %s )", command.from().username().c_str(), p.getStringField( "name" ), p.getStringField( "name" ) );
            }
            break;
        }
    }
    if( !executed )
    {
        Message::MessageType type;
        Message msg( type,  command.from(), "Dont know what to execute!" );
        j->send( msg );
    }
    return 0;
}

std::string Execute::executeScript( std::string script )
{
    std::string myScriptFolder = config->scriptDir;
    myScriptFolder.append( script.c_str() );
    FILE* pipe = popen( myScriptFolder.c_str(), "r" );
    if ( !pipe ) return "ERROR";
    char buffer[128];
    std::string result = "";
    while( !feof( pipe ) )
    {
        if( fgets( buffer, 128, pipe ) != NULL )
            result += buffer;
    }
    pclose( pipe );
    return result;
}

std::string Execute::executeShell( std::string shell )
{
    FILE* pipe = popen( shell.c_str(), "r" );
    if ( !pipe ) return "ERROR";
    char buffer[128];
    std::string result = "\n";
    while( !feof( pipe ) )
    {
        if( fgets( buffer, 128, pipe ) != NULL )
            result += buffer;
    }
    pclose( pipe );
    return result;
}

}
