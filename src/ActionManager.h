#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include <string>
#include <array>
#include <functional>
#include <map>
#include <SFML/Graphics/Rect.hpp>

namespace sf
{
class RenderTarget;
}

enum class ActionId
{
    INVALID,
    BLOCK,
    UNBLOCK,
    SLOW,
    SPEED,
    READY,
    CLEARMAP,
    GENBLOCKS,
    SHOWROUTE,
    RELOADCD,
    STARTROUND,
    GENMAP,
    SAVESESSION,
    COUNT
};

struct ActionInfo
{
    ActionId id;
    std::string name;
	std::string texName;
	sf::IntRect texCoord;
	int cooldown;
};

struct ActionCooldown
{
    int start = 0;
    int duration = 0;
};

class ActionManager final
{
private:
    std::array<ActionCooldown, int( ActionId::COUNT )> actionsCooldown;
	std::map<std::string, std::function<bool()>> actions;

    void resetCooldowns();

public:
    ActionManager();
	~ActionManager() = default;

	void AddAction( std::string &&name, std::function<bool()> &&action );
    void RemoveAction( std::string &&name );

	void Update();

    const ActionInfo &GetActionInfo( std::string_view name );

    ActionCooldown GetActionCooldown( std::string_view name );

    void UseAction( std::string_view name );
};

extern ActionManager actionManager;

#endif // ACTIONMANAGER_H
