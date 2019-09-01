#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <memory>
#include <vector>
#include <string>
#include <functional>
#include <array>
#include <SFML/Graphics/Transformable.hpp>

namespace sf
{
class RenderTarget;
}

enum class MenuScreenWidget
{
	INVALID = -1,
	MAIN = 0,
	NEWGAME,
	HIGHSCORE,
	COUNT
};

enum class HUDScreenWidget
{
    INVALID = -1,
    MAIN = 0,
    COUNT
};


enum class WidgetEvent
{
    NONE = 0,
	GO_BACK,
	PRESS,
	SCROLL_UP,
    SCROLL_DOWN,
    COUNT
};

enum class WidgetState
{
    NORMAL = 0,
    SELECTED,
    DISABLED,
    HIDDEN
};

using WidgetAction = std::function<void()>;

class MenuWidget;
using MenuWidgetPtr = std::shared_ptr<MenuWidget>;


class MenuHandler;

class MenuWidget : public sf::Transformable
{
private:
    unsigned int focusIndex;
    WidgetState state;

public:
    explicit MenuWidget( MenuHandler &handler );
	virtual ~MenuWidget() = default;

	virtual void Draw( sf::RenderTarget &render ) = 0;
    virtual void Update() = 0;

    virtual void HandleEvent( const WidgetEvent ev );

    void SetState( const WidgetState widgetState );
    WidgetState GetState() const;

    void SetFocusIndex( int index );
    unsigned int GetFocusIndex() const;
    MenuWidgetPtr GetFocusedWidget() const;

    void AddEvent( WidgetEvent type, WidgetAction &&action );
    const WidgetAction &GetEvent( WidgetEvent evType ) const;
	void ClearEvents();

    void AddWidget( MenuWidgetPtr widget );
    MenuWidgetPtr GetWidget( const unsigned int index ) const;

protected:
	std::vector<MenuWidgetPtr> widgets;
    std::array<WidgetAction, int( WidgetEvent::COUNT )> actions;
    MenuHandler &menu;
};

inline void MenuWidget::HandleEvent( [[maybe_unused]] const WidgetEvent ev )
{
}

inline void MenuWidget::SetState( const WidgetState widgetState )
{
    state = widgetState;
}

inline WidgetState MenuWidget::GetState() const
{
    return state;
}


#endif // MENUWIDGET_H
