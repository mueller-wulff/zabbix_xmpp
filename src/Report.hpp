#ifndef ZABBIX_REPORT_HPP
#define ZABBIX_REPORT_HPP

#include "Commands.hpp"

using namespace gloox;

namespace zabbix
{

class Report : public Commands
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

};

}

#endif

