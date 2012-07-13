#include "Master.hpp"

namespace zabbix
{

Master::Master( Config *_config ) : keepRunning( true )
{
    config = _config;
}

Master::~Master()
{
    kill( pidJabber, SIGTERM );
    kill( pidServer, SIGTERM );
}

void Master::run()
{
    createJabber();
    createServer();
    while( keepRunning )
    {
        pid_t testJabber = waitpid(pidJabber, &statusJabber, WNOHANG);
        pid_t testServer = waitpid(pidServer, &statusServer, WNOHANG);
        if( testJabber == -1 )
        {
            createJabber();
        }
        if( testServer == -1 )
        {
            createServer();
        }
        sleep( 1 );
    }
}

void Master::dropRights()
{
    if (getuid() == 0) {
        if (setgid( config->uid ) != 0)
            printf("setgid: Unable to drop group privileges: %s", strerror(errno));
        if (setuid( config->uid ) != 0)
            printf("setuid: Unable to drop user privileges: %S", strerror(errno));
    }
}

void Master::createJabber()
{
    if ( ( pidJabber = fork() ) == 0 )
    {
        Bot *b;
        dropRights();
        b = new Bot( config );
        delete b;
        exit( 0 );
    }
}

void Master::createServer()
{
    if ( ( pidServer = fork() ) == 0 )
    {
        Server *server;
        server = new Server( config );
        server->run();
        delete server;
        exit( 0 );
    }
}

}
