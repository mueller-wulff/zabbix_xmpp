#ifndef ZABBIX_BOT_HPP
#define ZABBIX_BOT_HPP

#include <iostream>
#include <string.h>
#include <stdio.h>

#include <gloox/client.h>
#include <gloox/loghandler.h>
#include <gloox/messagehandler.h>
#include <gloox/connectionlistener.h>
#include <gloox/message.h>

#include "CommandHandler.hpp"
#include "Config.hpp"

using namespace gloox;

namespace zabbix
{

class Bot : public MessageHandler, LogHandler, ConnectionListener
{
public:
    Bot( Config* _config );

    ~Bot();

    void connectToXMPP( Config* _config );

    void tidyUp();

private:
   Client* j;
   CommandHandler* comm;
   Config* config;
   time_t starttime;
   int janTimeout;

   void initCommandHandler( Client *j, Config* _config );
   virtual void onConnect();
   virtual void onDisconnect( ConnectionError e );
   virtual bool onTLSConnect( const CertInfo& info );
   virtual void handleMessage( const Message& stanza, MessageSession* session = 0 );
   virtual void handleLog( LogLevel level, LogArea area, const std::string& message );
};

}
#endif
