#ifndef ZABBIX_CONFIGPARSER_HPP
#define ZABBIX_CONFIGPARSER_HPP

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <syslog.h>

namespace zabbix
{

class ConfigParser
{
private:
    char * conf;
    bool hasConfig;

    std::string scriptdir;
    std::string jid;
    std::string jidpw;
    std::string mongohost;
    std::string mongodb;
    std::string jabberhost;
    std::string servercert;
    std::string serverkey;
    std::string sslhost;
    std::string reportColl;
    std::string adminColl;
    std::string commandsColl;
    int uid;
    int janTimeout;
    time_t flaptime;
    std::string sslpword;

public:
    ConfigParser();

    void parse( int &argc, char** &argv );

    void extractConfig( int &argc, char** &argv );

    int readConfig( char* config );

    void removeAllWhite( std::string &str );

    void checkKey( std::string key, std::string value );

    void log();

    void setMongoHost( std::string value );

    std::string getMongoHost();

    void setMongoDB( std::string value );

    std::string getMongoDB();

    void setJID( std::string value );

    std::string getJID();

    void setJIDPW( std::string value );

    std::string getJIDPW();

    void setScriptDir( std::string value );

    std::string getScriptDir();

    void setjanTimeout( std::string value );

    int getUID();

    void setUID( std::string value );

    int getjanTimeout();

    void setJabberHost( std::string value );

    std::string getJabberHost();

    void setServerCert( std::string value );

    std::string getServerCert();

    void setServerKey( std::string value );

    std::string getServerKey();

    void setSSLHost( std::string value );

    std::string getSSLHost();

    void setFlapTime( std::string value );

    time_t getFlapTime();

    void setSSLPword( std::string value );

    std::string getSSLPword();

    std::string getreportColl();

    std::string getadminColl();

    std::string getcommandsColl();

};

}

#endif
