#ifndef MENUITEM_H
#define MENUITEM_H

#include "MenuWidget.h"

#include "Ninepatch.h"

#include <string>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

const sf::Vector2f MENUITEM_DEFAULT_SIZE( 320.0f, 64.0f );

class MenuItem final : public MenuWidget
{
private:
    sf::Text label;
    sf::RectangleShape btnGlow;
    std::optional<unsigned int> btnGlowTexIndex;
    Ninepatch patch;

public:
    MenuItem() = delete;
    MenuItem( MenuHandler &handler, const std::string &text );
    virtual ~MenuItem() = default;

    virtual void Draw( sf::RenderTarget &render ) override;
    virtual void Update() override;
};

#endif // MENUITEM_H
