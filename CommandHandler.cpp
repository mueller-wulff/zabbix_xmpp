#include "CommandHandler.hpp"

namespace zabbix
{

CommandHandler::CommandHandler( Client* _j, ConfigParser* _parser )
{
    j = _j;
    parser = _parser;
    connectMongo();
    initCommandArr();
}

void CommandHandler::connectMongo()
{
    c = new mongo::DBClientConnection( true,0 ,0 );
    c->connect( parser->getMongoHost() );

}

void CommandHandler::checkAuth( const Message& command )
{
    if( auth( command ) )
    {
        validateCommand( command );
    }
    else
    {
        Message::MessageType type;
        std::string helpStr = "Your are not allowed to do this!";
        Message msg( type,  command.from(), helpStr );
        j->send( msg );
    }
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
                learnCommand( command );
                break;
            }
            //!report
            case 3:
            {
                reportIssue( command );
                break;
            }
            //!help
            case 4:
            {
                helpCommand( command );
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
    helpStr.append( "\nRight now, i know these commands:" );
    for( int j=0; j<COMMANDCOUNT; j++ )
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
        commandList.append( p.getStringField( "name" ) );
    }

    Message::MessageType type;
    Message msg( type,  command.from(), commandList.c_str() );
    j->send( msg );
}

int CommandHandler::validateExecute( const Message& command )
{
    bool executed = false;
    std::string appendix = getAppendix( command.body() );
    if( appendix.empty() )
    {
        /*
            send execute help
        */
        return -1;
    }

    mongo::auto_ptr<mongo::DBClientCursor> cursor = c->query( "zabbix.commands", mongo::BSONObj() );
    while( cursor->more() )
    {
        mongo::BSONObj p = cursor->next();
        if ( appendix.compare( 0, appendix.length(), p.getStringField( "name" ) ) == 0 )
        {
            Message::MessageType type;
            std::string commandType = p.getStringField("type");
            if( commandType.compare( 0, commandType.length(), "script" ) == 0 )
            {
                Message msg( type,  command.from(), executeScript( p.getStringField( ( "command" ) ) ) );
                j->send( msg );
                executed = true;
            }
            if( commandType.compare( 0, commandType.length(), "shell" ) == 0 )
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
    return 0;
}

std::string CommandHandler::executeScript( std::string script )
{
    std::string myScriptFolder = parser->getScriptDir();
    myScriptFolder.append( script.c_str() );
    FILE* pipe = popen( myScriptFolder.c_str(), "r" );
    if ( !pipe ) return "ERROR";
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
    if ( !pipe ) return "ERROR";
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

int CommandHandler::learnCommand( const Message& command )
{
    if( getAppendix( command.body() ).empty() )
    {
        /*
            send learn help
        */
        return -1;
    }
    std::string commandType    = getFirst( getAppendix( command.body() ) );
    std::string commandName    = getFirst( getAppendix( getAppendix( command.body() ) ) );
    std::string commandCommand = getAppendix( getAppendix( getAppendix( command.body() ) ) );

    mongo::BSONObjBuilder b;
    b.genOID();
    b.append( "command", commandCommand.c_str() );
    b.append( "name", commandName.c_str() );
    b.append( "type", commandType.c_str() );
    mongo::BSONObj p = b.obj();
    c->insert( "zabbix.commands", p );

    std::string info;
    info.append( "\nI learned the following:\nCommand-Name: " );
    info.append( commandName.c_str() );
    info.append( "\nCommand-Type: " );
    info.append( commandType.c_str() );
    info.append( "\nCommand actually executed: " );
    info.append( commandCommand );

    Message::MessageType type;
    Message msg( type,  command.from(), info );
    j->send( msg );
    return 0;
}

void CommandHandler::reportIssue( const Message& command )
{
    std::cout << "report" << std::endl;
}

void CommandHandler::helpCommand( const Message& command )
{
    Message::MessageType type;
    std::string help = "\n";
    bool extendedHelp = false;
    switch( searchCommand( commands, COMMANDCOUNT, getAppendix( command.body() ).c_str() ) )
    {
        case 0:
        {
            help.append( "!show shows you all avaiable commands i understand" );
            help.append( "\n!show takes no arguments" );
            break;
        }
        case 1:
        {
            help.append( "!execute executes a shell command or script i learned" );
            help.append( "\nthe first argument of !execute is the command i execute" );
            help.append( "\nto show all avaiable commands type !show" );
            break;
        }
        case 2:
        {
            help.append( "\n!learn tells me, which new command i have to learn" );
            help.append( "\nyou have to format the !learn command this way:" );
            help.append( "\n!learn type name command" );
            help.append( "\ntype can be shell or script\nif you choose script, the script has to be in the scriptdir" );
            help.append( "\nname is a freely choosen name. with this name you can execute learned commands" );
            help.append( "\ncommand is the actual executed shell command or scriptname" );
            break;
        }
        case 3:
        {
            help.append( "\n!report is not for interactive usage\nplease dont use it" );
            break;
        }
        case 4:
        {
            help.append( "\n!help shows this help\nfirst argument can be a command i know\n" );
            extendedHelp = true;
            break;
        }
        default:
        {
            help.append( "\ni did not understand your question\n" );
            help.append( "\n!help shows this help\nfirst argument can be a command i know\n" );
            extendedHelp = true;
        }
    }
    if( extendedHelp )
    {
        help.append( createHelpStr().c_str() );
    }
    Message msg( type,  command.from(), help );
    j->send( msg );
}

bool CommandHandler::auth( const Message& command )
{
    mongo::auto_ptr<mongo::DBClientCursor> cursor = c->query( "zabbix.admins", mongo::BSONObj() );
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
