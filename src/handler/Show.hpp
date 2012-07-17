#ifndef ZABBIX_SHOW_HPP
#define ZABBIX_SHOW_HPP

#include "Commands.hpp"

using namespace gloox;

namespace zabbix
{

class Show : public Commands
{

public:
    Show( Client* _j, Config* _config, mongo::DBClientConnection* _c );

    void showCommands( const Message& command );

private:

};

}

#endif
