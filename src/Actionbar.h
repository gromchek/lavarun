#ifndef SPELLBAR_H
#define SPELLBAR_H

#include <vector>
#include <string>
#include <memory>

#include "MenuWidget.h"

#include "Ninepatch.h"

#include "ActionManager.h"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "Cooldown.h"

const sf::Vector2f ACTIONBAR_DEFAULT_SIZE( 624.0f, 64.0f );

namespace sf
{
class RenderTarget;
}

class ActionButton final : public MenuWidget, public sf::Drawable
{
private:
    std::string name;
    sf::Text hotkey_text;
    Cooldown cooldown;
	std::string action;

	sf::RectangleShape icon;
    std::optional<unsigned int> iconTexIndex;
    sf::RectangleShape border;
    std::optional<unsigned int> borderTexIndex;

	void updateTexture( const ActionInfo &info );
    void updateHotkey();
    void updateCooldown();

public:
    ActionButton() = delete;
    explicit ActionButton( MenuHandler &handler, std::string &&internalName );
	virtual ~ActionButton() = default;

    virtual void Update() override;
    virtual void Draw( sf::RenderTarget &render ) override;

    std::string_view GetName() const;

    void SetAction( std::string &&newAction );
    std::string_view GetAction() const;

private:
    virtual void draw( sf::RenderTarget &target, sf::RenderStates states ) const override;
};

inline void ActionButton::Draw( [[maybe_unused]] sf::RenderTarget &render )
{
}

inline std::string_view ActionButton::GetName() const
{
    return name;
}

inline std::string_view ActionButton::GetAction() const
{
	return action;
}

using ActionButtonPtr = std::shared_ptr<ActionButton>;

class ActionBar final : public MenuWidget
{
private:
    Ninepatch background;
	std::vector<ActionButtonPtr> buttons;

public:
    ActionBar() = delete;
    explicit ActionBar( MenuHandler &handler );
    virtual ~ActionBar() = default;

	void ClearActions();
    void AddAction( const std::size_t slot, std::string &&action );

    virtual void Update() override;
    virtual void Draw( sf::RenderTarget &render ) override;
};

#endif // SPELLBAR_H
