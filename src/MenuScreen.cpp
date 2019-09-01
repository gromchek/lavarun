#include "MenuHandler.h"
#include "MenuItem.h"
#include "MenuScreen.h"
#include "UserInput.h"
#include "Game.h"

#include "ResourceManager.h"

#include "FontStyles.h"

#include <SFML/Graphics/RenderTarget.hpp>

MenuScreen::MenuScreen( MenuHandler &handler ) : MenuWidget( handler )
{
    AddEvent( WidgetEvent::SCROLL_UP, [this]() { SetFocusIndex( GetFocusIndex() - 1 ); } );
    AddEvent( WidgetEvent::SCROLL_DOWN, [this]() { SetFocusIndex( GetFocusIndex() + 1 ); } );
}

void MenuScreen::ShowScreen()
{
	SetFocusIndex( 0 );
    GetFocusedWidget()->SetState( WidgetState::SELECTED );
}

void MenuScreen::HideScreen()
{
	SetFocusIndex( 0 );
    GetFocusedWidget()->SetState( WidgetState::SELECTED );
}

void MenuScreen::Draw( sf::RenderTarget &render )
{
    const auto &renderSize = render.getView().getSize();

	const auto xCenterScreen = renderSize.x / 2.0f;
	const auto yCenterScreen = renderSize.y / 2.0f;

    screenTitle.setPosition( { xCenterScreen - screenTitle.getLocalBounds().width / 2.0f, yCenterScreen / 2.0f } );

    render.draw( screenTitle );

    const auto yWidgetsSize = MENUITEM_DEFAULT_SIZE.y * items.size();

    for( auto i = 0u; i < widgets.size(); i++ )
    {
        const auto xStride = xCenterScreen - MENUITEM_DEFAULT_SIZE.x / 2.0f;
        const auto yStride = yCenterScreen - yWidgetsSize / 2.0f;

        GetWidget( i )->setPosition( { xStride, MENUITEM_DEFAULT_SIZE.y * i + yStride } );

        GetWidget( i )->Draw( render );
    }
}

void MenuScreen::Update()
{
    for( auto &&w : widgets )
    {
        w->Update();
    }
}

void MenuScreen::HandleEvent( WidgetEvent type )
{
    GetWidget( GetFocusIndex() )->GetEvent( type )();

    GetEvent( type )();
}

MenuScreen_MainMenu::MenuScreen_MainMenu( MenuHandler &handler ) : MenuScreen( handler )
{
	items.push_back( std::make_shared<MenuItem>( handler, "New Game" ) );
	items.push_back( std::make_shared<MenuItem>( handler, "Statistic" ) );
	items.push_back( std::make_shared<MenuItem>( handler, "Exit" ) );

	int index = 0;
	AddWidget( items[index] );
    GetWidget( index )->AddEvent( WidgetEvent::PRESS,
                                  [this]() { menu.SetNextScreen( int( MenuScreenWidget::NEWGAME ) ); } );
    GetWidget( index )->SetState( WidgetState::SELECTED );

	index += 1;
	AddWidget( items[index] );
    GetWidget( index )->AddEvent( WidgetEvent::PRESS,
                                  [this]() { menu.SetNextScreen( int( MenuScreenWidget::HIGHSCORE ) ); } );

	index += 1;
	AddWidget( items[index] );
    GetWidget( index )->AddEvent( WidgetEvent::PRESS, [this]() { HandleExitGame(); } );
}

void MenuScreen_MainMenu::HandleExitGame()
{
	game.Quit();
}

MenuScreen_NewGame::MenuScreen_NewGame( MenuHandler &handler ) : MenuScreen( handler )
{
    screenTitle.setString( "Game mode" );

    ApplyFontStyle( screenTitle, GAME_MENU_SCREENTITLE_FONT_NORMAL );

    items.push_back( std::make_shared<MenuItem>( handler, game.GetMode().GetName( ModeType::FREE ) ) );
    items.push_back( std::make_shared<MenuItem>( handler, game.GetMode().GetName( ModeType::NORMAL ) ) );
    items.push_back( std::make_shared<MenuItem>( handler, game.GetMode().GetName( ModeType::REVERSE ) ) );
    items.push_back( std::make_shared<MenuItem>( handler, "Back to menu" ) );

	int index = 0;
	AddWidget( items[index] );
    GetWidget( index )->AddEvent( WidgetEvent::PRESS, [this]() {
        menu.SetNextScreen( int( MenuScreenWidget::INVALID ) );
        HandleNewGame( ModeType::FREE );
    } );

	index += 1;
	AddWidget( items[index] );
    GetWidget( index )->AddEvent( WidgetEvent::PRESS, [this]() {
        menu.SetNextScreen( int( MenuScreenWidget::INVALID ) );
        HandleNewGame( ModeType::NORMAL );
    } );

	index += 1;
	AddWidget( items[index] );
    GetWidget( index )->AddEvent( WidgetEvent::PRESS, [this]() {
        menu.SetNextScreen( int( MenuScreenWidget::INVALID ) );
        HandleNewGame( ModeType::REVERSE );
    } );

	index += 1;
	AddWidget( items[index] );
    GetWidget( index )->AddEvent( WidgetEvent::PRESS,
                                  [this]() { menu.SetNextScreen( int( MenuScreenWidget::MAIN ) ); } );

    AddEvent( WidgetEvent::GO_BACK, [this]() { menu.SetNextScreen( int( MenuScreenWidget::MAIN ) ); } );
}

void MenuScreen_NewGame::HandleNewGame( const ModeType type )
{
	game.MenuHide();

    game.StartNewGame( type );
}

void MenuScreen_Highscore::loadZeroResult( std::shared_ptr<MenuMultiboard> table, const ModeType type )
{
    table->SetTitle( game.GetMode().GetName( type ) + " (N/A)" );

    char buff[32];

    for( auto i = 0u; i < GAME_MAX_ROUNDS; i++ )
    {
        std::fill_n( buff, 32, 0 );
        snprintf( buff, sizeof( buff ), "#%3u\tN/A sec", i + 1 );
        table->AddRow( { buff } );
    }

    table->AddRow( { "" } );

    table->AddRow( "Best: N/A" );
}

void MenuScreen_Highscore::loadData( std::shared_ptr<MenuMultiboard> table, const ModeType type )
{
    table->Clear();

    const auto &stats = game.gameStats.GetStatistic( type );

    if( !stats.lastPlayedTimestamp )
    {
        loadZeroResult( table, type );

        return;
    }

    auto *time1 = std::localtime( (const time_t *)&stats.lastPlayedTimestamp );

    char buff[32];
    strftime( buff, sizeof( buff ), " (%d/%m/%y)", time1 );

    table->SetTitle( game.GetMode().GetName( type ) + std::string( buff ) );

    for( auto i = 0u; i < GAME_MAX_ROUNDS; i++ )
    {
        std::fill_n( buff, 32, 0 );
        snprintf( buff, sizeof( buff ), "#%3u %8.3f sec", i + 1, stats.lastPlayedScore[i] / 1000.0f );
        table->AddRow( { buff } );
    }

    table->AddRow( { "" } );

    std::fill_n( buff, 32, 0 );
    snprintf( buff, sizeof( buff ), "Best: %8.3f", stats.bestPlayedScore / 1000.0f );

    std::string bestResult( buff );

    std::fill_n( buff, 32, 0 );
    time1 = std::localtime( (const time_t *)&stats.bestPlayedTimestamp );
    strftime( buff, sizeof( buff ), " (%d/%m/%y)", time1 );

    bestResult.append( buff );

    table->AddRow( std::move( bestResult ) );
}

MenuScreen_Highscore::MenuScreen_Highscore( MenuHandler &handler ) : MenuScreen( handler )
{
    screenTitle.setString( "Statistic" );
    ApplyFontStyle( screenTitle, GAME_MENU_SCREENTITLE_FONT_NORMAL );

    normalModeStats = std::make_shared<MenuMultiboard>( handler, 3 );
    normalModeStats->setPosition( { R_VIRTUAL_WIDTH * 0.15f, R_VIRTUAL_HEIGHT * 0.4f } );

    reverseModeStats = std::make_shared<MenuMultiboard>( handler, 3 );
    reverseModeStats->setPosition( { R_VIRTUAL_WIDTH * 0.55f, R_VIRTUAL_HEIGHT * 0.4f } );

    AddWidget( normalModeStats );
    AddWidget( reverseModeStats );

    AddEvent( WidgetEvent::GO_BACK, [this]() { menu.SetNextScreen( int( MenuScreenWidget::MAIN ) ); } );
}

void MenuScreen_Highscore::ShowScreen()
{
    loadData( normalModeStats, ModeType::NORMAL );
    loadData( reverseModeStats, ModeType::REVERSE );
}

void MenuScreen_Highscore::Draw( sf::RenderTarget &render )
{
    const auto &renderSize = render.getView().getSize();

    const auto xCenterScreen = renderSize.x / 2.0f;
    const auto yCenterScreen = renderSize.y / 2.0f;

    screenTitle.setPosition( { xCenterScreen - screenTitle.getLocalBounds().width / 2.0f, yCenterScreen / 2.0f } );

    render.draw( screenTitle );

    for( auto &w : widgets )
    {
        w->Draw( render );
    }
}

MenuScreen_HUD::MenuScreen_HUD( MenuHandler &handler ) : MenuScreen( handler )
{
    actionBar = std::make_shared<ActionBar>( handler );
    actionBar->setPosition( { R_VIRTUAL_WIDTH * 0.5f - ACTIONBAR_DEFAULT_SIZE.x / 2.0f,
                              R_VIRTUAL_HEIGHT - ACTIONBAR_DEFAULT_SIZE.y * 1.25f } );

    progressBarWidget = std::make_shared<MenuProgressBar>( handler );
    progressBarWidget->setPosition( R_VIRTUAL_WIDTH * 0.5f - PROGRESS_BAR_DEFAULT_SIZE.x / 2.0f,
                                    actionBar->getPosition().y - PROGRESS_BAR_DEFAULT_SIZE.y * 2.0f );
    progressBarWidget->SetState( WidgetState::HIDDEN );

    errorWidget = std::make_shared<MenuMessage>( handler, MessageType::ERROR );
    errorWidget->setPosition( { R_VIRTUAL_WIDTH * 0.5f, 0.0f } );

    messageWidget = std::make_shared<MenuMessage>( handler, MessageType::NOTIFY );
    messageWidget->setPosition( { R_VIRTUAL_WIDTH * 0.125f, R_VIRTUAL_HEIGHT * 0.75f } );

    multiboard = std::make_shared<MenuMultiboard>( handler );
    multiboard->setPosition( { 6.0f, 6.0f } );

    warmupWidget = std::make_shared<MenuNotice>( handler );
    warmupWidget->setPosition( { R_VIRTUAL_WIDTH * 0.5f, R_VIRTUAL_HEIGHT * 0.5f } );

    AddWidget( actionBar );
    AddWidget( progressBarWidget );
    AddWidget( errorWidget );
    AddWidget( messageWidget );
    AddWidget( multiboard );
    AddWidget( warmupWidget );
}

void MenuScreen_HUD::AddErrorMessage( std::string &&message )
{
    errorWidget->AddMessage( std::move( message ) );
}

void MenuScreen_HUD::AddMessage( std::string &&message )
{
    messageWidget->AddMessage( std::move( message ) );
}

void MenuScreen_HUD::UpdateCountdownTimer( const int startTime, const int endTime )
{
    progressBarWidget->SetState( WidgetState::NORMAL );
    progressBarWidget->SetBarDirection( MenuProgressBarDir::DEC );

    const float value = ( endTime - startTime ) / 1000.0f;
    const float maxVal = ( endTime - startTime ) / 1000.0f;
    progressBarWidget->SetMinMaxValue( 0.0f, maxVal );
    progressBarWidget->SetValue( value );

    progressBarWidget->SetBarText( "Remaining time: " );
}

void MenuScreen_HUD::UpdateRunnerTimer( const int startTime, const int endTime )
{
    progressBarWidget->SetState( WidgetState::NORMAL );
    progressBarWidget->SetBarDirection( MenuProgressBarDir::INC );

    const float value = 0.0f;
    const float maxVal = ( endTime - startTime ) / 1000.0f;
    progressBarWidget->SetMinMaxValue( 0.0f, maxVal );
    progressBarWidget->SetValue( value );

    progressBarWidget->SetBarText( "Time passed: " );
}

void MenuScreen_HUD::ShowWarmupText( const bool show, std::string &&message )
{
    if( !show )
    {
        warmupWidget->AddMessage( "" );

        return;
    }

    warmupWidget->AddMessage( std::move( message ) );
}

void MenuScreen_HUD::UpdateMultiboardTitle( std::string &&str )
{
    multiboard->SetTitle( std::move( str ) );
}

void MenuScreen_HUD::AddMultiboardRow( std::string &&str )
{
    multiboard->AddRow( std::move( str ) );
}

void MenuScreen_HUD::ClearMultiboardRowsColor()
{
    multiboard->ClearRowsColor();
}

void MenuScreen_HUD::SetMultiboardRowColor( const unsigned int row, const sf::Color color )
{
    multiboard->SetRowColor( row, color );
}

void MenuScreen_HUD::ShowScreen()
{
    actionBar->ClearActions();

    switch( game.GetMode().GetModeParams().type )
    {
    case ModeType::FREE:
        actionBar->AddAction( 0, "cmdBlock" );
        actionBar->AddAction( 1, "cmdUnblock" );
        actionBar->AddAction( 2, "cmdSlow" );
        actionBar->AddAction( 3, "cmdSpeed" );
        actionBar->AddAction( 4, "cmdReady" );
        actionBar->AddAction( 5, "cmdClearMap" );
        actionBar->AddAction( 6, "cmdGenBlocks" );
        actionBar->AddAction( 7, "cmdShowRoute" );
        break;
    case ModeType::NORMAL:
        actionBar->AddAction( 0, "cmdBlock" );
        actionBar->AddAction( 1, "cmdUnblock" );
        actionBar->AddAction( 2, "cmdSlow" );
        break;
    case ModeType::REVERSE:
        actionBar->AddAction( 0, "cmdBlock" );
        actionBar->AddAction( 1, "cmdUnblock" );
        actionBar->AddAction( 3, "cmdSpeed" );
        break;
    default:
        break;
    }

    errorWidget->Clear();
    messageWidget->Clear();
    multiboard->Clear();
    warmupWidget->Clear();

    progressBarWidget->SetState( WidgetState::HIDDEN );
}

void MenuScreen_HUD::HideScreen()
{
    progressBarWidget->SetState( WidgetState::HIDDEN );
}

void MenuScreen_HUD::Draw( sf::RenderTarget &render )
{
    for( auto &w : widgets )
    {
        w->Draw( render );
    }
}
