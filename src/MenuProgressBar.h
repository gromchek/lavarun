#ifndef MENUPROGRESSBAR_H
#define MENUPROGRESSBAR_H

#include "MenuWidget.h"

#include "Ninepatch.h"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

enum class MenuProgressBarDir
{
    INC,
    DEC
};

const sf::Vector2f PROGRESS_BAR_DEFAULT_SIZE( 500, 40 );

class MenuProgressBar final : public MenuWidget
{
private:
    sf::Text label;
    Ninepatch border;
    MenuProgressBarDir dir;
    sf::RectangleShape gradient;
    std::optional<unsigned int> gradientTexIndex;
    std::string text;

    float val;
    float min;
    float max;

    bool fadeOut;

    void updateIncrease();
    void updateDecrease();
    void updateFadeOut();

public:
    MenuProgressBar() = delete;
    explicit MenuProgressBar( MenuHandler &handler );
    virtual ~MenuProgressBar() = default;

    void SetBarDirection( const MenuProgressBarDir direction );

    void SetBarText( std::string &&str );
    void SetMinMaxValue( const float minVal, const float maxVal );
    void SetValue( const float v );

    virtual void Update() override;
    virtual void Draw( sf::RenderTarget &render ) override;
};

inline void MenuProgressBar::SetBarDirection( const MenuProgressBarDir direction )
{
    dir = direction;

    gradientTexIndex = std::nullopt;
}

inline void MenuProgressBar::SetValue( const float v )
{
    val = v;
}

#endif // MENUPROGRESSBAR_H
