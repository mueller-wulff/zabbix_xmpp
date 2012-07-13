#include "Bot.hpp"

using namespace gloox;

namespace zabbix
{

Bot::Bot( Config* _config )
{
    starttime = time( 0 );
    config = _config;
    janTimeout = config->janTimeout;
    connectToXMPP( config );
}

Bot::~Bot()
{
    delete comm;
}

void Bot::connectToXMPP( Config* config )
{
    JID jid( config->jid );

    j = new Client( jid, config->jidpw );

    initCommandHandler( j, config );

    j->registerConnectionListener( this );
    j->registerMessageHandler( this );
    j->logInstance().registerLogHandler( LogLevelDebug, LogAreaAll, this );

    if( j->connect( false ) )
    {
        ConnectionError ce = ConnNoError;
        while( ce == ConnNoError )
        {
          ce = j->recv( janTimeout );
          time_t difftime = time( 0 ) - starttime;
          if( difftime > 60 )
            tidyUp();
        }
    }
}

void Bot::initCommandHandler( Client* j, Config* config )
{
    comm = new CommandHandler( j, config );
}

void Bot::onConnect()
{

}

void Bot::onDisconnect( ConnectionError e )
{

}

bool Bot::onTLSConnect( const CertInfo& info )
{
    return true;
}

void Bot::handleMessage( const Message& stanza, MessageSession* session )
{
    bool authed;
    if ( !stanza.body().empty() )
    {
        authed = comm->checkAuth( stanza );
    }
    if( authed )
    {
        comm->validateCommand( stanza );
    }
    else
    {
        Message::MessageType type;
        std::string helpStr = "You are are not allowed to do this!";
        Message msg( type,  stanza.from(), helpStr );
        j->send( msg );
    }
}

void Bot::handleLog( LogLevel level, LogArea area, const std::string& message )
{
    //printf("log: level: %d, area: %d, %s\n", level, area, message.c_str() );
}

void Bot::tidyUp()
{
    comm->tidyUp();
}

}
