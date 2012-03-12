#include "CommandHandler.hpp"

namespace zabbix
{

CommandHandler::CommandHandler( Client* _j)
{
    j = _j;
    connectMongo();
    initCommandArr();
}

void CommandHandler::connectMongo()
{
    c = new mongo::DBClientConnection( true,0 ,0 );
    c->connect( "localhost:27017" );

}

void CommandHandler::validateCommand( const Message& command )
{
    std::string commandBody     = getCommand( command.body() );
    std::string commandAppendix = getAppendix( command.body() );

    if ( commandBody.find_first_of( "!" ) == 0 )
    {
        switch( searchCommand( commands, COMMANDCOUNT, commandBody.c_str() ) )
        {
            case 0:
            {
                showCommands( command );
                break;
            }
            case 1:
            {
                std::cout << "execute" << std::endl;
                break;
            }
            case 2:
            {
                std::cout << "learn" << std::endl;
                break;
            }
            case 3:
            {
                std::cout << "report" << std::endl;
                break;
            }
            default:
            {
                sendHelp( command );
            }
        }
    }
}

int CommandHandler::searchCommand(std::string A[], int size, std::string target)
{
    int j;

	for(j=0; j < size; j++)
	{
        if(A[j] == target)
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
}

void CommandHandler::sendHelp( const Message& command )
{
    Message::MessageType type;
    std::string helpStr = createHelpStr();
    Message msg( type,  command.from(), helpStr );
    j->send( msg );
}

std::string CommandHandler::createHelpStr()
{
    std::string helpStr;
    helpStr.append( "\nI do not know your command" );
    helpStr.append( "\nRight now, i know these commands:");
    for( int j=0; j<COMMANDCOUNT; j++)
    {
        helpStr.append( "\n" );
        helpStr.append( commands[j] );
    }
    return helpStr;
}

void CommandHandler::showCommands( const Message& command )
{
    std::string commandList;
    mongo::auto_ptr<mongo::DBClientCursor> cursor = c->query( "zabbix.commands", mongo::BSONObj() );

    while( cursor->more() )
    {
        mongo::BSONObj p = cursor->next();
        commandList.append( "\n" );
        commandList.append( p.getStringField("test") );
    }

    Message::MessageType type;
    Message msg( type,  command.from(), commandList.c_str() );
    j->send( msg );
}

std::string CommandHandler::getCommand( std::string command )
{
    size_t found = command.find_first_of( " " );
    if (found < command.npos )
    {
        return command.substr( 0, found );
    }
    return command;
}

std::string CommandHandler::getAppendix( std::string appendix )
{
    size_t found = appendix.find_first_of( " " );
    if (found < appendix.npos )
    {
        return appendix.substr( found + 1, appendix.npos );
    }
    return "";
}

}
