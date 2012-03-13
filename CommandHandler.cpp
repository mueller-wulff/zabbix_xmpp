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
            //!show
            case 0:
            {
                showCommands( command );
                break;
            }
            //!execute
            case 1:
            {
                validateExecute( command );
                break;
            }
            //!learn
            case 2:
            {
                std::cout << "learn" << std::endl;
                break;
            }
            //!report
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
    else
    {
        sendHelp( command );
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
        commandList.append( p.getStringField("name") );
    }

    Message::MessageType type;
    Message msg( type,  command.from(), commandList.c_str() );
    j->send( msg );
}

void CommandHandler::validateExecute( const Message& command )
{
    bool executed = false;
    std::string appendix = getAppendix( command.body() );
    mongo::auto_ptr<mongo::DBClientCursor> cursor = c->query( "zabbix.commands", mongo::BSONObj() );
    while( cursor->more() )
    {
        mongo::BSONObj p = cursor->next();
        if ( appendix.compare( 0, appendix.length(), p.getStringField("name") ) == 0 )
        {
            Message::MessageType type;
            std::string commandType = p.getStringField("type");
            if(commandType.compare( 0, commandType.length(), "script" ) == 0 )
            {
                Message msg( type,  command.from(), executeScript( p.getStringField( ( "command" ) ) ) );
                j->send( msg );
                executed = true;
            }
            if(commandType.compare( 0, commandType.length(), "shell" ) == 0 )
            {
                Message msg( type,  command.from(), executeShell( p.getStringField( ( "command" ) ) ) );
                j->send( msg );
                executed = true;
            }
            break;
        }
    }
    if( !executed )
    {
        Message::MessageType type;
        Message msg( type,  command.from(), "Dont know what to execute!" );
        j->send( msg );
    }
}

std::string CommandHandler::executeScript( std::string script )
{
    std::string myScriptFolder = "/home/roa/programming/zabbix/notifier/zabbix_xmpp/script/";
    myScriptFolder.append( script.c_str() );
    FILE* pipe = popen( myScriptFolder.c_str(), "r" );
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while( !feof( pipe ) )
    {
        if( fgets( buffer, 128, pipe ) != NULL )
            result += buffer;
    }
    pclose( pipe );
    return result;
}

std::string CommandHandler::executeShell( std::string shell )
{
    FILE* pipe = popen( shell.c_str(), "r" );
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "\n";
    while( !feof( pipe ) )
    {
        if( fgets( buffer, 128, pipe ) != NULL )
            result += buffer;
    }
    pclose( pipe );
    return result;
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
