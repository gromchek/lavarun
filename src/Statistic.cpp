#include "Statistic.h"
#include <cassert>
#include <fstream>

#include "Misc.h"

#include <ctime>

void Statistic::loadDefault()
{
    stats[0].type = ModeType::NORMAL;
    stats[1].type = ModeType::REVERSE;

    for( auto i = 0u; i < STATS_MODES_NUM; i++ )
    {
        stats[i].data.lastPlayedTimestamp = 0;
        std::fill_n( stats[i].data.lastPlayedScore, GAME_MAX_ROUNDS, 0 );

        stats[i].data.bestPlayedTimestamp = 0;
        stats[i].data.bestPlayedScore = 0;
    }
}

Statistic::Statistic()
{
    loadDefault();
}

void Statistic::UpdateSessionData( const SessionData &sessionData, const ModeType type )
{
    for( auto &stat : stats )
    {
        if( stat.type == type )
        {
            stat.data.lastPlayedTimestamp = std::time( 0 );

            for( auto i = 0u; i < sessionData.size(); i++ )
            {
                stat.data.lastPlayedScore[i] = sessionData[i];
            }

            const auto bestTime = GameMode::GetBestTime( stat.data.bestPlayedScore, sessionData, type );

            if( bestTime )
            {
                stat.data.bestPlayedTimestamp = std::time( 0 );
                stat.data.bestPlayedScore = *bestTime;
            }
        }
    }
}

void Statistic::ReadStats( const std::string &fileName )
{
    std::ifstream file( fileName, std::ifstream::binary | std::ifstream::ate );

    if( !file )
    {
        return;
    }

    const auto fileSize = file.tellg();

    if( fileSize != ( sizeof( StatisticData ) * 2 + sizeof( unsigned int ) ) )
    {
        return;
    }

    file.seekg( 0, std::ifstream::beg );

    unsigned int fileChecksum = 0;

    file.read( (char *)( &fileChecksum ), sizeof( unsigned int ) );

    for( auto i = 0u; i < STATS_MODES_NUM; i++ )
    {
        file.read( (char *)( &stats[i].data.lastPlayedTimestamp ), sizeof( unsigned int ) );
        file.read( (char *)( stats[i].data.lastPlayedScore ), sizeof( unsigned int ) * GAME_MAX_ROUNDS );

        file.read( (char *)( &stats[i].data.bestPlayedTimestamp ), sizeof( unsigned int ) );
        file.read( (char *)( &stats[i].data.bestPlayedScore ), sizeof( unsigned int ) );
    }

    const auto checksum = djb2( reinterpret_cast<unsigned char *>( stats ), sizeof( ModeStatistic ) * 2 );

    if( fileChecksum != checksum )
    {
        loadDefault();
    }
}

void Statistic::WriteStats( const std::string &fileName )
{
    std::ofstream file( fileName, std::ofstream::out | std::ofstream::trunc | std::ofstream::binary );

    const auto checksum = djb2( reinterpret_cast<unsigned char *>( stats ), sizeof( ModeStatistic ) * 2 );

    file.write( (char *)( &checksum ), sizeof( unsigned int ) );

    for( auto i = 0u; i < STATS_MODES_NUM; i++ )
    {
        file.write( (char *)( &stats[i].data.lastPlayedTimestamp ), sizeof( unsigned int ) );
        file.write( (char *)( stats[i].data.lastPlayedScore ), sizeof( unsigned int ) * GAME_MAX_ROUNDS );

        file.write( (char *)( &stats[i].data.bestPlayedTimestamp ), sizeof( unsigned int ) );
        file.write( (char *)( &stats[i].data.bestPlayedScore ), sizeof( unsigned int ) );
    }
}
