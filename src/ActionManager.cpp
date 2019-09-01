#include <cassert>
#include <utility>

#include "ActionManager.h"

#include "Game.h"

#include <SFML/Graphics/RenderTarget.hpp>

const ActionInfo actionsInfo[] = { { ActionId::INVALID, "_null", "spelldata", { 0, 0, 64, 64 }, 1 },
                                   { ActionId::BLOCK, "cmdBlock", "spelldata", { 1 * 64, 0, 64, 64 }, 2000 },
                                   { ActionId::UNBLOCK, "cmdUnblock", "spelldata", { 2 * 64, 0, 64, 64 }, 2000 },
                                   { ActionId::SLOW, "cmdSlow", "spelldata", { 3 * 64, 0, 64, 64 }, 4000 },
                                   { ActionId::SPEED, "cmdSpeed", "spelldata", { 4 * 64, 0, 64, 64 }, 4000 },
                                   { ActionId::READY, "cmdReady", "spelldata", { 5 * 64, 0, 64, 64 }, 1000 },
                                   { ActionId::CLEARMAP, "cmdClearMap", "spelldata", { 6 * 64, 0, 64, 64 }, 1000 },
                                   { ActionId::GENBLOCKS, "cmdGenBlocks", "spelldata", { 7 * 64, 0, 64, 64 }, 1000 },
                                   { ActionId::SHOWROUTE, "cmdShowRoute", "spelldata", { 8 * 64, 0, 64, 64 }, 1000 },
                                   { ActionId::RELOADCD, "resetcooldowns", "spelldata", { 0, 0, 64, 64 }, 1 },
                                   { ActionId::STARTROUND, "cmdStartRound", "spelldata", { 0, 0, 64, 64 }, 1 },
                                   { ActionId::GENMAP, "cmdGenMap", "spelldata", { 0, 0, 64, 64 }, 1 },
                                   { ActionId::SAVESESSION, "cmdSaveSession", "spelldata", { 0, 0, 64, 64 }, 1 } };

void ActionManager::resetCooldowns()
{
    for( auto &&actionCooldown : actionsCooldown )
	{
		actionCooldown.start = 0;
		actionCooldown.duration = 0;
    }
}

ActionManager::ActionManager()
{
    AddAction( "_null", []() { return false; } );

    AddAction( "resetcooldowns", [this]() {
        resetCooldowns();
        return true;
    } );

    resetCooldowns();
}

void ActionManager::AddAction( std::string &&name, std::function<bool()> &&action )
{
	auto it = actions.find( name );
	if( it == actions.end() )
	{
        actions.emplace( std::move( name ), std::move( action ) );
    }
}

void ActionManager::RemoveAction( std::string &&name )
{
    auto it = actions.find( name );
    if( it != actions.end() )
    {
        actions.erase( it );
    }
}

void ActionManager::Update()
{
    for( auto &&actionCooldown : actionsCooldown )
	{
        if( actionCooldown.start > 0 && actionCooldown.duration > 0 )
        {
            if( game.thisTime > ( actionCooldown.start + actionCooldown.duration ) )
            {
                actionCooldown.start = 0;
                actionCooldown.duration = 0;
            }
        }
	}
}

const ActionInfo &ActionManager::GetActionInfo( std::string_view name )
{
	for( auto &&info : actionsInfo )
	{
		if( info.name == name )
		{
			return info;
		}
	}

	return actionsInfo[0];
}

ActionCooldown ActionManager::GetActionCooldown( std::string_view name )
{
	const auto &actionInfo = GetActionInfo( name );

	const int index = int( actionInfo.id );

	return actionsCooldown[index];
}

void ActionManager::UseAction( std::string_view name )
{
	const auto &actionInfo = GetActionInfo( name );

	auto kv = actions.find( actionInfo.name );
    if( kv != actions.end() && !GetActionCooldown( name ).start )
	{
        if( kv->second() )
		{
			const int index = int( actionInfo.id );
            actionsCooldown[index].start = game.thisTime;
			actionsCooldown[index].duration = actionInfo.cooldown;
		}
	}
}

ActionManager actionManager;
