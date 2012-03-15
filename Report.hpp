#ifndef ZABBIX_REPORT_HPP
#define ZABBIX_REPORT_HPP

#include <gloox/client.h>
#include <gloox/loghandler.h>
#include <gloox/messagehandler.h>
#include <gloox/connectionlistener.h>
#include <gloox/message.h>

#include "client/dbclient.h"
#include <iostream>
#include <string>

#include "ConfigParser.hpp"

using namespace gloox;

namespace zabbix
{

class Report
{

public:
    Report( Client* _j, ConfigParser* _parser, mongo::DBClientConnection* _c );

    void reportIssue( const Message& command );

private:
    void sendReport( std::string report );

    bool checkReport( std::string report );

    bool analyzeReport( std::string report, std::string status );

    int getTimestamp();

    bool storeReport( std::string report, std::string status );

    Client* j;

    ConfigParser* parser;

    mongo::DBClientConnection* c;
};

}

#endif

