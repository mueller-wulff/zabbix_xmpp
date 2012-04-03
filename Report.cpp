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
    mongo::auto_ptr<mongo::DBClientCursor> cursor = c->query( parser->getadminColl() , mongo::BSONObj() );

    while( cursor->more() )
    {
        mongo::BSONObj p = cursor->next();
        std::string currentAdmin = p.getStringField( "name" );
        if( currentAdmin == "notifier")
        {
            continue;
        }
        currentAdmin.append( "@" );
        currentAdmin.append( parser->getJabberHost() );
        Message::MessageType type;
        Message msg( type, currentAdmin, report.c_str() );
        j->send( msg );
    }
}

bool Report::checkReport( std::string report )
{
    size_t found;
    time_t deltatime = 0;
    std::string status;

    found = report.find( "OK" );
    if ( found < report.npos )
    {
        status = "OK";
    }

    found = report.find( "PROBLEM" );
    if ( found < report.npos )
    {
        status = "PROBLEM";
    }
    return analyzeReport( report, status );
}

bool Report::analyzeReport( std::string report, std::string status )
{
    size_t found;
    std::string tempstr;

    found = report.find_first_of( "!report" );
    if ( found < report.npos )
    {
        tempstr = report.substr( sizeof("!report ") , report.npos );
    }

    return storeReport( tempstr, status );
}

int Report::getTimestamp()
{
    int rawtime = time ( 0 );
    return rawtime;
}

bool Report::storeReport( std::string report, std::string status )
{
    const std::string coll = parser->getreportColl();
    std::string oldstatus;
    size_t found = report.find( " : " );

    std::string problem;
    std::string host;

    host = report.substr( status.size(), found - status.size() );

    problem = report.substr( found + 3, report.npos );

    time_t deltatime = 0;
    time_t olddeltatime = 0;
    time_t newtime = time( 0 );
    time_t oldtime = time( 0 );
    bool flapping = 0;

    mongo::auto_ptr<mongo::DBClientCursor> cursor = c->query( parser->getreportColl(), QUERY("problem"<<problem.c_str()<<"host"<<host.c_str()) );
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
                mongo::BSONObjBuilder().append("host", host.c_str() ).append("problem", problem.c_str() ).obj(),
                BSON( "$set" << p),
                1, 0
             );
    delete b;
    return flapping;
}

}
