#ifndef NINEPATCH_H
#define NINEPATCH_H

#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Transformable.hpp>

class Ninepatch final : public sf::Drawable, public sf::Transformable
{
private:
    sf::VertexArray verts;
    sf::Vector2f size;
    const sf::Texture *texture;

    sf::Color color;

    void updatePatch();
    void updateColor();

public:
    Ninepatch() = delete;
    Ninepatch( const sf::Vector2f &patchSize, const sf::Texture *newTexture );
    virtual ~Ninepatch() = default;

    const sf::Color &getFillColor();
    void setFillColor( const sf::Color &fillColor );

    const sf::Texture *getTexture() const;
    void setTexture( const sf::Texture *newTexture );

    void setSize( const sf::Vector2f &newSize );
    const sf::Vector2f &getSize() const;

    sf::FloatRect getLocalBounds() const;

private:
    virtual void draw( sf::RenderTarget &target, sf::RenderStates states ) const override;
};

inline const sf::Color &Ninepatch::getFillColor()
{
    return color;
}

inline const sf::Texture *Ninepatch::getTexture() const
{
    return texture;
}

inline const sf::Vector2f &Ninepatch::getSize() const
{
    return size;
}

inline sf::FloatRect Ninepatch::getLocalBounds() const
{
    return { { 0.0f, 0.0f }, size };
}

#endif // NINEPATCH_H
