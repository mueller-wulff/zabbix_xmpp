#include "Bot.hpp"
#include "ConfigParser.hpp"
using namespace zabbix;

int main ( int argc, char* argv[] )
{
    ConfigParser * parser;
    parser = new ConfigParser;

    parser->parse( argc, argv );
    parser->log();
    Bot *b;
    b = new Bot( parser );
    return 0;
}
