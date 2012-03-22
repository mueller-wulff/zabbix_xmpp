#include "Observer.hpp"

namespace zabbix
{
Observer::Observer( ConfigParser* _parser )
{
    parser = _parser;
    socketfd = create_and_bind("80");

    c = new mongo::DBClientConnection( true,0 ,0 );
    c->connect( parser->getMongoHost() );

    if (listen(socketfd, BACKLOG) == -1)
    {
        abort();
    }
    FD_SET(socketfd, &master);
    fdmax = socketfd;
}

void Observer::run()
{
    while(1)
    {
        sleep(10);
        if ( ( pid = fork() ) == 0 )
        {
            Bot *b;
            b = new Bot( parser );
            exit(0);
        }
        else
        {
            observe();
        }
    }
}

void Observer::observe()
{

    while ( 1 )
    {
        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        pid_t cpidstat = waitpid(pid, &status, WNOHANG);

        if ( cpidstat == -1 )
        {
            break;
        }

        read_fds = master;

        if (select(fdmax+1, &read_fds, NULL, NULL, &timeout) == -1)
        {
            //abort();
        }
        else
        {
            handleData();
        }

    }
}

int Observer::create_and_bind( const char *port )
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int listener;
    int socketfd;

    memset( &hints, 0, sizeof( struct addrinfo) );

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    listener = getaddrinfo( NULL, port, &hints, &result );
    if ( listener != 0 )
    {
        return -1;
    }

    for ( rp = result; rp != NULL; rp = rp->ai_next )
    {
        socketfd = socket( rp->ai_family, rp->ai_socktype, rp->ai_protocol );
        if ( socketfd == -1 )
        {
            continue;
        }
        int on = 1;

        listener = bind(socketfd, rp->ai_addr, rp->ai_addrlen);

        if ( listener == 0 )
        {
            break;
        }
        close( socketfd );
    }

    if ( rp == NULL )
    {
        return -1;
    }

    freeaddrinfo( result );
    return socketfd;
}

void Observer::handleData()
{
    int i;
    int nbytes;
    char buf[4028];


    for( i = 0; i <= fdmax; i++ )
    {
        if ( FD_ISSET( i, &read_fds ) )
        {
            if ( i == socketfd )
            {
                addrlen = sizeof remoteaddr;
                newfd = accept( socketfd, ( struct sockaddr * )&remoteaddr, &addrlen );

                if ( newfd == -1 )
                {
                    perror( "accept" );
                }
                else
                {
                    FD_SET( newfd, &master );
                    if ( newfd > fdmax )
                    {
                        fdmax = newfd;
                    }
                }
            }
            else
            {
                // handle data from a client
                if ( ( nbytes = recv( i, buf, sizeof buf, 0 ) ) <= 0 )
                {
                    // got error or connection closed by client
                    if ( nbytes == 0 )
                    {
                        //remote conn closed
                    }
                    else
                    {
                        perror( "recv" );
                    }
                    close( i );
                    FD_CLR( i, &master );
                }
                else
                {
                    std::cout << buf << std::endl;
                    processRequest( buf, i );
                }
                close(i);
                FD_CLR(i, &master);
                memset( buf, '\0', sizeof( buf ) );
            }
        }
    }
}

void Observer::processRequest( std::string request, int i )
{
    std::stringstream reqStream;
    std::string reqLine;
    reqStream << request;
    int numbytes;
    bool validReq = true;

    while ( getline( reqStream, reqLine ) )
    {
        if( reqLine.find( "Origin" ) == 0 )
        {
            if ( !checkOrigin( reqLine ) )
            {
                break;
            }
            else
            {
                validReq = true;
            }
        }
    }

    if ( validReq )
    {
        std::string answer = createAnswer();
        numbytes = send( i, answer.c_str(), answer.size(), 0 );
    }
}

bool Observer::checkOrigin( std::string origin )
{
    // :P
    return 1;
}

std::string Observer::createAnswer()
{
    std::string answer;
    answer.append( "HTTP/1.1 200 OK\r\n" );
    answer.append( "Server: Observer/0.01\r\n" );
    answer.append( "Content-Type: text/xml\r\n" );
    //answer.append("WWW-Authenticate: Basic realm=\"Secure Area\"");
    answer.append( "Connection: close\r\n");
    answer.append( "\r\n" );

    answer.append( "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\r\n" );
    answer.append( "<body>\r\n" );

    answer.append( getReports() );

    answer.append( "</body>\r\n" );

    return answer;
}

std::string Observer::getReports()
{
    std::stringstream report;
    int i = 1;
    mongo::auto_ptr<mongo::DBClientCursor> cursor = c->query( "zabbix.reports", mongo::BSONObj() );
    while( cursor->more() )
    {
        mongo::BSONObj p = cursor->next();
        std::string status = p.getStringField( "status" );
        if( status.find( "PROBLEM" ) == 0 )
        {
            report  << "\t<p id=\""
                    << i
                    << "\" "
                    << "problem=\""
                    << p.getStringField( "problem" )
                    << "\"/>\r\n";
            i++;
        }
    }
    return report.str();
}

}
