#ifndef ZABBIX_EXECUTE_HPP
#define ZABBIX_EXECUTE_HPP

#include "Commands.hpp"

using namespace gloox;
namespace zabbix
{
class Execute : public Commands
{
public:
    Execute( Client* _j, ConfigParser* _parser, mongo::DBClientConnection* _c );

    int validateExecute( const Message& command );

private:
    std::string executeScript( std::string script );

    std::string executeShell( std::string shell );

};

}

#endif
