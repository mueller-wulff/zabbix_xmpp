#ifndef ZABBIX_OBSERVER_HPP
#define ZABBIX_OBSERVER_HPP

#include <gloox/client.h>
#include <gloox/loghandler.h>
#include <gloox/messagehandler.h>
#include <gloox/connectionlistener.h>
#include <gloox/message.h>

#include "client/dbclient.h"
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "ConfigParser.hpp"
#include "Bot.hpp"

using namespace gloox;

namespace zabbix
{

class Observer
{
public:
    Observer( ConfigParser* _parser );

    void run();

private:
    ConfigParser* parser;

    pid_t pid;

    int status;

    void observe();
};

}

#endif
