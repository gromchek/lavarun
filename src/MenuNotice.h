#ifndef MENUNOTICE_H
#define MENUNOTICE_H

#include "MenuWidget.h"
#include <SFML/Graphics/Text.hpp>


class MenuNotice final : public MenuWidget
{
private:
    sf::Text label;

public:
    MenuNotice() = delete;
    explicit MenuNotice( MenuHandler &handler );
    virtual ~MenuNotice() = default;

    void AddMessage( std::string &&message );

    void Clear();

    virtual void Update() override;

    virtual void Draw( sf::RenderTarget &render ) override;
};

#endif // MENUNOTICE_H
