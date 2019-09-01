#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include "UserInput.h"
#include "Game.h"
#include "Misc.h"
#include "KeyBinds.h"
#include "ResourceManager.h"

void TakeScreenshot( const sf::RenderWindow &win, const unsigned int time );
void ProcessEvents( sf::RenderWindow &win );

int main()
{
	auto displayMode = ( R_FULLSCREEN ? sf::Style::Fullscreen : sf::Style::Close );

	const auto &vidModes = sf::VideoMode::getFullscreenModes();

	sf::RenderWindow window( vidModes[0], "Lava block", displayMode );
	window.setVerticalSyncEnabled( true );
	window.setKeyRepeatEnabled( false );
    window.setMouseCursorVisible( false );

	sf::View mainView;
	mainView.reset( { 0.0f, 0.0f, R_VIRTUAL_WIDTH, R_VIRTUAL_HEIGHT } );
	window.setView( mainView );

    sf::Clock clock;

	keyBinds.ReadBinds( "base//default.txt" );

	resourceManager.Init();
    resourceManager.LoadTexture( "spelldata" );
	resourceManager.LoadTexture( "ab-border" );
    resourceManager.LoadTexture( "ab-freeslot" );
    resourceManager.LoadTexture( "ab-background" );

    resourceManager.LoadTexture( "world" );
    resourceManager.LoadTexture( "items" );

    resourceManager.LoadTexture( "cursor" );
    resourceManager.LoadTexture( "btn-glow" );

    resourceManager.LoadTexture( "pb-border" );
    resourceManager.LoadTexture( "pb-grad-blue" );
    resourceManager.LoadTexture( "pb-grad-green" );

    resourceManager.LoadTexture( "dialog-box" );
    resourceManager.LoadTexture( "dialog-header" );
    resourceManager.LoadTexture( "btn-texture" );

    resourceManager.LoadTexture( "runner" );

    resourceManager.LoadFont( "alegreya" );
    resourceManager.LoadFont( "alegreya-reg" );

	game.Init();

	game.MenuShow();

    game.thisTime = clock.getElapsedTime().asMilliseconds();

    sf::RectangleShape cursor;
    cursor.setTexture( &resourceManager.GetTextureByName( "cursor" ), true );
    cursor.setSize( { 32.0f, 32.0f } );

    game.gameStats.ReadStats( "base//save.dat" );

	while( game.IsRunning() )
	{
        game.thisTime = clock.getElapsedTime().asMilliseconds();
        game.time = game.thisTime - game.oldTime;

		ProcessEvents( window );

		game.HandleEvents();

        cursor.setPosition( static_cast<sf::Vector2f>( sf::Mouse::getPosition( window ) ) );

		window.clear();

		game.Frame( window );

        window.draw( cursor );

		window.display();

        game.oldTime = game.thisTime;
	}

    game.gameStats.WriteStats( "base//save.dat" );

	keyBinds.WriteBinds( "base//default.txt" );

	return 0;
}

void ProcessEvents( sf::RenderWindow &win )
{
	userInput.ReleaseAllKey();

	sf::Event event;

	while( win.pollEvent( event ) )
	{
		if( event.type == sf::Event::Closed )
		{
			win.close();
			break;
		}

		switch( event.type )
		{
		case sf::Event::KeyPressed:
			userInput.SetKey( event.key.code, true );
			break;
		case sf::Event::KeyReleased:
			userInput.SetKey( event.key.code, false );
			break;
		case sf::Event::MouseButtonPressed:
			userInput.SetMouseKey( event.mouseButton.button, true );
			break;
		case sf::Event::MouseButtonReleased:
			userInput.SetMouseKey( event.mouseButton.button, false );
			break;
		default:
			break;
		}

        if( event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F12 )
        {
            TakeScreenshot( win, game.thisTime );
        }
	}

    userInput.SetMousePosition( sf::Mouse::getPosition( win ) );
}


void TakeScreenshot( const sf::RenderWindow &win, const unsigned int time )
{
    std::string fileName( "scr" );
    fileName.append( std::to_string( time ) );
    fileName.append( ".png" );

    sf::Texture texture;
    texture.create( win.getSize().x, win.getSize().y );
    texture.update( win );

    texture.copyToImage().saveToFile( fileName );
}
