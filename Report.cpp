#include "Report.hpp"


namespace zabbix
{

Report::Report( Client* _j, ConfigParser* _parser, mongo::DBClientConnection* _c )
    : Commands( _j, _parser, _c )
{
}

void Report::reportIssue( const Message& command )
{
    std::string report = command.body();
    if( !checkReport( report ) )
        sendReport( report );
}

void Report::sendReport( std::string report )
{
    mongo::auto_ptr<mongo::DBClientCursor> cursor = c->query( "zabbix.admins", mongo::BSONObj() );

    while( cursor->more() )
    {
        mongo::BSONObj p = cursor->next();
        std::string currentAdmin = p.getStringField( "name" );
        currentAdmin.append( "@" );
        currentAdmin.append( parser->getJabberHost() );
        Message::MessageType type;
        Message msg( type, currentAdmin, report.c_str() );
        j->send( msg );
    }
}

bool Report::checkReport( std::string report )
{
    time_t deltatime = 0;
    std::string status;
    size_t found = report.find( "OK" );

    if ( found < report.npos )
    {
        status = "OK";
    }
    else
    {
        status = "PROBLEM";
    }

    return analyzeReport( report, status );
}

bool Report::analyzeReport( std::string report, std::string status )
{
    size_t found;
    std::string tempstr;
    found = report.find_first_of( ":" );
    tempstr = report.substr( found + 1, report.npos );
    found = tempstr.find_first_of( ":" );
    tempstr = tempstr.substr( 0, found );
    found = tempstr.find_last_of( " " );
    tempstr = tempstr.substr( 0, found );
    return storeReport( tempstr, status );
}

int Report::getTimestamp()
{
    int rawtime = time ( 0 );
    return rawtime;
}

bool Report::storeReport( std::string report, std::string status )
{
    const std::string coll = "zabbix.reports";
    std::string oldstatus;
    time_t deltatime = 0;
    time_t olddeltatime = 0;
    time_t newtime = time( 0 );
    time_t oldtime = time( 0 );
    bool flapping = 0;

    mongo::auto_ptr<mongo::DBClientCursor> cursor = c->query( "zabbix.reports", QUERY( "problem"<<report.c_str() ) );
    while( cursor->more() )
    {
        mongo::BSONObj p = cursor->next();
        oldtime = p.getIntField( "new_timestamp");
        olddeltatime = oldtime - p.getIntField( "old_timestamp" );
        oldstatus = p.getStringField( "status" );
    }

    deltatime = newtime - oldtime;

    if ( deltatime < 600 && olddeltatime < 600 )
    {
        flapping = true;
    }
    if ( deltatime == 0 || olddeltatime == 0 )
    {
        flapping = false;
    }

    mongo::BSONObjBuilder* b;
    b = new mongo::BSONObjBuilder;
    b->append( "flapping" , flapping );
    b->append( "new_timestamp", ( int ) newtime );
    b->append( "old_timestamp", ( int ) oldtime );
    b->append( "status", status.c_str() );
    mongo::BSONObj p = b->obj();
    c->update(  coll,
                mongo::BSONObjBuilder().append("problem", report.c_str() ).obj(),
                BSON( "$set" << p),
                1, 0
             );
    delete b;
    return flapping;
}

}
