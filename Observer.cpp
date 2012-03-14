#include "Observer.hpp"

namespace zabbix
{
Observer::Observer( ConfigParser* _parser )
{
    parser = _parser;
}

void Observer::run()
{
    if ( ( pid = fork() ) == 0 )
    {
        Bot *b;
        b = new Bot( parser );
    }
    else
    {
        observe();
    }
}

void Observer::observe()
{
    while ( 1 )
    {
        pid_t test = waitpid(pid, &status, WNOHANG);

        sleep( 1 );

        if ( test == -1 )
        {
            break;
        }
    }
}

}
