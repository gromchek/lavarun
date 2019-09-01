#include "MenuHandler.h"
#include "MenuScreen.h"
#include "UserInput.h"

#include "Misc.h"

#include <cassert>


void MenuHandler::Draw( sf::RenderTarget &render )
{
	if( !IsActive() )
	{
		return;
	}

	screens[current]->Draw( render );
}

MenuHandler_Menu::MenuHandler_Menu() : MenuHandler()
{
	screens.reserve( int( MenuScreenWidget::COUNT ) );

	screens.push_back( std::make_shared<MenuScreen_MainMenu>( *this ) );
	screens.push_back( std::make_shared<MenuScreen_NewGame>( *this ) );
	screens.push_back( std::make_shared<MenuScreen_Highscore>( *this ) );
}

void MenuHandler_Menu::Update()
{
	if( current != next )
	{
        // assert( next > int( MenuScreenWidget::INVALID ) || next < int( MenuScreenWidget::COUNT ) );

        if( inRange( MenuScreenWidget::INVALID, MenuScreenWidget::COUNT, current ) )
		{
			screens[current]->HideScreen();
		}

		current = next;

        if( inRange( MenuScreenWidget::INVALID, MenuScreenWidget::COUNT, current ) )
		{
			screens[current]->ShowScreen();
		}
	}

    if( current == int( MenuScreenWidget::INVALID ) )
	{
		return;
	}


    WidgetEvent evType = WidgetEvent::NONE;

	if( userInput.GetKeyState( sf::Keyboard::Down ) )
	{
        evType = WidgetEvent::SCROLL_DOWN;
	}
	if( userInput.GetKeyState( sf::Keyboard::Up ) )
	{
        evType = WidgetEvent::SCROLL_UP;
	}
	if( userInput.GetKeyState( sf::Keyboard::Escape ) )
	{
        evType = WidgetEvent::GO_BACK;
	}
	if( userInput.GetKeyState( sf::Keyboard::Return ) )
	{
        evType = WidgetEvent::PRESS;
	}

    screens[current]->Update();
	screens[current]->HandleEvent( evType );
}

void MenuHandler_Menu::Show( bool show )
{
	active = show;

	if( show )
	{
		current = int( MenuScreenWidget::INVALID );
		next = int( MenuScreenWidget::MAIN );
	}
	else
	{
		current = int( MenuScreenWidget::INVALID );
		next = int( MenuScreenWidget::INVALID );
	}
}

MenuHandler_HUD::MenuHandler_HUD() : MenuHandler()
{
    screens.reserve( int( HUDScreenWidget::COUNT ) );

    hudScreen = std::make_shared<MenuScreen_HUD>( *this );

    screens.push_back( hudScreen );
}

void MenuHandler_HUD::Update()
{
	if( current != next )
	{
        // assert( next > int( HUDScreenWidget::INVALID ) || next < int( HUDScreenWidget::COUNT ) );

        if( inRange( HUDScreenWidget::INVALID, HUDScreenWidget::COUNT, current ) )
		{
			screens[current]->HideScreen();
		}

		current = next;

        if( inRange( HUDScreenWidget::INVALID, HUDScreenWidget::COUNT, current ) )
		{
			screens[current]->ShowScreen();
		}
	}

	if( current == int( HUDScreenWidget::INVALID ) )
	{
		return;
	}


    WidgetEvent evType = WidgetEvent::NONE;

    screens[current]->Update();
	screens[current]->HandleEvent( evType );
}

void MenuHandler_HUD::Show( bool show )
{
	active = show;

	if( show )
	{
		current = int( HUDScreenWidget::INVALID );
		next = int( HUDScreenWidget::MAIN );
	}
	else
	{
		current = int( HUDScreenWidget::INVALID );
		next = int( HUDScreenWidget::INVALID );
	}
}
