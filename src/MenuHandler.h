#ifndef MENUHANDLER_H
#define MENUHANDLER_H

#include "MenuScreen.h"
#include <vector>

namespace sf
{
class RenderTarget;
}

using MenuScreenPtr = std::shared_ptr<MenuScreen>;

class MenuHandler
{
private:
public:
	MenuHandler();
	virtual ~MenuHandler() = default;

	virtual void Update() = 0;
	void SetNextScreen( const int screen );

	void Draw( sf::RenderTarget &render );

	virtual void Show( bool show ) = 0;
	bool IsActive() const;

protected:
	bool active;

	int current;
	int next;

	std::vector<MenuScreenPtr> screens;
};

inline MenuHandler::MenuHandler() : active( false ), current( 0 ), next( 0 )
{
}

inline void MenuHandler::SetNextScreen( const int screen )
{
	next = screen;
}

inline bool MenuHandler::IsActive() const
{
	return active;
}

class MenuHandler_Menu final : public MenuHandler
{
private:
public:
	MenuHandler_Menu();
	virtual ~MenuHandler_Menu() = default;

	virtual void Update() override;

	virtual void Show( bool show ) override;
};


class MenuHandler_HUD final : public MenuHandler
{
private:
    std::shared_ptr<MenuScreen_HUD> hudScreen;

public:
	MenuHandler_HUD();
	virtual ~MenuHandler_HUD() = default;

    std::shared_ptr<MenuScreen_HUD> GetHUD() const;

	virtual void Update() override;

    virtual void Show( bool show ) override;
};

inline std::shared_ptr<MenuScreen_HUD> MenuHandler_HUD::GetHUD() const
{
    return hudScreen;
}


#endif // MENUHANDLER_H
