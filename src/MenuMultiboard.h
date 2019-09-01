#ifndef MENUMULTIBOARD_H
#define MENUMULTIBOARD_H

#include <string>
#include <array>

#include "MenuWidget.h"
#include <SFML/Graphics/Text.hpp>
#include "Ninepatch.h"

#include "GameMode.h"

const sf::Vector2f MULTIBOARD_DEFAULT_SIZE( 128.0f, 32.0f );

struct MultiboardRow
{
    std::string text;

    sf::Color color;
};

class MenuMultiboard final : public MenuWidget
{
private:
    static const unsigned int MAX_ROWS = 7;
    std::array<MultiboardRow, MAX_ROWS> table;
    unsigned int currentRow;
    sf::Text title;
    sf::Text row;

    Ninepatch background;
    Ninepatch header;

public:
    MenuMultiboard() = delete;
    explicit MenuMultiboard( MenuHandler &handler, const unsigned int cellCount = 2 );
    virtual ~MenuMultiboard() = default;

    void SetTitle( std::string &&str );
    void AddRow( std::string &&value );

    void ClearRowsColor();
    void SetRowColor( const unsigned int row, const sf::Color color );

    void Clear();

    virtual void Update() override;
    virtual void Draw( sf::RenderTarget &render ) override;
};

#endif // MENUMULTIBOARD_H
