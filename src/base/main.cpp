#include "Config.hpp"
#include "Master.hpp"

using namespace zabbix;

Master *master;

void sigHandler( int signum )
{
    master->keepRunning = false;
}

int main ( int argc, char* argv[] )
{
    signal( SIGHUP, sigHandler );
    Config * config;
    config = new Config();
    config->load( "/etc/zabbix_bot/bot.lua" );


    master = new Master( config );
    master->run();

    delete master;
    delete config;
    return 0;
}
