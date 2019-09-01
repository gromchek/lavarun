#include "Tilemap.h"
#include "ResourceManager.h"
#include "Misc.h"
#include "UserInput.h"
#include "ActionManager.h"

#include "Game.h"

#include <cassert>

#include <SFML/Graphics/RenderTarget.hpp>

inline void TextureRectToQuadTex( const sf::IntRect &rect, sf::Vertex *quad )
{
    if( !quad )
    {
        return;
    }

    const float left = static_cast<float>( rect.left );
    const float right = left + static_cast<float>( rect.width );
    const float top = static_cast<float>( rect.top );
    const float bottom = top + static_cast<float>( rect.height );

    quad[0].texCoords.x = left;
    quad[0].texCoords.y = top;

    quad[1].texCoords.x = right;
    quad[1].texCoords.y = top;

    quad[2].texCoords.x = right;
    quad[2].texCoords.y = bottom;

    quad[3].texCoords.x = left;
    quad[3].texCoords.y = bottom;
}

void Tilemap::initWorldBorder()
{
    worldBorder.resize( 4 * ( TILEMAP_TILES_W_NUM + 2 ) );
    worldBorder.setPrimitiveType( sf::PrimitiveType::Quads );

    for( auto i = 0u; i < TILEMAP_TILES_W_NUM + 2; i++ )
    {
        sf::Vertex *quad = &worldBorder[i * 4];

        quad[0].position = sf::Vector2f( i * TILEMAP_TILE_SIZE - TILEMAP_TILE_SIZE, -TILEMAP_TILE_SIZE );
        quad[1].position = sf::Vector2f( ( i + 1 ) * TILEMAP_TILE_SIZE - TILEMAP_TILE_SIZE, -TILEMAP_TILE_SIZE );
        quad[2].position = sf::Vector2f( ( i + 1 ) * TILEMAP_TILE_SIZE - TILEMAP_TILE_SIZE, 0.0f );
        quad[3].position = sf::Vector2f( i * TILEMAP_TILE_SIZE - TILEMAP_TILE_SIZE, 0.0f );

        TextureRectToQuadTex( { 192, 128, 64, 64 }, quad );
    }

    sf::Vertex *quad = &worldBorder[0 * 4];
    TextureRectToQuadTex( { 128, 128, 64, 64 }, quad );

    quad = &worldBorder[( TILEMAP_TILES_W_NUM + 1 ) * 4];
    TextureRectToQuadTex( { 256, 128, 64, 64 }, quad );
}

void Tilemap::drawWorldBorder( sf::RenderTarget &render )
{
    sf::RenderStates state;
    sf::Transform trans( getTransform() );

    // top
    state.transform *= trans;
    state.texture = &resourceManager.GetTextureByIndex( worldTexIndex );
    render.draw( worldBorder, state );

    // right
    sf::Transform rot;
    rot.rotate( 90.0f );
    trans.translate( { TILEMAP_TILE_SIZE * TILEMAP_TILES_W_NUM, 0.0f } );
    state.transform = trans * rot;
    render.draw( worldBorder, state );

    // bot
    rot.rotate( 90.0f );
    trans.translate( { 0.0f, TILEMAP_TILE_SIZE * TILEMAP_TILES_H_NUM } );
    state.transform = trans * rot;
    render.draw( worldBorder, state );

    // left
    rot.rotate( 90.0f );
    trans.translate( { -TILEMAP_TILE_SIZE * TILEMAP_TILES_W_NUM, 0.0f } );
    state.transform = trans * rot;
    render.draw( worldBorder, state );
}

void Tilemap::drawItems( sf::RenderTarget &render )
{
    for( auto &&tile : tiles )
    {
        if( tile.item == TileItem::EMPTY )
        {
            continue;
        }

        item.setPosition( tile.position );

        if( tile.item == TileItem::SLOW )
        {
            item.setTextureRect( { 0, 0, 64, 64 } );
        }
        if( tile.item == TileItem::SPEED )
        {
            item.setTextureRect( { 64, 0, 64, 64 } );
        }

        render.draw( item );
    }
}

void Tilemap::drawSelectedTileBorder( sf::RenderTarget &render )
{
    if( selectedTileIndex < tiles.size() )
    {
        border.setPosition( tiles[selectedTileIndex].position );
        border.setOutlineColor( { 25, 255, 25 } );
        border.setOutlineThickness( 2.0f );

        render.draw( border );
    }

    border.setOutlineColor( { 255, 255, 255, 255 } );
    border.setOutlineThickness( 0.0f );
}

void Tilemap::updateSelectedTileIndex()
{
    const auto &mouse = userInput.GetMousePosition();

    const sf::IntRect tilemap( getPosition().x, getPosition().y, int( TILEMAP_WIDTH_SIZE ),
                               int( TILEMAP_HEIGHT_SIZE ) );

    if( !tilemap.contains( mouse ) )
    {
        return;
    }

    const int tileX = int( mouse.x - getPosition().x ) / int( TILEMAP_TILE_SIZE );
    const int tileY = int( mouse.y - getPosition().y ) / int( TILEMAP_TILE_SIZE );

    selectedTileIndex = ConvertTileIndex( { tileX, tileY } );

    if( selectedTileIndex == TILEMAP_START_TILE || selectedTileIndex == TILEMAP_END_TILE )
    {
        selectedTileIndex = -1;
    }
}

void Tilemap::updateTexture()
{
    for( auto &&tile : tiles )
    {
        if( tile.isSolid )
        {
            TextureRectToQuadTex( animTile.GetCurrentFrame().frameCoord, tile.quad );
        }
        else
        {
            TextureRectToQuadTex( { int( tile.texIndex * TILEMAP_TILE_SIZE ), 0, 64, 64 }, tile.quad );
        }
    }

    TextureRectToQuadTex( { 0, 128, int( TILEMAP_TILE_SIZE ), int( TILEMAP_TILE_SIZE ) },
                          tiles[TILEMAP_START_TILE].quad );
    TextureRectToQuadTex( { 64, 128, int( TILEMAP_TILE_SIZE ), int( TILEMAP_TILE_SIZE ) },
                          tiles[TILEMAP_END_TILE].quad );
}

Tilemap::Tilemap() : selectedTileIndex( -1 )
{
    item.setSize( { TILEMAP_TILE_SIZE, TILEMAP_TILE_SIZE } );
    border.setSize( item.getSize() );
    border.setFillColor( sf::Color::Transparent );

    unsigned int index = 0;

    verts.setPrimitiveType( sf::PrimitiveType::Quads );
    verts.resize( TILEMAP_TILES_NUM * 4 );

    for( auto &tile : tiles )
    {
        tile.isSolid = false;
        tile.item = TileItem::EMPTY;
        tile.index = ConvertTileIndex( index );

        sf::Vertex *quad = &verts[index * 4];

        quad[0].position = sf::Vector2f( tile.index.x * TILEMAP_TILE_SIZE, tile.index.y * TILEMAP_TILE_SIZE );
        quad[1].position = sf::Vector2f( ( tile.index.x + 1 ) * TILEMAP_TILE_SIZE, tile.index.y * TILEMAP_TILE_SIZE );
        quad[2].position =
            sf::Vector2f( ( tile.index.x + 1 ) * TILEMAP_TILE_SIZE, ( tile.index.y + 1 ) * TILEMAP_TILE_SIZE );
        quad[3].position = sf::Vector2f( tile.index.x * TILEMAP_TILE_SIZE, ( tile.index.y + 1 ) * TILEMAP_TILE_SIZE );

        tile.quad = quad;

        index += 1;

        tile.texIndex = 0;
    }

    for( int i = 0; i < 6; i++ )
    {
        animTile.AddFrame( { { 64 * i, 64, int( TILEMAP_TILE_SIZE ), int( TILEMAP_TILE_SIZE ) }, 500 } );
    }

    initWorldBorder();
}

void Tilemap::Update( const unsigned int dt )
{
    selectedTileIndex = -1;

    animTile.Update( dt );

    updateSelectedTileIndex();
    updateTexture();
}

void Tilemap::Clear()
{
    random.SetSeed( game.thisTime );

    resourceManager.ValidateTextureIndex( worldTexIndex, "world" );

    for( auto &tile : tiles )
    {
        tile.isSolid = false;
        tile.item = TileItem::EMPTY;

        tile.texIndex = random.GetInt( 6 );

        tile.position = tile.quad[0].position + getPosition();
    }

    resourceManager.ValidateTextureIndex( itemTexIndex, "items" );

    item.setTexture( &resourceManager.GetTextureByIndex( itemTexIndex ) );
    item.setFillColor( sf::Color::White );
}

void Tilemap::Draw( sf::RenderTarget &render )
{
    sf::RenderStates state;

    state.transform *= getTransform();
    state.texture = &resourceManager.GetTextureByIndex( worldTexIndex );

    render.draw( verts, state );

    drawWorldBorder( render );

    drawItems( render );

    drawSelectedTileBorder( render );
}

const Tile &Tilemap::operator[]( const unsigned int index ) const
{
    assert( index < tiles.size() );
    return tiles[index];
}

Tile &Tilemap::operator[]( const unsigned int index )
{
    assert( index < tiles.size() );
    return tiles[index];
}

std::vector<sf::Vector2i> Tilemap::Neighbors( const sf::Vector2i &index, const TileNeighborsDir tileDir,
                                              const unsigned int dist ) const
{
    std::vector<sf::Vector2i> dirs;
    dirs.reserve( 8 );

    dirs.push_back( { 1, 0 } );
    dirs.push_back( { 0, 1 } );
    dirs.push_back( { -1, 0 } );
    dirs.push_back( { 0, -1 } );

    if( tileDir == TileNeighborsDir::ALL )
    {
        dirs.push_back( { -1, 1 } );
        dirs.push_back( { 1, 1 } );
        dirs.push_back( { 1, -1 } );
        dirs.push_back( { -1, -1 } );
    }

    std::vector<sf::Vector2i> result;
    result.reserve( 8 );

    for( const auto &dir : dirs )
    {
        for( auto i = 1u; i < dist + 1; i++ )
        {
            sf::Vector2i n{ index.x + dir.x * i, index.y + dir.y * i };
            if( 0 <= n.x && n.x < TILEMAP_TILES_W_NUM && 0 <= n.y && n.y < TILEMAP_TILES_H_NUM )
            {
                result.push_back( n );
            }
        }
    }

    return result;
}

std::vector<sf::Vector2i> Tilemap::Neighbors( const unsigned int index, const TileNeighborsDir tileDir,
                                              const unsigned int dist ) const
{
    return Neighbors( ConvertTileIndex( index ), tileDir, dist );
}
