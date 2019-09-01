#ifndef GAMEMODE_H
#define GAMEMODE_H

#include <vector>
#include <memory>

#include <optional>

constexpr unsigned int GAME_WARMUP_TIME = 10000;
constexpr unsigned int GAME_PREPARE_TIME = 25000;
constexpr unsigned int GAME_ENDREVIEW_TIME = 30000;
constexpr unsigned int GAME_NEXT_ROUND_TIME = 3000;
constexpr unsigned int GAME_SWITCH_TO_REVIEW = 1000;
constexpr unsigned int GAME_MAX_ROUNDS = 5;

class MenuScreen_HUD;

enum class ModeType : unsigned int
{
    NORMAL,
    REVERSE,
    FREE
};

struct ModeParameters
{
    ModeType type = ModeType::FREE;

    unsigned int prepTime = 0;

    unsigned int numOfRounds = 0;
};

using RoundTime = std::pair<unsigned int, unsigned int>;
using SessionData = std::array<unsigned int, GAME_MAX_ROUNDS>;

struct SessionInfo
{
    unsigned int roundNum = 0;
    RoundTime roundTime;
    unsigned int bestRoundIndex = 0;

    SessionData data;

    SessionInfo();

    unsigned int LastRoundTime() const;
    void Clear();
};

inline SessionInfo::SessionInfo()
{
    Clear();
}

inline unsigned int SessionInfo::LastRoundTime() const
{
    return roundTime.second - roundTime.first;
}

inline void SessionInfo::Clear()
{
    roundNum = 0;
    roundTime.first = 0;
    roundTime.second = 0;
    bestRoundIndex = 0;

    std::fill( data.begin(), data.end(), 0 );
}

enum class ModeState
{
    INACTIVE,
    WARMUP,
    PREPARE,
    ROUNDON,
    COMPLETEROUND,
    GAMEREVIEW
};


class GameMode
{
private:
    ModeState current = ModeState::INACTIVE;
    ModeState next = ModeState::INACTIVE;
    int timeToNextState = 0;

    bool three = false;
    bool two = false;
    bool one = false;

    SessionInfo info;

    ModeParameters params;

    void newState( const ModeState newState );
    void updateWarmupTimer();

    void prepare();
    void roundStart();
    void roundComplete();
    void endGameReview();

    void updateSessionInfo();

    std::weak_ptr<MenuScreen_HUD> hudScreen;

    std::vector<std::pair<std::string, ModeType>> modeNameString;

public:
    GameMode();
    ~GameMode() = default;

    void SetHUD( std::shared_ptr<MenuScreen_HUD> screen );

    void Restart( const ModeType mode );

    void RoundStart();
    void RoundEnd();

    void Update();

    unsigned int GetCurrentRoundNum() const;

    ModeState GetState() const;
    bool PlayerCanUseAction() const;

    bool OnRound() const;

    SessionData GetSessionData() const;
    static std::optional<unsigned int> GetBestTime( const unsigned int lastBest, const SessionData &sessionData,
                                                    const ModeType type );

    const ModeParameters &GetModeParams() const;

    std::string GetName( const ModeType type ) const;
};

inline void GameMode::SetHUD( std::shared_ptr<MenuScreen_HUD> screen )
{
    hudScreen = screen;
}

inline unsigned int GameMode::GetCurrentRoundNum() const
{
    return info.roundNum;
}

inline ModeState GameMode::GetState() const
{
    return current;
}

inline bool GameMode::PlayerCanUseAction() const
{
    if( current != ModeState::ROUNDON && params.type == ModeType::FREE )
    {
        return true;
    }

    return current == ModeState::PREPARE && params.type != ModeType::FREE;
}

inline bool GameMode::OnRound() const
{
    return current == ModeState::ROUNDON;
}

inline const ModeParameters &GameMode::GetModeParams() const
{
    return params;
}

inline std::string GameMode::GetName( const ModeType type ) const
{
    for( const auto &mode : modeNameString )
    {
        if( mode.second == type )
        {
            return mode.first;
        }
    }

    return { "???" };
}

#endif // GAMEMODE_H
