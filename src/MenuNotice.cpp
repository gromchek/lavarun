#include "MenuNotice.h"
#include "FontStyles.h"
#include <cmath>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

const float SCALE_FACTOR = 0.9f;
const float MIN_SCALE_FACTOR = 1.0f;

MenuNotice::MenuNotice( MenuHandler &handler ) : MenuWidget( handler )
{
    ApplyFontStyle( label, GAME_NOTICE_FONT_NORMAL );
}

void MenuNotice::AddMessage( std::string &&message )
{
    label.setString( std::move( message ) );
    label.setOrigin( { label.getLocalBounds().width / 2.0f, label.getLocalBounds().height / 2.0f } );
    label.setScale( { 2.0f, 2.0f } );
}

void MenuNotice::Clear()
{
    label.setString( "" );
}

void MenuNotice::Update()
{
    if( MIN_SCALE_FACTOR >= std::fabs( label.getScale().x ) )
    {
        label.setScale( { 1.0f, 1.0f } );

        return;
    }

    label.scale( { SCALE_FACTOR, SCALE_FACTOR } );
}

void MenuNotice::Draw( sf::RenderTarget &render )
{
    sf::RenderStates state;

    state.transform *= getTransform();

    label.setPosition( { 0.0f, -label.getLocalBounds().height * 2.0f } );

    render.draw( label, state );
}
