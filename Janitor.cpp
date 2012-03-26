#include "Janitor.hpp"

using namespace gloox;

namespace zabbix
{

Janitor::Janitor( Client* _j, ConfigParser* _parser, mongo::DBClientConnection* _c )
{
    j = _j;
    c = _c;
    parser = _parser;
}

void Janitor::tidyUp()
{
    size_t found;
    std::string status;
    std::string problem;
    bool flapping;
    time_t now = time( 0 );
    time_t deltatime;

    mongo::auto_ptr<mongo::DBClientCursor> cursor = c->query( parser->getreportColl(), mongo::BSONObj() );
    while( cursor->more() )
    {
        const std::string coll = parser->getreportColl();
        mongo::BSONObj p = cursor->next();
        status = p.getStringField( "status" );
        found = status.find( "OK" );
        flapping = p.getBoolField( "flapping" );
        deltatime = time( 0 ) - p.getIntField( "new_timestamp" );
        problem = p.getStringField( "problem");

        if( flapping && deltatime > 600 && found < status.npos )
        {

            mongo::BSONObjBuilder* b;
            b = new mongo::BSONObjBuilder;
            b->append( "flapping" , !flapping );
            mongo::BSONObj tp = b->obj();

            c->update(  coll,
                        mongo::BSONObjBuilder().append( "problem", problem.c_str() ).obj(),
                        BSON( "$set" << tp),
                1, 0
             );
            delete b;
            sendOK( problem );
        }
    }
}

void Janitor::sendOK( std::string problem )
{
    mongo::auto_ptr<mongo::DBClientCursor> cursor = c->query( parser->getadminColl(), mongo::BSONObj() );
    problem.append( "\nOK\nflapping is over\n");

    while( cursor->more() )
    {
        mongo::BSONObj p = cursor->next();
        std::string currentAdmin = p.getStringField( "name" );
        currentAdmin.append( parser->getJabberHost() );
        Message::MessageType type;
        Message msg( type, currentAdmin, problem.c_str() );
        j->send( msg );
    }
}
}
