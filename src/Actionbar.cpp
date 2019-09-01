#include <cassert>

#include "Actionbar.h"

#include "Game.h"
#include "UserInput.h"
#include "KeyBinds.h"

#include "ResourceManager.h"

#include "FontStyles.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

constexpr size_t NUM_ACTION_BUTTONS = 8;
constexpr float ACTIONBUTTON_X_STRIDE = 16.0f;

const sf::Vector2f ACTIONBUTTON_DEFAULT_SIZE( 64.0f, 64.0f );

constexpr int MS_TO_FLOAT_FORMAT = 1000;

const sf::Color ACTIONBUTTON_NORMAL_COLOR( sf::Color::White );
const sf::Color ACTIONBUTTON_DISABLE_COLOR( 102, 102, 102 );

ActionBar::ActionBar( MenuHandler &handler ) :
    MenuWidget( handler ),
    background( { ACTIONBAR_DEFAULT_SIZE.x + 28.0f, ACTIONBAR_DEFAULT_SIZE.y + 28.0f },
                &resourceManager.GetTextureByName( "ab-background" ) )
{
    buttons.reserve( NUM_ACTION_BUTTONS );

    for( auto i = 0u; i < NUM_ACTION_BUTTONS; i++ )
    {
        char buf[16];
        snprintf( buf, sizeof( buf ), "actionbtn%u", i );
        buttons.push_back( std::make_shared<ActionButton>( handler, std::string( { buf } ) ) );

        buttons.back()->setPosition( { i * ( ACTIONBUTTON_X_STRIDE + ACTIONBUTTON_DEFAULT_SIZE.x ), 0.0f } );

        AddWidget( buttons[i] );
    }
}

void ActionBar::ClearActions()
{
    for( auto &&btn : buttons )
	{
		btn->SetAction( "" );
	}
}

void ActionBar::AddAction( const std::size_t slot, std::string &&action )
{
	assert( slot < NUM_ACTION_BUTTONS );

    buttons[slot]->SetAction( std::move( action ) );
}

void ActionBar::Update()
{
	for( auto &&button : buttons )
	{
		button->Update();

        if( userInput.GetKeyState( keyBinds.GetBind( button->GetName() ) ) )
        {
            actionManager.UseAction( button->GetAction() );
		}
	}
}

void ActionBar::Draw( sf::RenderTarget &render )
{
    sf::RenderStates state;

    state.transform *= getTransform();

    background.setPosition( { -14.0f, -14.0f } );
    render.draw( background, state );

    for( auto &&button : buttons )
    {
        render.draw( *button, state );
    }
}

void ActionButton::updateTexture( const ActionInfo &info )
{
    resourceManager.ValidateTextureIndex( borderTexIndex, "ab-border" );

    border.setTexture( &resourceManager.GetTextureByIndex( borderTexIndex ), true );
    border.setSize( ACTIONBUTTON_DEFAULT_SIZE );

	if( action.empty() )
	{
        resourceManager.ValidateTextureIndex( iconTexIndex, "ab-freeslot" );

        icon.setTexture( &resourceManager.GetTextureByIndex( iconTexIndex ), true );
        icon.setSize( ACTIONBUTTON_DEFAULT_SIZE );

		return;
	}

    resourceManager.ValidateTextureIndex( iconTexIndex, info.texName );

    icon.setTexture( &resourceManager.GetTextureByIndex( iconTexIndex ) );
    icon.setSize( ACTIONBUTTON_DEFAULT_SIZE );
	icon.setTextureRect( info.texCoord );
}

void ActionButton::updateHotkey()
{
    const auto scancode = keyBinds.GetBind( name );

    hotkey_text.setString( std::string( keyBinds.KeyNameToString( scancode ) ) );

    hotkey_text.setOrigin( { hotkey_text.getLocalBounds().width / 2.0f, hotkey_text.getLocalBounds().height / 2.0f } );

    auto pos = icon.getPosition();
    pos.x += icon.getLocalBounds().width - hotkey_text.getCharacterSize() / 2.0f;
    pos.y += hotkey_text.getCharacterSize() / 3.0f;

    hotkey_text.setPosition( pos );
}

void ActionButton::updateCooldown()
{
    if( action.empty() )
	{
		return;
	}

    const auto cd = actionManager.GetActionCooldown( action );

    if( !cd.start && !cd.duration )
    {
        cooldown.SetCooldown( 0, 0 );
        return;
    }

    cooldown.SetCooldown( cd.start, cd.duration );
    cooldown.Update();
}


ActionButton::ActionButton( MenuHandler &handler, std::string &&internalName ) :
    MenuWidget( handler ),
    name( std::move( internalName ) ),
    cooldown( ACTIONBUTTON_DEFAULT_SIZE.x ),
    action( "" )
{
    ApplyFontStyle( hotkey_text, GAME_HOTKEY_FONT_NORMAL );
}

void ActionButton::Update()
{
    const auto &info = actionManager.GetActionInfo( action );
	updateTexture( info );
    updateHotkey();
    updateCooldown();
}

void ActionButton::SetAction( std::string &&newAction )
{
    action = std::move( newAction );

    iconTexIndex = std::nullopt;
}

void ActionButton::draw( sf::RenderTarget &target, sf::RenderStates states ) const
{
    states.transform *= getTransform();

    target.draw( icon, states );
    target.draw( cooldown, states );
    target.draw( border, states );
    target.draw( hotkey_text, states );
}
