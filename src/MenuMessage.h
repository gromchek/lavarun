#ifndef MENUMESSAGE_H
#define MENUMESSAGE_H

#include <string>
#include "MenuWidget.h"
#include "Misc.h"

#include <SFML/Graphics/Text.hpp>

const sf::Vector2f MENUMESSAGE_DEFAULT_SIZE( 256, 96 );
const unsigned int LINES_COUNT = 3;

enum class MessageType
{
    ERROR,
    NOTIFY
};

class MenuMessage final : public MenuWidget
{
private:
    MessageType type;
    unsigned currentIndex;
    FadingLine line[LINES_COUNT];
    sf::Text label;

public:
    MenuMessage() = delete;
    explicit MenuMessage( MenuHandler &handler, const MessageType messageType );
    virtual ~MenuMessage() = default;

    void AddMessage( std::string &&message );

    void Clear();

    virtual void Draw( sf::RenderTarget &render ) override;
    virtual void Update() override;
};
#endif // MENUMESSAGE_H
