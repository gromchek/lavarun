#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics/RectangleShape.hpp>
#include "Animation.h"
#include <optional>

namespace sf
{
class RenderTarget;
}

enum class RunnerSpeedMod
{
    NORMAL,
    INC,
    DEC
};

class Runner
{
private:
    bool moving;
    RunnerSpeedMod mode;
    sf::Vector2f a;
    sf::Vector2f b;
    int thinktime;
    sf::RectangleShape model;
    std::optional<unsigned int> modelTexIndex;
    unsigned int tilesPassed;

    AnimationSet anim;

    void updateVisual();

public:
    Runner();
    ~Runner() = default;

    void Update( const int dt );
    void Draw( sf::RenderTarget &render );

    void SetSpeedMod( RunnerSpeedMod speedMode );

    void SetWaypoints( const sf::Vector2f &start, const sf::Vector2f &end );

    bool IsMoving() const;
};

inline void Runner::SetSpeedMod( RunnerSpeedMod speedMode )
{
    tilesPassed = 0;

    if( mode == RunnerSpeedMod::INC && speedMode == RunnerSpeedMod::DEC )
    {
        mode = RunnerSpeedMod::NORMAL;
        return;
    }

    if( mode == RunnerSpeedMod::DEC && speedMode == RunnerSpeedMod::INC )
    {
        mode = RunnerSpeedMod::NORMAL;
        return;
    }

    mode = speedMode;
}

inline void Runner::SetWaypoints( const sf::Vector2f &start, const sf::Vector2f &end )
{
    a = start;
    b = end;
    model.setPosition( a );

    moving = true;
}

inline bool Runner::IsMoving() const
{
    return moving;
}

#endif // ENTITY_H
