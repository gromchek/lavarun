#include "Game.h"
#include "UserInput.h"
#include <SFML/Graphics/RenderTarget.hpp>

void Game::generateMap( const TileItem itemType )
{
    tilemap.Clear();

    std::vector<int> areaCells;
    areaCells.reserve( 32 );

    for( auto i = 0u; i < TILEMAP_TILES_W_NUM; i += 2 )
    {
        for( auto j = 0u; j < TILEMAP_TILES_H_NUM; j += 2 )
        {
            const auto tile = tilemap.ConvertTileIndex( { int( i ), int( j ) } );

            if( tile == TILEMAP_START_TILE || tile == TILEMAP_END_TILE )
            {
                continue;
            }

            areaCells.push_back( tile );
        }
    }

    for( const auto &cell : areaCells )
    {
        int lastTile = cell;
        int errorPass = 2;

        for( int j = 0; j < random.GetInt( 3 ) && errorPass > 0; )
        {
            const auto nb = tilemap.Neighbors( lastTile );

            const int nIdx = random.GetInt() % nb.size();
            const int tile = tilemap.ConvertTileIndex( nb[nIdx] );

            if( tilemap[tile].isSolid )
            {
                errorPass--;
                continue;
            }

            tilemap[tile].isSolid = true;

            auto possibleRoute = pathFind.BuildPath( tilemap.GetTiles() );

            if( !possibleRoute )
            {
                tilemap[tile].isSolid = false;

                continue;
            }

            route = std::move( *possibleRoute );

            lastTile = tile;
            j++;
        }
    }

    if( itemType == TileItem::EMPTY )
    {
        return;
    }

    for( int i = 0; i < 3; )
    {
        const int idx = random.GetInt( route.size() );
        const int tile = tilemap.ConvertTileIndex( route[idx] );

        if( tile == TILEMAP_START_TILE || tile == TILEMAP_END_TILE )
        {
            continue;
        }

        if( tilemap[tile].isSolid || tilemap[tile].item == itemType )
        {
            continue;
        }

        const auto nbors = tilemap.Neighbors( route[idx], TileNeighborsDir::ALL, 3 );
        bool failNbor = false;

        for( const auto &nbor : nbors )
        {
            const int t = tilemap.ConvertTileIndex( nbor );

            if( t == TILEMAP_START_TILE || t == TILEMAP_END_TILE )
            {
                continue;
            }

            if( tilemap[t].isSolid )
            {
                continue;
            }

            if( tilemap[t].item == itemType )
            {
                failNbor = true;
                break;
            }
        }

        if( failNbor )
        {
            continue;
        }

        tilemap[tile].item = itemType;

        i++;
    }

    for( int i = 0; i < 2; )
    {
        const int tile = random.GetInt( TILEMAP_TILES_NUM );

        if( tile == TILEMAP_START_TILE || tile == TILEMAP_END_TILE )
        {
            continue;
        }

        if( tilemap[tile].isSolid || tilemap[tile].item == itemType )
        {
            continue;
        }

        const auto nbors = tilemap.Neighbors( tile, TileNeighborsDir::ALL, 3 );
        bool failNbor = false;

        for( const auto &nbor : nbors )
        {
            const int t = tilemap.ConvertTileIndex( nbor );

            if( t == TILEMAP_START_TILE || t == TILEMAP_END_TILE )
            {
                continue;
            }

            if( tilemap[t].isSolid )
            {
                continue;
            }

            if( tilemap[t].item == itemType )
            {
                failNbor = true;
                break;
            }
        }

        if( failNbor )
        {
            continue;
        }

        tilemap[tile].item = itemType;

        i++;
    }
}

bool Game::cmdBlock_f()
{
    if( !gameMode.PlayerCanUseAction() )
    {
        hud->AddErrorMessage( "You can't do that right now" );
        return false;
    }

    const auto tileIndex = tilemap.GetSelectedTileIndex();

    if( tileIndex > TILEMAP_TILES_NUM )
    {
        return false;
    }

    if( !tilemap[tileIndex].isSolid && tilemap[tileIndex].item == TileItem::EMPTY )
    {
        tilemap[tileIndex].isSolid = true;

        auto possibleRoute = pathFind.BuildPath( tilemap.GetTiles() );

        if( possibleRoute )
        {
            if( drawRoute )
            {
                route = std::move( *possibleRoute );
            }

            return true;
        }
        else
        {
            tilemap[tileIndex].isSolid = false;
            hud->AddErrorMessage( "Invalid target" );

            return false;
        }
    }

    hud->AddErrorMessage( "You must select an empty tile" );
    return false;
}

bool Game::cmdUnblock_f()
{
    if( !gameMode.PlayerCanUseAction() )
    {
        hud->AddErrorMessage( "You can't do that right now" );
        return false;
    }

    const auto tileIndex = tilemap.GetSelectedTileIndex();

    if( tileIndex > TILEMAP_TILES_NUM )
    {
        return false;
    }

    if( tilemap[tileIndex].isSolid )
    {
        tilemap[tileIndex].isSolid = false;

        if( drawRoute )
        {
            route = *pathFind.BuildPath( tilemap.GetTiles() );
        }

        return true;
    }

    hud->AddErrorMessage( "You must select a lava tile" );

    return false;
}

bool Game::cmdSlow_f()
{
    if( !gameMode.PlayerCanUseAction() )
    {
        hud->AddErrorMessage( "You can't do that right now" );
        return false;
    }

    const auto tileIndex = tilemap.GetSelectedTileIndex();

    if( tileIndex > TILEMAP_TILES_NUM )
    {
        return false;
    }

    if( tilemap[tileIndex].isSolid || tilemap[tileIndex].item != TileItem::EMPTY )
    {
        hud->AddErrorMessage( "You must select an empty tile" );
        return false;
    }

    if( tilemap[tileIndex].item == TileItem::EMPTY )
    {
        tilemap[tileIndex].item = TileItem::SLOW;

        return true;
    }

    return false;
}

bool Game::cmdSpeed_f()
{
    if( !gameMode.PlayerCanUseAction() )
    {
        hud->AddErrorMessage( "You can't do that right now" );
        return false;
    }

    const auto tileIndex = tilemap.GetSelectedTileIndex();

    if( tileIndex > TILEMAP_TILES_NUM )
    {
        return false;
    }

    if( tilemap[tileIndex].isSolid || tilemap[tileIndex].item != TileItem::EMPTY )
    {
        hud->AddErrorMessage( "You must select an empty tile" );
        return false;
    }

    if( tilemap[tileIndex].item == TileItem::EMPTY )
    {
        tilemap[tileIndex].item = TileItem::SPEED;

        return true;
    }

    return false;
}

bool Game::cmdReady_f()
{
    if( !gameMode.PlayerCanUseAction() )
    {
        hud->AddErrorMessage( "You can't do that right now" );
        return false;
    }

    gameMode.RoundStart();

    return true;
}

bool Game::cmdStartRound_f()
{
    route = *pathFind.BuildPath( tilemap.GetTiles() );

    currentPathTile = route.size() - 1;

    return true;
}

bool Game::cmdGenBlocks_f()
{
    if( !gameMode.PlayerCanUseAction() )
    {
        hud->AddErrorMessage( "You can't do that right now" );
        return false;
    }

    tilemap.Clear();

    switch( GetMode().GetModeParams().type )
    {
    case ModeType::NORMAL:
        generateMap( TileItem::SPEED );
        break;
    case ModeType::REVERSE:
        generateMap( TileItem::SLOW );
        break;
    default:
        generateMap( TileItem::EMPTY );
        break;
    }

    return true;
}

bool Game::cmdClearMap_f()
{
    if( !gameMode.PlayerCanUseAction() )
    {
        hud->AddErrorMessage( "You can't do that right now" );
        return false;
    }

    tilemap.Clear();

    return true;
}

bool Game::cmdShowRoute_f()
{
    if( !gameMode.PlayerCanUseAction() )
    {
        hud->AddErrorMessage( "You can't do that right now" );
        return false;
    }

    drawRoute = !drawRoute;

    if( route.empty() )
    {
        route = *pathFind.BuildPath( tilemap.GetTiles() );
    }

    return true;
}

bool Game::cmdGenMap_f()
{
    tilemap.Clear();

    switch( GetMode().GetModeParams().type )
    {
    case ModeType::NORMAL:
        generateMap( TileItem::SPEED );
        break;
    case ModeType::REVERSE:
        generateMap( TileItem::SLOW );
        break;
    default:
        generateMap( TileItem::EMPTY );
        break;
    }

    return true;
}

bool Game::cmdSaveSession_f()
{
    const auto type = gameMode.GetModeParams().type;

    if( type == ModeType::FREE )
    {
        return false;
    };

    gameStats.UpdateSessionData( gameMode.GetSessionData(), type );
    return true;
}

void Game::drawPath( sf::RenderTarget &render )
{
    if( !drawRoute )
    {
        return;
    }

    for( auto i = 0u; i < routeVerts.getVertexCount(); i++ )
    {
        routeVerts[i].color = sf::Color::Transparent;
    }

    for( auto &&index : route )
    {
        const int tile = tilemap.ConvertTileIndex( index );

        sf::Vertex *quad = &routeVerts[tile * 4];
        quad[0].color = sf::Color( 255, 255, 0, 104 );
        quad[1].color = sf::Color( 255, 255, 0, 104 );
        quad[2].color = sf::Color( 255, 255, 0, 104 );
        quad[3].color = sf::Color( 255, 255, 0, 104 );
    }

    render.draw( routeVerts, { tilemap.getTransform() } );
}

void Game::runnerTouchItem( const Route &route )
{
    if( currentPathTile >= route.size() )
    {
        return;
    }

    const int tile = tilemap.ConvertTileIndex( route[currentPathTile] );

    if( tilemap[tile].item == TileItem::SPEED )
    {
        runner.SetSpeedMod( RunnerSpeedMod::INC );

        tilemap[tile].item = TileItem::EMPTY;
    }

    if( tilemap[tile].item == TileItem::SLOW )
    {
        runner.SetSpeedMod( RunnerSpeedMod::DEC );

        tilemap[tile].item = TileItem::EMPTY;
    }
}

void Game::Init()
{
    actionManager.AddAction( "cmdBlock", [this]() { return cmdBlock_f(); } );
    actionManager.AddAction( "cmdUnblock", [this]() { return cmdUnblock_f(); } );
    actionManager.AddAction( "cmdSlow", [this]() { return cmdSlow_f(); } );
    actionManager.AddAction( "cmdSpeed", [this]() { return cmdSpeed_f(); } );
    actionManager.AddAction( "cmdReady", [this]() { return cmdReady_f(); } );
    actionManager.AddAction( "cmdClearMap", [this]() { return cmdClearMap_f(); } );
    actionManager.AddAction( "cmdGenBlocks", [this]() { return cmdGenBlocks_f(); } );
    actionManager.AddAction( "cmdShowRoute", [this]() { return cmdShowRoute_f(); } );
    actionManager.AddAction( "cmdStartRound", [this]() { return cmdStartRound_f(); } );
    actionManager.AddAction( "cmdGenMap", [this]() { return cmdGenMap_f(); } );
    actionManager.AddAction( "cmdSaveSession", [this]() { return cmdSaveSession_f(); } );

    menuHandler = std::make_shared<MenuHandler_Menu>();
    hudHandler = std::make_shared<MenuHandler_HUD>();
    hud = hudHandler->GetHUD();

    gameMode.SetHUD( hud );

    random.SetSeed( 0 );

    const sf::Vector2f worldPos( ( R_VIRTUAL_WIDTH - ( TILEMAP_TILES_W_NUM * TILEMAP_TILE_SIZE ) ) / 2.0f,
                                 ( R_VIRTUAL_HEIGHT - ( TILEMAP_TILES_H_NUM * TILEMAP_TILE_SIZE ) ) / 4.0f );

    tilemap.setPosition( worldPos );

    for( auto i = 0u; i < TILEMAP_TILES_NUM; i++ )
    {
        const auto &tile = tilemap[i];

        sf::Vertex *quad = &routeVerts[i * 4];

        quad[0].position = tile.quad[0].position;
        quad[1].position = tile.quad[1].position;
        quad[2].position = tile.quad[2].position;
        quad[3].position = tile.quad[3].position;
    }
}

void Game::StartNewGame( const ModeType newMode )
{
	inGame = false;

	currentMode = newMode;

	hudHandler->Show( true );

    tilemap.Clear();
    gameMode.Restart( newMode );

	inGame = true;
    drawRoute = false;

    actionManager.UseAction( "resetcooldowns" );

    random.SetSeed( game.thisTime );
}

void Game::RestartGame()
{
	StartNewGame( currentMode );
}

void Game::HandleEvents()
{
	if( userInput.GetKeyState( sf::Keyboard::Escape ) )
	{
		if( IsInGame() && !menuHandler->IsActive() )
		{
			menuHandler->Show( true );
			// MenuShow();

			inGame = false;
		}

		if( IsInGame() && hudHandler->IsActive() )
		{
			hudHandler->Show( false );

			menuHandler->Show( true );
			// MenuShow();

			inGame = false;
		}
	}

	menuHandler->Update();

	if( IsInGame() )
	{
		actionManager.Update();

		hudHandler->Update();

        tilemap.Update( time );

        gameMode.Update();

        if( !runner.IsMoving() && gameMode.OnRound() && gameMode.GetState() != ModeState::GAMEREVIEW )
        {
            runnerTouchItem( route );

            int tileStart = 0;
            int tileEnd = 0;

            if( currentPathTile > 0 && currentPathTile < route.size() )
            {
                tileStart = tilemap.ConvertTileIndex( route[currentPathTile] );
                tileEnd = tilemap.ConvertTileIndex( route[currentPathTile - 1] );
            }
            else
            {
                currentPathTile = -1;

                runner.SetSpeedMod( RunnerSpeedMod::NORMAL );

                gameMode.RoundEnd();
            }

            runner.SetWaypoints( tilemap[tileStart].position, tilemap[tileEnd].position );
            currentPathTile -= 1;
        }

        runner.Update( time );
	}
}

void Game::Frame( sf::RenderTarget &render )
{
    frameNum += 1;

	menuHandler->Draw( render );

	if( IsInGame() )
	{
        tilemap.Draw( render );
        drawPath( render );

        if( gameMode.OnRound() )
        {
            runner.Draw( render );
        }

        hudHandler->Draw( render );
	}
}

const GameMode &Game::GetMode() const
{
    return gameMode;
}

Game game;
