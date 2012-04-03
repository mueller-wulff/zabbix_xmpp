#include "ConfigParser.hpp"


namespace zabbix
{

ConfigParser::ConfigParser()
{
    openlog( "zabbix", LOG_PID, LOG_USER );

    hasConfig = 0;

    janTimeout = -2;

}

void ConfigParser::parse( int &argc, char** &argv )
{
    extractConfig( argc, argv );
    if ( hasConfig )
    {
        readConfig( conf );
    }
    else
    {
        syslog( LOG_INFO, "read config: no config" );
    }
}

void ConfigParser::extractConfig( int &argc, char** &argv )
{
    int opt = 0;
    while ( ( opt = getopt( argc, argv, "c:" ) ) != -1 )
    {
        switch( opt )
        {
            case 'c':
            {
                conf = optarg;
                hasConfig = 1;
                syslog( LOG_INFO, "trying to parse config: %s\n", conf );
                break;
            }
            default:
            {
                /*
                  do stuff, if there is no config
                */
                syslog( LOG_INFO, "no config: trying default values" );
            }
        }
    }
}

int ConfigParser::readConfig( char* config )
{
    int fd;
    char buf[4048];

    if ( (fd = open( config, O_RDONLY)) == -1 )
    {
        syslog( LOG_ALERT, "failed to open conf" );
        return -1;
    }

    if ( read( fd, buf, sizeof( buf ) ) == -1 )
    {
        syslog( LOG_ALERT, "failed to read conf");
        return -1;
    }

    close( fd );

    std::stringstream stream( buf );
    std::string line;

    while ( std::getline( stream, line ) )
    {
        size_t found;

        found = line.find_first_not_of( " " );
        if ( !(found < line.npos) )
        {
            continue;
        }
        if ( found > 0 )
        {
            line.replace(0, found, "");
        }

        found = line.find_first_of( "#" );
        if ( found == 0 )
        {
            continue;
        }

        found = line.find_first_of( " " );
        if ( found < line.npos )
        {
            std::string key   = line.substr( 0, found );
            std::string value = line.substr( found, line.npos );

            removeAllWhite( key );
            removeAllWhite( value );
            checkKey( key, value );
        }
    }
    return 0;
}

void ConfigParser::removeAllWhite( std::string &str )
{
    std::string temp;

    for ( unsigned int i = 0; i < str.length(); i++ )
    {
        if ( str[i] != ' ' )
        {
            temp += str[i];
        }
    }
    str = temp;
}

void ConfigParser::checkKey( std::string key, std::string value )
{

    if ( ( key.compare( 0, key.length(), "mongohost" )) == 0 )
    {
        setMongoHost( value );
        return;
    }

    if ( ( key.compare( 0, key.length(), "mongodb" ) ) == 0 )
    {
        setMongoDB( value );
        return;
    }

    if ( ( key.compare( 0, key.length(), "jid" ) ) == 0 )
    {
        setJID( value );
        return;
    }

    if ( ( key.compare( 0, key.length(), "jidpw" ) ) == 0 )
    {
        setJIDPW( value );
        return;
    }

    if ( ( key.compare( 0, key.length(), "scriptdir" ) ) == 0 )
    {
        setScriptDir( value );
        return;
    }

    if ( ( key.compare( 0, key.length(), "jantimeout" ) ) == 0 )
    {
        setjanTimeout( value );
        return;
    }

    if ( ( key.compare( 0, key.length(), "jabberhost" ) ) == 0 )
    {
        setJabberHost( value );
        return;
    }

    if ( ( key.compare( 0, key.length(), "servercert" ) ) == 0 )
    {
        setServerCert( value );
        return;
    }
    if ( ( key.compare( 0, key.length(), "serverkey" ) ) == 0 )
    {
        setServerKey( value );
        return;
    }
    if ( ( key.compare( 0, key.length(), "sslhost" ) ) == 0 )
    {
        setSSLHost( value );
        return;
    }
    if ( ( key.compare( 0, key.length(), "flaptime" ) ) == 0 )
    {
        setFlapTime( value );
        return;
    }
    if ( ( key.compare( 0, key.length(), "sslpword" ) ) == 0 )
    {
        setSSLPword( value );
        return;
    }
}

void ConfigParser::log()
{
    syslog( LOG_INFO, "mongohost set to %s", getMongoHost().c_str() );
    syslog( LOG_INFO, "mongodb set to %s", getMongoDB().c_str() );
    syslog( LOG_INFO, "jid set to %s", getJID().c_str() );
    syslog( LOG_INFO, "jidpw set to %s", getJIDPW().c_str() );
    syslog( LOG_INFO, "jantimeout set to %d", getjanTimeout() );
    syslog( LOG_INFO, "servercert set to %s", getServerCert().c_str() );
    syslog( LOG_INFO, "serverkey set to %s", getServerKey().c_str() );
    syslog( LOG_INFO, "sslhost set to %s", getSSLHost().c_str() );
    syslog( LOG_INFO, "flaptime set to %d", getFlapTime() );
    syslog( LOG_INFO, "sslpword set to %s", getSSLPword().c_str() );
}

void ConfigParser::setMongoHost( std::string value )
{
    mongohost = value;
}

std::string ConfigParser::getMongoHost()
{
    if ( mongohost.empty() )
    {
        mongohost = "127.0.0.1:27017";
    }
    return mongohost;
}

void ConfigParser::setMongoDB( std::string value )
{
    mongodb = value;
}

std::string ConfigParser::getMongoDB()
{
    if ( mongodb.empty() )
    {
        mongodb = "zabbix";
    }
    return mongodb;
}

void ConfigParser::setJID( std::string value )
{
    jid = value;
}

std::string ConfigParser::getJID()
{
    if ( jid.empty() )
    {
        abort();
    }
    return jid;
}

void ConfigParser::setJIDPW( std::string value )
{
    jidpw = value;
}

std::string ConfigParser::getJIDPW()
{
    if ( jidpw.empty() )
    {
        abort();
    }
    return jidpw;
}

void ConfigParser::setScriptDir( std::string value )
{
    scriptdir = value;
}

std::string ConfigParser::getScriptDir()
{
    if ( scriptdir.empty() )
    {
        abort();
    }
    return scriptdir;
}

void ConfigParser::setjanTimeout( std::string value )
{
    janTimeout = atoi( value.c_str() );
}

int ConfigParser::getjanTimeout()
{
    if ( janTimeout == -2 )
    {
        abort();
    }
    return janTimeout;
}

void ConfigParser::setJabberHost( std::string value )
{
    jabberhost = value;
}

std::string ConfigParser::getJabberHost()
{
    if ( jabberhost.empty() )
    {
        abort();
    }
    return jabberhost;
}

void ConfigParser::setServerCert( std::string value )
{
    servercert = value;
}

std::string ConfigParser::getServerCert()
{
    if ( servercert.empty() )
    {
        abort();
    }
    return servercert;
}

void ConfigParser::setServerKey( std::string value )
{
    serverkey = value;
}

std::string ConfigParser::getServerKey()
{
    if ( serverkey.empty() )
    {
        abort();
    }
    return serverkey;
}

void ConfigParser::setSSLHost( std::string value )
{
    sslhost = value;
}

void ConfigParser::setFlapTime( std::string value )
{
    flaptime = ( time_t ) atoi( value.c_str() );
}

time_t ConfigParser::getFlapTime()
{
    return flaptime;
}

std::string ConfigParser::getSSLHost()
{
    if ( sslhost.empty() )
    {
        abort();
    }
    return sslhost;
}

void ConfigParser::setSSLPword( std::string value )
{
    sslpword = value;
}

std::string ConfigParser::getSSLPword()
{
    return sslpword;
}

std::string ConfigParser::getreportColl()
{
    return mongodb + ".reports";
}

std::string ConfigParser::getadminColl()
{
    return mongodb + ".admins";
}

std::string ConfigParser::getcommandsColl()
{
    return mongodb + ".commands";
}

}
