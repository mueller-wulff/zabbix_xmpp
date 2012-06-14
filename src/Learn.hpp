#ifndef ZABBIX_LEARN_HPP
#define ZABBIX_LEARN_HPP

#include "Commands.hpp"

using namespace gloox;

namespace zabbix
{

class Learn : public Commands
{

public:
    Learn( Client* _j, Config* _parser, mongo::DBClientConnection* _c );

    int learnCommand( const Message& command );

private:

};

}

#endif
