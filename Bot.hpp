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
#include "ConfigParser.hpp"

using namespace gloox;

namespace zabbix
{

class Bot : public MessageHandler, LogHandler, ConnectionListener
{
public:
   Bot( ConfigParser* _parser );

   void connectToXMPP( ConfigParser* _parser );

   void initCommandHandler( Client *j, ConfigParser* _parser );

   virtual void onConnect();

   virtual void onDisconnect( ConnectionError e );

   virtual bool onTLSConnect( const CertInfo& info );

   virtual void handleMessage( const Message& stanza, MessageSession* session = 0 );

   virtual void handleLog( LogLevel level, LogArea area, const std::string& message );

private:
   Client* j;
   CommandHandler* comm;
   ConfigParser* parser;
};

}
#endif
