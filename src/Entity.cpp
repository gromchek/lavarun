#include "Entity.h"
#include "Misc.h"
#include "ResourceManager.h"

#include <algorithm>

#include <SFML/Graphics/RenderTarget.hpp>

constexpr unsigned int MAX_TILES_PASS_ON_SPEED_MOD = 2;

sf::Vector2f lerp( const sf::Vector2f &a, const sf::Vector2f &b, const float t )
{
    return a * ( 1.0f - t ) + b * t;
}


struct SpeedModTable
{
    RunnerSpeedMod type;
    int timeBetweenTiles;
    sf::Color modColor;
};

const SpeedModTable speedModTable[] = { { RunnerSpeedMod::NORMAL, 500, { 255, 255, 255 } },
                                        { RunnerSpeedMod::INC, 375, { 37, 197, 53 } },
                                        { RunnerSpeedMod::DEC, 625, { 160, 32, 240 } } };

void Runner::updateVisual()
{
    resourceManager.ValidateTextureIndex( modelTexIndex, "runner" );

    model.setTexture( &resourceManager.GetTextureByIndex( modelTexIndex ) );
    model.setTextureRect( anim.GetCurrentFrame().frameCoord );

    if( ( a.x - b.x ) > 0.0f )
    {
        const auto &rect = anim.GetCurrentFrame().frameCoord;
        model.setTextureRect( { rect.left, rect.top, -rect.width, rect.height } );
    }


    const auto &speedModValue =
        *std::find_if( std::begin( speedModTable ), std::end( speedModTable ),
                       [type = mode]( const auto &speedMode ) { return speedMode.type == type; } );

    model.setFillColor( speedModValue.modColor );
}

Runner::Runner() : moving( false ), mode( RunnerSpeedMod::NORMAL ), thinktime( 0 ), tilesPassed( 0 )
{
    model.setSize( { TILEMAP_TILE_SIZE, TILEMAP_TILE_SIZE } );
    model.setFillColor( sf::Color::White );

    for( int i = 0; i < 10; i++ )
    {
        anim.AddFrame(
            { { int( TILEMAP_TILE_SIZE ) * i, 0, int( TILEMAP_TILE_SIZE ), int( TILEMAP_TILE_SIZE ) }, 75 } );
    }
}

void Runner::Update( const int dt )
{
    if( !moving )
    {
        return;
    }

    anim.Update( dt );

    thinktime += dt;

    const auto &speedModValue =
        *std::find_if( std::begin( speedModTable ), std::end( speedModTable ),
                       [type = mode]( const auto &speedMode ) { return speedMode.type == type; } );

    if( thinktime > speedModValue.timeBetweenTiles )
    {
        thinktime -= speedModValue.timeBetweenTiles;
        moving = false;

        tilesPassed += 1;

        if( tilesPassed > MAX_TILES_PASS_ON_SPEED_MOD )
        {
            tilesPassed = 0;
            mode = RunnerSpeedMod::NORMAL;
        }

        return;
    }

    model.setPosition( lerp( a, b, float( thinktime ) / float( speedModValue.timeBetweenTiles ) ) );

    updateVisual();
}

void Runner::Draw( sf::RenderTarget &render )
{
    render.draw( model );
}
