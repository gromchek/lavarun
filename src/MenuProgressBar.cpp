#include "MenuProgressBar.h"
#include "ResourceManager.h"
#include "FontStyles.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "Game.h"

const unsigned char PROGRESS_BAR_FADEOUT_STEP = 8;

void MenuProgressBar::updateIncrease()
{
    val += game.time / 1000.0f;

    if( val >= max )
    {
        SetValue( max );
        fadeOut = true;
    }

    const float width = val / max;

    if( width <= 1.0f )
    {
        gradient.setSize( { PROGRESS_BAR_DEFAULT_SIZE.x * width, PROGRESS_BAR_DEFAULT_SIZE.y } );
    }

    resourceManager.ValidateTextureIndex( gradientTexIndex, "pb-grad-blue" );

    gradient.setTexture( &resourceManager.GetTextureByIndex( gradientTexIndex ) );
}

void MenuProgressBar::updateDecrease()
{
    val -= game.time / 1000.0f;

    if( val <= 0.0f )
    {
        SetValue( 0.0f );
        fadeOut = true;
    }

    const float width = val / max;

    if( width >= 0.0f )
    {
        gradient.setSize( { PROGRESS_BAR_DEFAULT_SIZE.x * width, PROGRESS_BAR_DEFAULT_SIZE.y } );
    }

    resourceManager.ValidateTextureIndex( gradientTexIndex, "pb-grad-green" );

    gradient.setTexture( &resourceManager.GetTextureByIndex( gradientTexIndex ) );
}

void MenuProgressBar::updateFadeOut()
{
    if( !fadeOut )
    {
        return;
    }

    const auto &fillColor = gradient.getFillColor();

    int alpha = fillColor.a;
    alpha -= PROGRESS_BAR_FADEOUT_STEP;

    const sf::Color fadeOutColor( { fillColor.r, fillColor.g, fillColor.b, (unsigned char)alpha } );

    if( alpha > 0 )
    {
        gradient.setFillColor( fadeOutColor );
        border.setFillColor( fadeOutColor );
        label.setFillColor( fadeOutColor );
    }
    else
    {
        gradient.setFillColor( sf::Color::White );
        border.setFillColor( sf::Color::White );
        label.setFillColor( GAME_PROGRESSBAR_FONT_NORMAL.fillColor );

        SetState( WidgetState::HIDDEN );
        fadeOut = false;
    }
}

MenuProgressBar::MenuProgressBar( MenuHandler &handler ) :
    MenuWidget( handler ),
    border( PROGRESS_BAR_DEFAULT_SIZE, &resourceManager.GetTextureByName( "pb-border" ) ),
    dir( MenuProgressBarDir::INC ),
    text( "" ),
    val( 0.0f ),
    min( 0.0f ),
    max( 0.0f ),
    fadeOut( false )
{
    gradient.setSize( PROGRESS_BAR_DEFAULT_SIZE );

    ApplyFontStyle( label, GAME_PROGRESSBAR_FONT_NORMAL );
}

void MenuProgressBar::SetBarText( std::string &&str )
{
    text = std::move( str );
}

void MenuProgressBar::SetMinMaxValue( const float minVal, const float maxVal )
{
    min = minVal;
    max = maxVal;

    gradient.setFillColor( sf::Color::White );
    border.setFillColor( sf::Color::White );
    label.setFillColor( GAME_PROGRESSBAR_FONT_NORMAL.fillColor );

    fadeOut = false;
}

void MenuProgressBar::Update()
{
    if( GetState() == WidgetState::HIDDEN )
    {
        return;
    }

    label.setString( text + std::to_string( int( val ) ) + " sec." );
    label.setOrigin( { label.getLocalBounds().width / 2.0f, label.getLocalBounds().height / 2.0f } );
    label.setPosition( { PROGRESS_BAR_DEFAULT_SIZE.x / 2.0f, PROGRESS_BAR_DEFAULT_SIZE.y / 3.0f } );

    if( dir == MenuProgressBarDir::INC )
    {
        updateIncrease();
    }
    else
    {
        updateDecrease();
    }

    updateFadeOut();
}

void MenuProgressBar::Draw( sf::RenderTarget &render )
{
    if( GetState() == WidgetState::HIDDEN )
    {
        return;
    }

    sf::RenderStates state;

    state.transform *= getTransform();

    render.draw( gradient, state );
    render.draw( label, state );
    render.draw( border, state );
}
