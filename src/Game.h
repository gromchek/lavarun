#ifndef GAME_H
#define GAME_H

#include "MenuHandler.h"
#include "GameMode.h"
#include "Actionbar.h"
#include "Tilemap.h"
#include "Rnd.h"
#include "Pathfind.h"
#include "Statistic.h"

#include "Entity.h"

#include <memory>

namespace sf
{
class RenderTarget;
}

class Game
{
private:
	bool running;
    ModeType currentMode;
    Runner runner;
    unsigned int currentPathTile;
    Route route;
    bool drawRoute;
    sf::VertexArray routeVerts;
	bool inGame;

    unsigned int frameNum;

    void generateMap( const TileItem itemType );

    bool cmdBlock_f();
    bool cmdUnblock_f();
    bool cmdSlow_f();
    bool cmdSpeed_f();
    bool cmdReady_f();
    bool cmdStartRound_f();
    bool cmdGenBlocks_f();
    bool cmdClearMap_f();
    bool cmdShowRoute_f();
    bool cmdGenMap_f();
    bool cmdSaveSession_f();


    void drawPath( sf::RenderTarget &render );
    void runnerTouchItem( const Route &route );

    std::shared_ptr<MenuHandler_Menu> menuHandler;
    std::shared_ptr<MenuHandler_HUD> hudHandler;
    std::shared_ptr<MenuScreen_HUD> hud;

    Pathfind pathFind;
    Tilemap tilemap;
    Random random;

    GameMode gameMode;

public:
    int thisTime;
    int time;
    int oldTime;

    Statistic gameStats;

	Game();
	~Game() = default;

	void Init();

    void StartNewGame( const ModeType newMode );
	void RestartGame();
	void Quit();

	void MenuHide();
	void MenuShow();

	void HandleEvents();
	void Frame( sf::RenderTarget &render );

	bool IsInGame() const;
	bool IsRunning() const;

    const GameMode &GetMode() const;
};

inline Game::Game() :
    running( true ),
    currentMode( ModeType::FREE ),
    currentPathTile( -1 ),
    drawRoute( false ),
    inGame( false ),
    frameNum( 0 ),
    pathFind( tilemap.ConvertTileIndex( TILEMAP_START_TILE ), tilemap.ConvertTileIndex( TILEMAP_END_TILE ) ),
    thisTime( 0 ),
    time( 0 ),
    oldTime( 0 )
{
    route.reserve( 32 );

    routeVerts.setPrimitiveType( sf::PrimitiveType::Quads );
    routeVerts.resize( TILEMAP_TILES_NUM * 4 );
}

inline void Game::MenuHide()
{
	menuHandler->Show( false );
}

inline void Game::MenuShow()
{
	menuHandler->Show( true );
}

inline bool Game::IsInGame() const
{
	return inGame;
}

inline void Game::Quit()
{
	running = false;
}

inline bool Game::IsRunning() const
{
	return running;
}

extern Game game;

#endif // GAME_H
