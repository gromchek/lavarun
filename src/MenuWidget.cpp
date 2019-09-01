#include "MenuWidget.h"
#include <cassert>
#include <algorithm>

#include <SFML/Graphics/RenderTarget.hpp>

MenuWidget::MenuWidget( MenuHandler &handler ) : focusIndex( 0 ), state( WidgetState::NORMAL ), menu( handler )
{
	widgets.reserve( 8 );
    ClearEvents();
}

void MenuWidget::SetFocusIndex( int index )
{
    if( !widgets.size() )
	{
		return;
	}

    const int widgetsCount = widgets.size();

    if( index < 0 )
    {
        index = widgetsCount - 1;
    }
    if( index >= widgetsCount )
    {
        index = 0;
    }

    const auto oldIndex = focusIndex;

    if( GetWidget( index )->GetState() == WidgetState::DISABLED ||
        GetWidget( index )->GetState() == WidgetState::HIDDEN )
    {
        if( index < focusIndex )
        {
            index -= 1;
        }
        if( index > focusIndex )
        {
            index += 1;
        }
    }

	focusIndex = index;

    GetWidget( oldIndex )->SetState( WidgetState::NORMAL );
    GetWidget( focusIndex )->SetState( WidgetState::SELECTED );
}

unsigned int MenuWidget::GetFocusIndex() const
{
    return focusIndex;
}

MenuWidgetPtr MenuWidget::GetFocusedWidget() const
{
    return widgets[focusIndex];
}

void MenuWidget::AddEvent( WidgetEvent type, WidgetAction &&action )
{
    const int index = int( type );
    actions[index] = std::move( action );
}

const WidgetAction &MenuWidget::GetEvent( WidgetEvent evType ) const
{
    const int index = int( evType );
    return actions[index];
}

void MenuWidget::ClearEvents()
{
    for( auto &e : actions )
	{
        e = []() {};
	}
}

void MenuWidget::AddWidget( MenuWidgetPtr widget )
{
    if( std::find( widgets.begin(), widgets.end(), widget ) == widgets.end() )
    {
        widgets.push_back( widget );
    }
}

MenuWidgetPtr MenuWidget::GetWidget( const unsigned int index ) const
{
    assert( index < widgets.size() );
	return widgets[index];
}
