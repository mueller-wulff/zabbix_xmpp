#include "Config.hpp"

Config::Config()
{

}

Config::~Config()
{

}

void Config::load( const char* fname )
{
    L = luaL_newstate();
    if( luaL_loadfile( L, fname ) || lua_pcall( L, 0, 0, 0 ) )
    {
        std::cout << "cannot run config file: " << lua_tostring( L, -1 ) << std::endl;
    }

    /**********************
     *    load scriptdir  *
     **********************/
    lua_getglobal( L, "scriptdir" );
    if( !lua_isstring( L, 1 ) )
    {
        std::cerr << "scriptDir is not a string" << std::endl;
    }
    else
    {
        scriptDir = lua_tostring( L, 1 );
    }
    lua_pop( L, 1 );

    /**********************
     *    load jid        *
     **********************/
    lua_getglobal( L, "jid" );
    if( !lua_isstring( L, 1 ) )
    {
        std::cerr << "jid is not a string" << std::endl;
    }
    else
    {
        jid = lua_tostring( L, 1 );
    }
    lua_pop( L, 1 );

    /**********************
     *    load jidpw      *
     **********************/
    lua_getglobal( L, "jidpw" );
    if( !lua_isstring( L, 1 ) )
    {
        std::cerr << "jidpw is not a string" << std::endl;
    }
    else
    {
        jidpw = lua_tostring( L, 1 );
    }
    lua_pop( L, 1 );

    /**********************
     *    load mongohost  *
     **********************/
    lua_getglobal( L, "mongohost" );
    if( !lua_isstring( L, 1 ) )
    {
        std::cerr << "mongohost is not a string" << std::endl;
    }
    else
    {
        mongohost = lua_tostring( L, 1 );
    }
    lua_pop( L, 1 );

    /**********************
     *    load mongodb    *
     **********************/
    lua_getglobal( L, "mongodb" );
    if( !lua_isstring( L, 1 ) )
    {
        std::cerr << "mongodb is not a string" << std::endl;
    }
    else
    {
        mongodb      = lua_tostring( L, 1 );
        reportColl   = mongodb + ".reports";
        adminColl    = mongodb + ".admins";
        commandsColl = mongodb + ".commands";
    }
    lua_pop( L, 1 );

    /***********************
     *    load jabberhost  *
     ***********************/
    lua_getglobal( L, "jabberhost" );
    if( !lua_isstring( L, 1 ) )
    {
        std::cerr << "jabberhost is not a string" << std::endl;
    }
    else
    {
        jabberHost = lua_tostring( L, 1 );
    }
    lua_pop( L, 1 );

    /***********************
     *    load servercert  *
     ***********************/
    lua_getglobal( L, "servercert" );
    if( !lua_isstring( L, 1 ) )
    {
        std::cerr << "servercert is not a string" << std::endl;
    }
    else
    {
        serverCert = lua_tostring( L, 1 );
    }
    lua_pop( L, 1 );

    /***********************
     *    load serverkey  *
     ***********************/
    lua_getglobal( L, "serverkey" );
    if( !lua_isstring( L, 1 ) )
    {
        std::cerr << "serverkey is not a string" << std::endl;
    }
    else
    {
        serverKey = lua_tostring( L, 1 );
    }
    lua_pop( L, 1 );

    /***********************
     *    load sslhost  *
     ***********************/
    lua_getglobal( L, "sslhost" );
    if( !lua_isstring( L, 1 ) )
    {
        std::cerr << "sslhost is not a string" << std::endl;
    }
    else
    {
        sslHost = lua_tostring( L, 1 );
    }
    lua_pop( L, 1 );


     /***********************
     *    load sslpword  *
     ***********************/
    lua_getglobal( L, "sslpword" );
    if( !lua_isstring( L, 1 ) )
    {
        std::cerr << "sslpword is not a string" << std::endl;
    }
    else
    {
        sslpword = lua_tostring( L, 1 );
    }
    lua_pop( L, 1 );

     /***********************
     *    load uid  *
     ***********************/
    lua_getglobal( L, "uid" );
    if( !lua_isnumber( L, 1 ) )
    {
        std::cerr << "uid is not a number" << std::endl;
    }
    else
    {
        uid = lua_tonumber( L, 1 );
    }
    lua_pop( L, 1 );

     /***********************
     *    load janTimeout  *
     ***********************/
    lua_getglobal( L, "jantimeout" );
    if( !lua_isnumber( L, 1 ) )
    {
        std::cerr << "janTimeout is not a number" << std::endl;
    }
    else
    {
        janTimeout = lua_tonumber( L, 1 );
    }
    lua_pop( L, 1 );

     /***********************
     *    load flaptime  *
     ***********************/
    lua_getglobal( L, "flaptime" );
    if( !lua_isnumber( L, 1 ) )
    {
        std::cerr << "flaptime is not a number" << std::endl;
    }
    else
    {
        flaptime = lua_tonumber( L, 1 );
    }
    lua_pop( L, 1 );

    lua_close( L );
}
