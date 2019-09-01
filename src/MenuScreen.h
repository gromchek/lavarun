#ifndef MENUSCREEN_H
#define MENUSCREEN_H

#include <vector>
#include <memory>

#include "MenuWidget.h"
#include "ActionBar.h"
#include "MenuProgressBar.h"
#include "MenuMessage.h"
#include "MenuMultiboard.h"
#include "MenuNotice.h"

#include <SFML/Graphics/Text.hpp>

class MenuItem;
using MenuItemPtr = std::shared_ptr<MenuItem>;

namespace sf
{
class RenderTarget;
}

class MenuScreen : public MenuWidget
{
private:
public:
    MenuScreen() = delete;
    explicit MenuScreen( MenuHandler &handler );
	virtual ~MenuScreen() = default;

	virtual void ShowScreen();
	virtual void HideScreen();

	virtual void Draw( sf::RenderTarget &render ) override;
    virtual void Update() override;

    virtual void HandleEvent( WidgetEvent type ) override;

protected:
    sf::Text screenTitle;
	std::vector<MenuItemPtr> items;
};

class MenuScreen_MainMenu final : public MenuScreen
{
private:
public:
    MenuScreen_MainMenu() = delete;
    explicit MenuScreen_MainMenu( MenuHandler &handler );
	virtual ~MenuScreen_MainMenu() = default;

	void HandleExitGame();
};

class MenuScreen_NewGame final : public MenuScreen
{
private:
public:
    MenuScreen_NewGame() = delete;
    explicit MenuScreen_NewGame( MenuHandler &handler );
	virtual ~MenuScreen_NewGame() = default;

    void HandleNewGame( const ModeType type );
};

class MenuScreen_Highscore final : public MenuScreen
{
private:
    std::shared_ptr<MenuMultiboard> normalModeStats;
    std::shared_ptr<MenuMultiboard> reverseModeStats;

    void loadZeroResult( std::shared_ptr<MenuMultiboard> table, const ModeType type );
    void loadData( std::shared_ptr<MenuMultiboard> table, const ModeType type );

public:
    MenuScreen_Highscore() = delete;
    explicit MenuScreen_Highscore( MenuHandler &handler );
	virtual ~MenuScreen_Highscore() = default;

    virtual void ShowScreen() override;

    virtual void Draw( sf::RenderTarget &render ) override;
};

class MenuScreen_HUD final : public MenuScreen
{
private:
    std::shared_ptr<ActionBar> actionBar;
    std::shared_ptr<MenuMessage> errorWidget;
    std::shared_ptr<MenuMessage> messageWidget;
    std::shared_ptr<MenuProgressBar> progressBarWidget;
    std::shared_ptr<MenuMultiboard> multiboard;
    std::shared_ptr<MenuNotice> warmupWidget;

public:
    MenuScreen_HUD() = delete;
    explicit MenuScreen_HUD( MenuHandler &handler );
	virtual ~MenuScreen_HUD() = default;

    void AddErrorMessage( std::string &&message );
    void AddMessage( std::string &&message );
    void UpdateCountdownTimer( const int startTime, const int endTime );
    void UpdateRunnerTimer( const int startTime, const int endTime );
    void ShowWarmupText( const bool show, std::string &&message );
    void UpdateMultiboardTitle( std::string &&str );
    void AddMultiboardRow( std::string &&str );
    void ClearMultiboardRowsColor();
    void SetMultiboardRowColor( const unsigned int row, const sf::Color color );

	virtual void ShowScreen() override;
	virtual void HideScreen() override;

	virtual void Draw( sf::RenderTarget &render ) override;
};

#endif // MENUSCREEN_H
