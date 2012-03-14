#include "Observer.hpp"
#include "ConfigParser.hpp"
using namespace zabbix;

int main ( int argc, char* argv[] )
{
    ConfigParser * parser;
    parser = new ConfigParser;

    parser->parse( argc, argv );
    parser->log();

    Observer* observer;
    observer = new Observer( parser );
    observer->run();
    return 0;
}
