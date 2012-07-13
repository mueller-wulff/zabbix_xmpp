#ifndef ZABBIX_MASTER_HPP
#define ZABBIX_MASTER_HPP

#include "Bot.hpp"
#include "Config.hpp"
#include "Server.hpp"
#include <sys/types.h>
#include <signal.h>


namespace zabbix
{

class Master
{
public:
    Master( Config *_config );
    ~Master();

    Config *config;

    bool keepRunning;

    pid_t pidJabber;
    int statusJabber;

    pid_t pidServer;
    int statusServer;

    void run();
    void dropRights();
    void createJabber();
    void createServer();

};

}
#endif
