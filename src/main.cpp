#include "Config.hpp"
#include "Master.hpp"

using namespace zabbix;

int main ( int argc, char* argv[] )
{
    Config * config;
    config = new Config();
    config->load( "config/bot.lua" );

    Master *master;
    master = new Master( config );
    master->run();

    delete master;
    delete config;

    return 0;
}
