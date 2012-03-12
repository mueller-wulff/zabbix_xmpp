#include "Bot.hpp"

using namespace gloox;

namespace zabbix
{

Bot::Bot()
{
    connectToXMPP();
}

void Bot::connectToXMPP()
{
    JID jid( "zabbix@localhost/resource" );
    j = new Client( jid, "test123" );

    initCommandHandler( j );

    j->registerConnectionListener( this );
    j->registerMessageHandler( this );
    j->logInstance().registerLogHandler( LogLevelDebug, LogAreaAll, this );

    if( j->connect( false ) )
    {
        ConnectionError ce = ConnNoError;
        while( ce == ConnNoError )
        {
          ce = j->recv();
        }
    }
}

void Bot::initCommandHandler( Client* j )
{
    comm = new CommandHandler( j );
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
    if ( !stanza.body().empty() )
    {
        comm->validateCommand( stanza );
    }
}

void Bot::handleLog( LogLevel level, LogArea area, const std::string& message )
{
    //printf("log: level: %d, area: %d, %s\n", level, area, message.c_str() );
}


}
