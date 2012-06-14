#ifndef ZABBIX_FORGET_HPP
#define ZABBIX_FORGET_HPP

#include "Commands.hpp"

using namespace gloox;

namespace zabbix
{

class Forget : public Commands
{

public:
    Forget( Client* _j, Config* _parser, mongo::DBClientConnection* _c );

    void forgetCommand( const Message& command );

private:

};

}

#endif
