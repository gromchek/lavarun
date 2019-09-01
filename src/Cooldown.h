#ifndef COOLDOWN_H
#define COOLDOWN_H

#include <array>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Transformable.hpp>


class Cooldown final : public sf::Drawable, public sf::Transformable
{
private:
    const float size;
    float angle;
    bool onCooldown;
    sf::VertexArray verts;
    std::array<sf::Vector2f, 16> clipRegion;
    sf::Text label;

    void initClipRegion();
    void updateVertex();
    void updateCountdown( const int start, const int duration );

public:
    Cooldown() = delete;
    explicit Cooldown( const float btnSize );
    virtual ~Cooldown() = default;

    void SetCooldown( const int start, const int duration );

    void Update();

private:
    virtual void draw( sf::RenderTarget &target, sf::RenderStates states ) const override;
};

#endif // COOLDOWN_H
