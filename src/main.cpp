#include "Observer.hpp"
#include "Config.hpp"
#include "Config.hpp"

using namespace zabbix;

int main ( int argc, char* argv[] )
{
    Config * config;
    config = new Config();
    config->load( "config/bot.lua" );

    Observer* observer;
    observer = new Observer( config );
    observer->run();

    delete observer;
    delete config;

    return 0;
}
