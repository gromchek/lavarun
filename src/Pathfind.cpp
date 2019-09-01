#include "Pathfind.h"

#include <cmath>
#include <queue>

constexpr int dir = 4;
constexpr int dx[dir] = { 1, 0, -1, 0 };
constexpr int dy[dir] = { 0, 1, 0, -1 };

class Node
{
private:
    sf::Vector2i coord;
    int dist;
    int priority;

    int estimate( const sf::Vector2i &dest ) const;

public:
    Node() = default;
    Node( const sf::Vector2i &c, const int d, const int p );
    Node( const Node &node );
    ~Node() = default;

    void Set( const Node &node );
    const sf::Vector2i &GetPosition() const;
    int GetDist() const;
    int GetPriority() const;

    void UpdateDist();
    void UpdatePriority( const sf::Vector2i &dest );
};

inline int Node::estimate( const sf::Vector2i &dest ) const
{
    const auto &dist = dest - coord;

    int d = static_cast<int>( std::sqrt( dist.x * dist.x + dist.y * dist.y ) );

    return d;
}

inline Node::Node( const sf::Vector2i &c, const int d, const int p ) : coord( c ), dist( d ), priority( p )
{
}

inline Node::Node( const Node &node ) : coord( node.coord ), dist( node.dist ), priority( node.priority )
{
}

inline void Node::Set( const Node &node )
{
    coord = node.coord;
    dist = node.dist;
    priority = node.priority;
}

inline const sf::Vector2i &Node::GetPosition() const
{
    return coord;
}

inline int Node::GetDist() const
{
    return dist;
}

inline int Node::GetPriority() const
{
    return priority;
}

inline void Node::UpdateDist()
{
    dist += 10;
}

inline void Node::UpdatePriority( const sf::Vector2i &dest )
{
    priority = dist + estimate( dest ) * 10;
}

bool operator<( const Node &a, const Node &b )
{
    return a.GetPriority() > b.GetPriority();
}

void Pathfind::clear()
{
    for( auto &cell : cells )
    {
        cell.priority = 0;
        cell.closed = 0;
        cell.dir = 0;

        cell.solid = false;
    }
}


Pathfind::Pathfind( const sf::Vector2i &start, const sf::Vector2i &end ) : startCell( start ), endCell( end )
{
    clear();
}

std::optional<Route> Pathfind::BuildPath( const Tilearray &tiles )
{
    clear();

    for( auto i = 0u; i < tiles.size(); i++ )
    {
        cells[i].solid = tiles[i].isSolid;
    }

    auto route( buildPath() );

    if( route.empty() )
    {
        return std::nullopt;
    }

    return { route };
}

Route Pathfind::buildPath()
{
    std::vector<sf::Vector2i> route;
    route.reserve( 32 );

    std::priority_queue<Node> pq[2];

    int pqi = 0;

    Node n0( { startCell, 0, 0 } );
    n0.UpdatePriority( endCell );

    pq[pqi].push( n0 );
    getCell( startCell ).priority = n0.GetPriority();

    while( !pq[pqi].empty() )
    {
        n0.Set( pq[pqi].top() );

        auto pos = n0.GetPosition();

        pq[pqi].pop();

        getCell( pos ).priority = 0;
        getCell( pos ).closed = 1;

        if( pos == endCell )
        {
            route.push_back( endCell );

            while( !( pos == startCell ) )
            {
                const auto j = getCell( pos ).dir;

                pos.x += dx[j];
                pos.y += dy[j];

                route.push_back( pos );
            }

            return route;
        }

        for( int i = 0; i < dir; i++ )
        {
            sf::Vector2i dPos( pos.x + dx[i], pos.y + dy[i] );

            if( !( dPos.x < 0 || dPos.x > TILEMAP_TILES_W_NUM - 1 || dPos.y < 0 || dPos.y > TILEMAP_TILES_H_NUM - 1 ||
                   getCell( dPos ).solid || getCell( dPos ).closed == 1 ) )
            {

                Node m0( { dPos, n0.GetDist(), n0.GetPriority() } );
                m0.UpdateDist();
                m0.UpdatePriority( endCell );

                if( getCell( dPos ).priority == 0 )
                {
                    getCell( dPos ).priority = m0.GetPriority();
                    pq[pqi].push( m0 );
                    getCell( dPos ).dir = ( i + dir / 2 ) % dir;
                }
                else
                {
                    if( getCell( dPos ).priority > m0.GetPriority() )
                    {
                        getCell( dPos ).priority = m0.GetPriority();
                        getCell( dPos ).dir = ( i + dir / 2 ) % dir;

                        while( !( pq[pqi].top().GetPosition() == dPos ) )
                        {
                            pq[1 - pqi].push( pq[pqi].top() );
                            pq[pqi].pop();
                        }
                        pq[pqi].pop();

                        if( pq[pqi].size() > pq[1 - pqi].size() )
                        {
                            pqi = 1 - pqi;
                        }
                        while( !pq[pqi].empty() )
                        {
                            pq[1 - pqi].push( pq[pqi].top() );
                            pq[pqi].pop();
                        }
                        pqi = 1 - pqi;
                        pq[pqi].push( m0 );
                    }
                }
            }
        }
    }

    return {};
}
