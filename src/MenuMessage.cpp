#include "MenuMessage.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "ResourceManager.h"
#include "Game.h"

#include "FontStyles.h"

constexpr unsigned int HOLDTIME = 5000;
constexpr unsigned int FADEOUTTIME = 2500;

const float LINES_STRIDE = 8.0f;

MenuMessage::MenuMessage( MenuHandler &handler, const MessageType messageType ) :
    MenuWidget( handler ),
    type( messageType ),
    currentIndex( 0 )
{
    switch( type )
    {
    case MessageType::ERROR:
        ApplyFontStyle( label, GAME_ERROR_FONT_NORMAL );
        break;
    case MessageType::NOTIFY:
        ApplyFontStyle( label, GAME_MESSAGE_FONT_NORMAL );
        break;
    default:
        ApplyFontStyle( label, GAME_MESSAGE_FONT_NORMAL );
        break;
    }

    Clear();
}

void MenuMessage::AddMessage( std::string &&message )
{
    line[currentIndex].thinktime = 0;
    line[currentIndex].message = std::move( message );
    line[currentIndex].alpha = 255;

    currentIndex += 1;

    if( currentIndex >= LINES_COUNT )
    {
        currentIndex = 0;
    }
}

void MenuMessage::Clear()
{
    for( auto i = 0u; i < LINES_COUNT; i++ )
    {
        line[i].thinktime = 0;
        line[i].message.clear();
        line[i].alpha = 0;
    }

    currentIndex = 0;
}

void MenuMessage::Draw( sf::RenderTarget &render )
{
    sf::RenderStates state;

    state.transform *= getTransform();

    for( auto i = 0u; i < LINES_COUNT; i++ )
    {
        label.setString( line[i].message );

        switch( type )
        {
        case MessageType::ERROR:
            label.setPosition( { -label.getLocalBounds().width / 2.0f, i * LINES_STRIDE } );
            break;
        case MessageType::NOTIFY:
            label.setPosition( { 0.0f, i * LINES_STRIDE } );
            break;
        default:
            label.setPosition( { 0.0f, i * LINES_STRIDE } );
            break;
        }

        label.move( { 0.0f, float( label.getCharacterSize() * i + LINES_STRIDE ) } );

        auto fillColor = sf::Color::White;
        auto outlineColor = sf::Color::Black;

        switch( type )
        {
        case MessageType::ERROR:
            fillColor = GAME_ERROR_FONT_NORMAL.fillColor;
            outlineColor = GAME_ERROR_FONT_NORMAL.outlineColor;
            break;

        case MessageType::NOTIFY:
            fillColor = GAME_MESSAGE_FONT_NORMAL.fillColor;
            outlineColor = GAME_MESSAGE_FONT_NORMAL.outlineColor;
            break;

        default:
            break;
        }

        label.setFillColor( { fillColor.r, fillColor.g, fillColor.b, line[i].alpha } );
        label.setOutlineColor( { outlineColor.r, outlineColor.g, outlineColor.b, line[i].alpha } );

        render.draw( label, state );
    }
}

void MenuMessage::Update()
{
    for( auto i = 0u; i < LINES_COUNT; i++ )
    {
        line[i].thinktime += game.time;

        if( line[i].thinktime < HOLDTIME )
        {
            line[i].alpha = 255;
            continue;
        }

        if( line[i].thinktime < ( HOLDTIME + FADEOUTTIME ) )
        {
            line[i].alpha = int( ( 1.0f - ( ( line[i].thinktime - HOLDTIME ) / float( FADEOUTTIME ) ) ) * 255 );
            continue;
        }

        line[i].message = "";
        line[i].alpha = 0;

        if( !i )
        {
            currentIndex = 0;
        }
    }
}
