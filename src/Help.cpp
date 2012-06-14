#include "Help.hpp"

namespace zabbix
{

Help::Help( Client* _j, Config* _parser, mongo::DBClientConnection* _c )
    : Commands( _j, _parser, _c )
{
    initCommandArr();
}

void Help::helpCommand( const Message& command )
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

void Help::sendHelp( const Message& command )
{
    Message::MessageType type;
    std::string helpStr = createHelpStr();
    Message msg( type,  command.from(), helpStr );
    j->send( msg );
}

int Help::searchCommand( std::string A[], int size, std::string target )
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

void Help::initCommandArr()
{
    commands[0] = "!show";
    commands[1] = "!execute";
    commands[2] = "!learn";
    commands[3] = "!report";
    commands[4] = "!help";
    commands[5] = "!forget";
}

std::string Help::createHelpStr()
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

}
