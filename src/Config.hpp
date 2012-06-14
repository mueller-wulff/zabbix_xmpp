#ifndef ZABBIX_CONFIG_HPP
#define ZABBIX_CONFIG_HPP

#include <lua.hpp>
#include <iostream>

class Config
{
public:
    Config();
    ~Config();

    lua_State *L;
    std::string scriptDir;
    std::string jid;
    std::string jidpw;
    std::string mongohost;
    std::string mongodb;
    std::string jabberHost;
    std::string serverCert;
    std::string serverKey;
    std::string sslHost;
    std::string reportColl;
    std::string adminColl;
    std::string commandsColl;
    int uid;
    int janTimeout;
    time_t flaptime;
    std::string sslpword;

    void load( const char* fname );

private:
};

#endif
