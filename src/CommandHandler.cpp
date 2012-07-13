#include "CommandHandler.hpp"

namespace zabbix
{

CommandHandler::CommandHandler( Client* _j, Config* _config )
{
    j = _j;
    config = _config;
    connectMongo();

    show    = new Show( j, config, c );
    execute = new Execute( j, config, c );
    learn   = new Learn( j, config, c );
    report  = new Report( j, config, c );
    help    = new Help( j, config, c );
    forget  = new Forget( j, config, c );
    janitor = new Janitor( j, config, c );

    initCommandArr();
}

CommandHandler::~CommandHandler()
{
    delete show;
    delete execute;
    delete learn;
    delete report;
    delete help;
    delete forget;
    delete janitor;
    delete c;
}

void CommandHandler::connectMongo()
{
    c = new mongo::DBClientConnection( true,0 ,0 );
    c->connect( config->mongohost );
}

bool CommandHandler::checkAuth( const Message& command )
{
    if( auth( command ) )
    {
        return true;
    }
    return false;
}

void CommandHandler::tidyUp()
{
    janitor->tidyUp();
}

void CommandHandler::validateCommand( const Message& command )
{
    std::string commandBody     = getFirst( command.body() );
    std::string commandAppendix = getAppendix( command.body() );

    if ( commandBody.find_first_of( "!" ) == 0 )
    {
        switch( searchCommand( commands, COMMANDCOUNT, commandBody.c_str() ) )
        {
            //!show
            case 0:
            {
                show->showCommands( command );
                break;
            }
            //!execute
            case 1:
            {
                execute->validateExecute( command );
                break;
            }
            //!learn
            case 2:
            {
                learn->learnCommand( command );
                break;
            }
            //!report
            case 3:
            {
                report->reportIssue( command );
                break;
            }
            //!help
            case 4:
            {
                help->helpCommand( command );
                break;
            }
            //!forget
            case 5:
            {
                forget->forgetCommand( command );
                break;
            }
            default:
            {
                help->sendHelp( command );
            }
        }
    }
    else
    {
        help->sendHelp( command );
    }
}

int CommandHandler::searchCommand( std::string A[], int size, std::string target )
{
    int j;

	for( j=0; j < size; j++ )
	{
        if( A[j] == target )
        {
            return j;
        }
	}
	return -1;
}

void CommandHandler::initCommandArr()
{
    commands[0] = "!show";
    commands[1] = "!execute";
    commands[2] = "!learn";
    commands[3] = "!report";
    commands[4] = "!help";
    commands[5] = "!forget";
}

bool CommandHandler::auth( const Message& command )
{
    mongo::auto_ptr<mongo::DBClientCursor> cursor = c->query( config->adminColl, mongo::BSONObj() );
    while( cursor->more() )
    {
        mongo::BSONObj currentAdmin = cursor->next();
        std::string username = command.from().username();

        if( username.compare( 0, username.length(), currentAdmin.getStringField( "name" ) ) == 0 )
        {
            return true;
        }
    }
    return false;
}

std::string CommandHandler::getFirst( std::string command )
{
    size_t found = command.find_first_of( " " );
    if ( found < command.npos )
    {
        return command.substr( 0, found );
    }
    return command;
}

std::string CommandHandler::getAppendix( std::string appendix )
{
    size_t found = appendix.find_first_of( " " );
    if ( found < appendix.npos )
    {
        return appendix.substr( found + 1, appendix.npos );
    }
    return "";
}

}
