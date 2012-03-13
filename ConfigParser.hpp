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
};

}

#endif
