#include "MenuMultiboard.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "FontStyles.h"

#include "Game.h"

#include "ResourceManager.h"

const float ROW_Y_STRIDE = 8.0f;

MenuMultiboard::MenuMultiboard( MenuHandler &handler, const unsigned int cellCount ) :
    MenuWidget( handler ),
    currentRow( 0 ),
    background( { MULTIBOARD_DEFAULT_SIZE.x * cellCount,
                  MULTIBOARD_DEFAULT_SIZE.y * MAX_ROWS + ( ROW_Y_STRIDE * MAX_ROWS - 1 ) },
                &resourceManager.GetTextureByName( "dialog-box" ) ),
    header( { MULTIBOARD_DEFAULT_SIZE.x * cellCount, 63.0f }, &resourceManager.GetTextureByName( "dialog-header" ) )
{
    ApplyFontStyle( row, GAME_MULTIBOARD_FONT_NORMAL );

    ApplyFontStyle( title, GAME_MULTIBOARD_FONT_HEADER );

    Clear();
}

void MenuMultiboard::SetTitle( std::string &&str )
{
    title.setString( std::move( str ) );
}

void MenuMultiboard::AddRow( std::string &&value )
{
    if( currentRow == table.size() )
    {
        for( auto i = 1u; i < table.size(); i++ )
        {
            table[i - 1] = table[i];
        }

        currentRow = table.size() - 1;
    }

    table[currentRow].text = std::move( value );

    currentRow += 1;
}

void MenuMultiboard::ClearRowsColor()
{
    for( auto &row : table )
    {
        row.color = GAME_MULTIBOARD_FONT_NORMAL.fillColor;
    }
}

void MenuMultiboard::SetRowColor( const unsigned int row, const sf::Color color )
{
    if( row >= table.size() )
    {
        return;
    }

    table[row].color = color;
}

void MenuMultiboard::Clear()
{
    for( auto &row : table )
    {
        row.text.clear();
        row.color = GAME_MULTIBOARD_FONT_NORMAL.fillColor;
    }

    currentRow = 0;
}

void MenuMultiboard::Update()
{
}

void MenuMultiboard::Draw( sf::RenderTarget &render )
{
    sf::RenderStates state;
    state.transform *= getTransform();

    title.setPosition( { header.getSize().x / 2.0f - title.getLocalBounds().width / 2.0f,
                         header.getSize().y / 2.0f - title.getLocalBounds().height + 5.0f + 3.0f } );

    render.draw( header, state );
    render.draw( title, state );

    background.setPosition( { 0.0f, header.getPosition().y + header.getSize().y } );

    render.draw( background, state );

    state.transform *= background.getTransform();

    for( auto i = 0u; i < table.size(); i++ )
    {
        row.setString( table[i].text );

        row.setPosition( { 16.0f, float( row.getCharacterSize() * i + ROW_Y_STRIDE ) } );

        row.setFillColor( table[i].color );

        render.draw( row, state );
    }
}
