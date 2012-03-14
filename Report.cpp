#include "Report.hpp"


namespace zabbix
{

Report::Report( Client* _j, ConfigParser* _parser, mongo::DBClientConnection* _c )
{
    j = _j;
    c = _c;
    parser = _parser;
}

void Report::reportIssue( const Message& command )
{
    std::string report = command.body();
    if( checkReport( report ) )
        sendReport( report );
}

void Report::sendReport( std::string report )
{
    mongo::auto_ptr<mongo::DBClientCursor> cursor = c->query( "zabbix.admins", mongo::BSONObj() );

    while( cursor->more() )
    {
        mongo::BSONObj p = cursor->next();
        std::string currentAdmin = p.getStringField( "name" );
        currentAdmin.append( "@localhost" );
        Message::MessageType type;
        Message msg( type, currentAdmin, report.c_str() );
        j->send( msg );
    }
}

bool Report::checkReport( std::string report )
{
    time_t deltatime = 0;
    if ( report.find_first_of( "PROBLEM" ) < report.npos )
    {
        deltatime = analyzeReport( report );
    }

    if ( deltatime > 600 )
    {
        return true;
    }
    else
    {
        return false;
    }
}

time_t Report::analyzeReport( std::string report )
{
    size_t found;
    std::string tempstr;
    found = report.find_first_of( ":" );
    tempstr = report.substr( found + 1, report.npos );
    found = tempstr.find_first_of( ":" );
    tempstr = tempstr.substr( 0, found );
    found = tempstr.find_last_of( " " );
    tempstr = tempstr.substr( 0, found );
    return storeReport( tempstr );
}

int Report::getTimestamp()
{
    int rawtime = time ( 0 );
    return rawtime;
}

time_t Report::storeReport( std::string report )
{
    const std::string coll = "zabbix.reports";
    time_t deltatime = 0;
    time_t newtime = time( 0 );

    mongo::auto_ptr<mongo::DBClientCursor> cursor = c->query( "zabbix.reports", QUERY( "problem"<<report.c_str() ) );
    while( cursor->more() )
    {
        mongo::BSONObj p = cursor->next();
        deltatime = newtime - p.getIntField( "timestamp");
    }

    mongo::BSONObjBuilder* b;
    b = new mongo::BSONObjBuilder;
    b->append( "timestamp", ( int ) newtime );
    mongo::BSONObj p = b->obj();
    c->update(  coll,
                mongo::BSONObjBuilder().append("problem", report.c_str() ).obj(),
                BSON( "$set" << p),
                1, 0
             );
    delete b;
    return deltatime;
}


}
