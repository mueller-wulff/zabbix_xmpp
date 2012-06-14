#include "Bot.hpp"

using namespace gloox;

namespace zabbix
{

Bot::Bot( ConfigParser* _parser )
{
    starttime = time( 0 );
    parser = _parser;
    janTimeout = parser->getjanTimeout();
    connectToXMPP( parser );
}

Bot::~Bot()
{
    delete comm;
}

void Bot::connectToXMPP( ConfigParser* parser )
{
    JID jid( parser->getJID() );

    j = new Client( jid, parser->getJIDPW() );

    initCommandHandler( j, parser );

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

void Bot::initCommandHandler( Client* j, ConfigParser* parser )
{
    comm = new CommandHandler( j, parser );
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
        comm->checkAuth( stanza );
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
