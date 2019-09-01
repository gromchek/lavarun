#ifndef FONTSTYLES_H
#define FONTSTYLES_H

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Text.hpp>
#include <string>

struct FontStyle
{
    std::string fontName;

    unsigned int characterSize;

    sf::Color fillColor;

    float outlineThickness;
    sf::Color outlineColor;

    sf::Text::Style style;
};

// clang-format off

const FontStyle GAME_MENU_FONT_NORMAL{
    "alegreya-reg", 48, { 251, 205, 25 }, 0.0f, sf::Color::Transparent, sf::Text::Regular
};

const FontStyle GAME_MENU_FONT_HIGHLIGHT{
    "alegreya-reg", 48, sf::Color::White, 0.0f, sf::Color::Transparent, sf::Text::Regular
};

const FontStyle GAME_MENU_FONT_DISABLE{
    "alegreya-reg",	48, { 127, 127, 127 }, 0.0f, sf::Color::Transparent, sf::Text::Regular
};

const FontStyle GAME_MENU_SCREENTITLE_FONT_NORMAL{
    "alegreya-reg", 64, { 255, 215, 0 }, 0.0f, sf::Color::Transparent, sf::Text::Regular
};

const FontStyle GAME_HOTKEY_FONT_NORMAL{
    "alegreya", 20, { 255, 215, 0 }, 1.5f, sf::Color::Black, sf::Text::Bold
};

const FontStyle GAME_COOLDOWN_FONT_NORMAL{
    "alegreya", 36, { 215, 215, 215 }, 1.0f, sf::Color::Black, sf::Text::Regular
};

const FontStyle GAME_COOLDOWN_FONT_SOONISH{
    "alegreya", 36, { 255, 242, 80 }, 1.0f, sf::Color::Black, sf::Text::Regular
};

const FontStyle GAME_ERROR_FONT_NORMAL{
    "alegreya", 24, { 255, 50, 50 }, 1.0f, sf::Color::Black, sf::Text::Regular
};

const FontStyle GAME_PROGRESSBAR_FONT_NORMAL{
    "alegreya", 28, sf::Color::White, 1.0f, sf::Color::Black, sf::Text::Regular
};

const FontStyle GAME_MESSAGE_FONT_NORMAL{
    "alegreya", 28, { 255, 215, 0 }, 1.0f, sf::Color::Black, sf::Text::Regular
};

const FontStyle GAME_MULTIBOARD_FONT_NORMAL{
    "alegreya",	30, { 251, 205, 100 }, 1.0f, sf::Color::Black, sf::Text::Regular
};

const FontStyle GAME_MULTIBOARD_FONT_HEADER{
    "alegreya",	28, { 251, 205, 100 }, 1.0f, sf::Color::Black, sf::Text::Regular
};

const FontStyle GAME_MULTIBOARD_FONT_BEST{
    "alegreya", 30, { 25, 255, 25 }, 1.0f, sf::Color::Black, sf::Text::Regular
};

const FontStyle GAME_NOTICE_FONT_NORMAL{
    "alegreya", 120, { 243, 100, 37 }, 1.25f, sf::Color::Black, sf::Text::Bold
};

// clang-format on

void ApplyFontStyle( sf::Text &text, const FontStyle &fontStyle );

#endif // FONTSTYLES_H
