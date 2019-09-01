#include "MenuItem.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "ResourceManager.h"

#include "FontStyles.h"

const sf::Color MENUITEM_SELECT_COLOR( sf::Color::White );
const sf::Color MENUITEM_NORMAL_COLOR( 205, 205, 205 );
const sf::Color MENUITEM_UNSELECT_COLOR( 127, 127, 127 );

MenuItem::MenuItem( MenuHandler &handler, const std::string &text ) :
    MenuWidget( handler ),
    patch( MENUITEM_DEFAULT_SIZE, &resourceManager.GetTextureByName( "btn-texture" ) )
{
    btnGlow.setSize( MENUITEM_DEFAULT_SIZE );

    btnGlow.setFillColor( sf::Color::Transparent );

    label.setString( text );

    ApplyFontStyle( label, GAME_MENU_FONT_NORMAL );
}

void MenuItem::Draw( sf::RenderTarget &render )
{
    if( GetState() == WidgetState::HIDDEN )
    {
        return;
    }

    render.draw( patch, { getTransform() } );
    render.draw( btnGlow, { getTransform() } );

    label.setOrigin( { label.getLocalBounds().width / 2.0f, label.getLocalBounds().height / 2.0f } );

    auto pos = patch.getPosition();
    pos.x += patch.getSize().x / 2.0f;
    pos.y += patch.getSize().y / 2.0f;

    pos.y -= label.getLocalBounds().height / 2.5f;

    label.setPosition( pos );

    render.draw( label, { getTransform() } );
}

void MenuItem::Update()
{
    resourceManager.ValidateTextureIndex( btnGlowTexIndex, "btn-glow" );

    switch( GetState() )
    {
    case WidgetState::NORMAL:
        btnGlow.setTexture( nullptr );
        btnGlow.setFillColor( sf::Color::Transparent );
        label.setFillColor( GAME_MENU_FONT_NORMAL.fillColor );
        patch.setFillColor( MENUITEM_NORMAL_COLOR );
        break;
    case WidgetState::SELECTED:
        btnGlow.setTexture( &resourceManager.GetTextureByIndex( btnGlowTexIndex ), true );
        btnGlow.setFillColor( MENUITEM_SELECT_COLOR );
        label.setFillColor( GAME_MENU_FONT_HIGHLIGHT.fillColor );
        patch.setFillColor( MENUITEM_SELECT_COLOR );
        break;
    case WidgetState::DISABLED:
        btnGlow.setTexture( nullptr );
        label.setFillColor( GAME_MENU_FONT_DISABLE.fillColor );
        patch.setFillColor( MENUITEM_UNSELECT_COLOR );
        break;
    case WidgetState::HIDDEN:
        // skip draw
        break;
    default:
        break;
    }
}
