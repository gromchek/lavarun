#include "GameMode.h"
#include "Game.h"
#include "FontStyles.h"
#include "MenuHandler.h"


const ModeParameters gameModeParams[] = { { ModeType::NORMAL, GAME_PREPARE_TIME, GAME_MAX_ROUNDS },
                                          { ModeType::REVERSE, GAME_PREPARE_TIME, GAME_MAX_ROUNDS },
                                          { ModeType::FREE, 0, 0 } };


void GameMode::updateWarmupTimer()
{
    if( hudScreen.expired() )
    {
        return;
    }

    auto hud = hudScreen.lock();

    if( params.type == ModeType::FREE )
    {
        hud->UpdateMultiboardTitle( GetName( ModeType::FREE ) );
        return;
    }

    const int timeLeft = ( timeToNextState - game.thisTime ) / 1000 + 1;

    if( timeLeft > 3 )
    {
        hud->UpdateMultiboardTitle( "Game begins in " + std::to_string( timeLeft ) );
        return;
    }

    hud->UpdateMultiboardTitle( "" );

    if( timeLeft == 3 && !three )
    {
        hud->ShowWarmupText( true, std::to_string( timeLeft ) );
        three = true;
    }
    else if( timeLeft == 2 && !two )
    {
        hud->ShowWarmupText( true, std::to_string( timeLeft ) );
        two = true;
    }
    else if( timeLeft == 1 && !one )
    {
        hud->ShowWarmupText( true, std::to_string( timeLeft ) );
        one = true;
    }

    if( !timeLeft && three && two && one )
    {
        hud->ShowWarmupText( false, "" );
    }
}

void GameMode::prepare()
{
    if( hudScreen.expired() )
    {
        return;
    }

    auto hud = hudScreen.lock();

    hud->UpdateCountdownTimer( game.thisTime, game.thisTime + params.prepTime );

    hud->UpdateMultiboardTitle( "Round " + std::to_string( GetCurrentRoundNum() + 1 ) + " of " +
                                std::to_string( params.numOfRounds ) );

    if( three && two && one )
    {
        hud->ShowWarmupText( false, "" );
    }
}

void GameMode::roundStart()
{
    if( hudScreen.expired() )
    {
        return;
    }

    auto hud = hudScreen.lock();

    hud->UpdateRunnerTimer( game.thisTime, game.thisTime + 40000 );
}

void GameMode::roundComplete()
{
    if( hudScreen.expired() )
    {
        return;
    }

    auto hud = hudScreen.lock();

    hud->UpdateRunnerTimer( game.thisTime, game.thisTime );

    const float time = float( info.LastRoundTime() ) / 1000.0f;

    updateSessionInfo();

    char buff[32];
    snprintf( buff, sizeof( buff ), "Round %u finished in %5.3f sec.", info.roundNum, time );

    hud->AddMessage( std::string( buff ) );

    std::fill_n( buff, 32, 0 );
    snprintf( buff, sizeof( buff ), "#%3u %8.3f sec", info.roundNum, time );

    hud->AddMultiboardRow( std::string( buff ) );

    if( params.type != ModeType::FREE )
    {
        hud->ClearMultiboardRowsColor();
        hud->SetMultiboardRowColor( info.bestRoundIndex, GAME_MULTIBOARD_FONT_BEST.fillColor );
    }
}

void GameMode::endGameReview()
{
    if( hudScreen.expired() )
    {
        return;
    }

    auto hud = hudScreen.lock();

    hud->AddMessage( "Game is over!" );
    hud->AddMessage( "Press ESC to exit" );
}

void GameMode::updateSessionInfo()
{
    info.roundNum += 1;

    if( params.type == ModeType::FREE )
    {
        return;
    }

    info.data[( info.roundNum - 1 ) % GAME_MAX_ROUNDS] = info.LastRoundTime();

    if( params.type == ModeType::NORMAL )
    {
        unsigned int max_time = 0;
        for( auto i = 0u; i < info.roundNum; i++ )
        {
            const auto time = info.data[i];
            if( time > max_time )
            {
                max_time = time;
                info.bestRoundIndex = i;
            }
        }
    }

    if( params.type == ModeType::REVERSE )
    {
        unsigned int min_time = 999999;
        for( auto i = 0u; i < info.roundNum; i++ )
        {
            const auto time = info.data[i];
            if( time < min_time && time > 0 )
            {
                min_time = time;
                info.bestRoundIndex = i;
            }
        }
    }
}

GameMode::GameMode() : current( ModeState::INACTIVE )
{
    modeNameString.reserve( 3 );

    modeNameString.emplace_back( "Free mode", ModeType::FREE );
    modeNameString.emplace_back( "Normal mode", ModeType::NORMAL );
    modeNameString.emplace_back( "Reverse mode", ModeType::REVERSE );
}

void GameMode::Restart( const ModeType mode )
{
    current = ModeState::WARMUP;
    next = ModeState::INACTIVE;

    params = *std::find_if( std::begin( gameModeParams ), std::end( gameModeParams ),
                            [&mode]( const auto &modeParams ) { return modeParams.type == mode; } );

    timeToNextState = GAME_WARMUP_TIME + game.thisTime;

    three = false;
    two = false;
    one = false;

    info.Clear();
}

void GameMode::RoundStart()
{
    if( params.type == ModeType::FREE )
    {
        newState( ModeState::ROUNDON );
    }
}

void GameMode::RoundEnd()
{
    info.roundTime.second = game.thisTime;

    newState( ModeState::COMPLETEROUND );
}

void GameMode::Update()
{
    if( game.thisTime > timeToNextState && next != ModeState::INACTIVE )
    {
        newState( next );
        next = ModeState::INACTIVE;
    }

    switch( current )
    {
    case ModeState::WARMUP:
        updateWarmupTimer();
        if( params.type == ModeType::FREE )
        {
            next = ModeState::INACTIVE;
        }
        else
        {
            next = ModeState::PREPARE;
        }
        break;

    case ModeState::PREPARE:
        next = ModeState::ROUNDON;
        break;

    case ModeState::COMPLETEROUND:
        break;

    case ModeState::GAMEREVIEW:
        next = ModeState::INACTIVE;
        break;

    default:
        break;
    }
}

SessionData GameMode::GetSessionData() const
{
    return info.data;
}

std::optional<unsigned int> GameMode::GetBestTime( const unsigned int lastBest, const SessionData &sessionData,
                                                   const ModeType type )
{
    const auto minmax = std::minmax_element( sessionData.begin(), sessionData.end() );

    switch( type )
    {
    case ModeType::NORMAL:
    {
        if( *minmax.second > lastBest )
        {
            return { *minmax.second };
        }
    }
    break;
    case ModeType::REVERSE:
    {
        if( *minmax.first < lastBest )
        {
            return { *minmax.first };
        }
    }
    break;
    default:
        break;
    }

    return std::nullopt;
}

void GameMode::newState( const ModeState newState )
{
    if( current == newState )
    {
        return;
    }

    switch( newState )
    {
    case ModeState::WARMUP:
        break;

    case ModeState::PREPARE:
        prepare();
        actionManager.UseAction( "cmdGenMap" );
        timeToNextState = params.prepTime + game.thisTime;
        next = ModeState::ROUNDON;
        break;

    case ModeState::ROUNDON:
        roundStart();
        actionManager.UseAction( "cmdStartRound" );

        info.roundTime.first = game.thisTime;
        info.roundTime.second = game.thisTime;
        break;

    case ModeState::COMPLETEROUND:
        roundComplete();
        timeToNextState = game.thisTime + GAME_NEXT_ROUND_TIME;

        if( params.type == ModeType::FREE )
        {
            next = ModeState::INACTIVE;
        }
        else
        {
            if( params.numOfRounds > GetCurrentRoundNum() )
            {
                next = ModeState::PREPARE;
            }
            else
            {
                timeToNextState = game.thisTime + GAME_SWITCH_TO_REVIEW;
                next = ModeState::GAMEREVIEW;
            }
        }
        break;

    case ModeState::GAMEREVIEW:
        actionManager.UseAction( "cmdSaveSession" );
        endGameReview();
        break;

    default:
        break;
    }

    current = newState;
}
