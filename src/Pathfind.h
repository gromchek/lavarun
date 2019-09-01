#ifndef PATHFIND_H
#define PATHFIND_H

#include <string>
#include <vector>
#include "Tilemap.h"
#include "Misc.h"
#include <optional>

#include <SFML/System/Vector2.hpp>

using Route = std::vector<sf::Vector2i>;

struct NavCell
{
    int priority = 0;
    int closed = 0;
    int dir = 0;

    bool solid = false;
};

class Pathfind
{
private:
    NavCell cells[TILEMAP_TILES_NUM];
    sf::Vector2i startCell;
    sf::Vector2i endCell;

    void clear();
    NavCell &getCell( const sf::Vector2i &coord );

    Route buildPath();

public:
    Pathfind( const sf::Vector2i &start, const sf::Vector2i &end );

    std::optional<Route> BuildPath( const Tilearray &tiles );
};

inline NavCell &Pathfind::getCell( const sf::Vector2i &coord )
{
    const int index = coord.x + ( coord.y * TILEMAP_TILES_W_NUM );

    return cells[index];
}


#endif // PATHFIND_H
