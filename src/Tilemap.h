#ifndef TILEMAP_H
#define TILEMAP_H

#include <array>
#include "Misc.h"
#include "Animation.h"
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "Rnd.h"
#include <optional>

enum class TileItem
{
    EMPTY,
    SPEED,
    SLOW
};

struct Tile
{
    sf::Vector2f position;
    bool isSolid;
    TileItem item;

    unsigned int texIndex;

    sf::Vertex *quad;

    sf::Vector2i index;
};

enum class TileNeighborsDir
{
    UDLR, // up down left right
    ALL // + diagonal
};

using Tilearray = std::array<Tile, TILEMAP_TILES_NUM>;


namespace sf
{
class RenderTarget;
}

class Tilemap final : public sf::Transformable
{
private:
    unsigned int selectedTileIndex;
    sf::VertexArray worldBorder;
    sf::VertexArray verts;
    sf::RectangleShape border;
    std::optional<unsigned int> worldTexIndex;

    Tilearray tiles;

    sf::RectangleShape item;
    std::optional<unsigned int> itemTexIndex;
    AnimationSet animTile;

    Random random;

    void initWorldBorder();

    void drawWorldBorder( sf::RenderTarget &render );
    void drawItems( sf::RenderTarget &render );
    void drawSelectedTileBorder( sf::RenderTarget &render );
    void updateSelectedTileIndex();

    void updateTexture();

public:
    Tilemap();
    virtual ~Tilemap() = default;

    void Update( const unsigned int dt );

    void Clear();

    void Draw( sf::RenderTarget &render );

    unsigned int GetSelectedTileIndex() const;
    const Tilearray &GetTiles();

    const Tile &operator[]( const unsigned int index ) const;
    Tile &operator[]( const unsigned int index );

    int ConvertTileIndex( const sf::Vector2i &index ) const;
    sf::Vector2i ConvertTileIndex( const unsigned int index ) const;

    std::vector<sf::Vector2i> Neighbors( const sf::Vector2i &index,
                                         const TileNeighborsDir tileDir = TileNeighborsDir::UDLR,
                                         const unsigned int dist = 1 ) const;
    std::vector<sf::Vector2i> Neighbors( const unsigned int index,
                                         const TileNeighborsDir tileDir = TileNeighborsDir::UDLR,
                                         const unsigned int dist = 1 ) const;
};

inline unsigned int Tilemap::GetSelectedTileIndex() const
{
    return selectedTileIndex;
}

inline const Tilearray &Tilemap::GetTiles()
{
    return tiles;
}

inline int Tilemap::ConvertTileIndex( const sf::Vector2i &index ) const
{
    return index.x + ( index.y * TILEMAP_TILES_W_NUM );
}

inline sf::Vector2i Tilemap::ConvertTileIndex( const unsigned int index ) const
{
    return { int( index % TILEMAP_TILES_W_NUM ), int( index / TILEMAP_TILES_H_NUM ) };
}

#endif // TILEMAP_H
