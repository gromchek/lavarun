#include "KeyBinds.h"
#include "UserInput.h"
#include "Misc.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <SFML/Window/Keyboard.hpp>

constexpr unsigned int KB_UNKNOWN_KEY = 255;

void KeyBinds::initKeyNames()
{
    keyNames[KB_UNKNOWN_KEY].append( "???" );

    unsigned int scancode = sf::Keyboard::A;

    for( auto i = 0u; i < 26; i++ )
	{
        keyNames[scancode++] = std::move( std::string{ char( 'A' + i ) } );
	}

	scancode = sf::Keyboard::Num0;

    for( auto i = 0u; i < 10; i++ )
	{
		char buf[8];
        sprintf( buf, "NUM%u", i );
        keyNames[scancode++] = std::move( std::string{ buf } );
	}

	scancode = sf::Keyboard::F1;

    for( auto i = 0u; i < 12; i++ )
	{
		char buf[8];
        sprintf( buf, "F%u", i + 1 );
        keyNames[scancode++] = std::move( std::string{ buf } );
	}
}

void KeyBinds::loadDefaultBinds()
{
    SetBind( "Q", "actionbtn0" );
    SetBind( "W", "actionbtn1" );
    SetBind( "E", "actionbtn2" );
    SetBind( "R", "actionbtn3" );
    SetBind( "F", "actionbtn4" );
    SetBind( "G", "actionbtn5" );
    SetBind( "H", "actionbtn6" );
    SetBind( "J", "actionbtn7" );
}

KeyBinds::KeyBinds()
{
    initKeyNames();

    loadDefaultBinds();
}

unsigned int KeyBinds::GetBind( std::string_view command ) const
{
    auto bind = std::find_if( binds.begin(), binds.end(), [&command]( const auto &b ) { return b.second == command; } );

    if( bind != binds.end() )
	{
        return bind->first;
	}

    return KB_UNKNOWN_KEY;
}

void KeyBinds::SetBind( std::string_view keyName, std::string_view command )
{
    const auto scancode = StringToKeyName( keyName );

    binds[scancode] = command;
}

void KeyBinds::ReadBinds( const std::string &fileName )
{
	std::ifstream file( fileName );

	if( !file )
	{
		return;
	}

	std::string fileLine;

	while( std::getline( file, fileLine ) )
	{
		if( fileLine.empty() || fileLine[0] == '#' )
		{
			continue;
		}

		std::istringstream iss( fileLine );
		std::string token;
		while( iss >> token )
		{
			if( token != "bind" )
			{
				break;
			}

			iss >> token;
            if( token.empty() )
			{
                break;
			}
            auto codeStr = token;

			iss >> token;
            if( token.empty() )
			{
                break;
			}
            auto actionStr = token;

            toUpperCase( codeStr );
            toLowerCase( actionStr );

            SetBind( codeStr, actionStr );
		}
	}
}

void KeyBinds::WriteBinds( const std::string &fileName ) const
{
	std::ofstream file( fileName, std::ofstream::out | std::ofstream::trunc );

    for( auto &&[scancode, command] : binds )
	{
        file << "bind" << ' ' << KeyNameToString( scancode ) << ' ' << command << '\n';
	}
}

unsigned int KeyBinds::StringToKeyName( std::string_view str ) const
{
    auto iter = std::find_if( keyNames.begin(), keyNames.end(), [&str]( const auto &s ) { return s == str; } );

    if( iter != keyNames.end() )
	{
        return std::distance( keyNames.begin(), iter );
	}

    return KB_UNKNOWN_KEY;
}

std::string_view KeyBinds::KeyNameToString( const unsigned int scancode ) const
{
    if( scancode < keyNames.size() )
    {
        return keyNames[scancode];
    }

    return "???";
}

KeyBinds keyBinds;
