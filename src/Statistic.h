#ifndef STATISTIC_H
#define STATISTIC_H

#include <string>
#include "GameMode.h"


constexpr unsigned int STATS_MODES_NUM = 2;

struct StatisticData
{
    unsigned int lastPlayedTimestamp;
    unsigned int lastPlayedScore[GAME_MAX_ROUNDS];

    unsigned int bestPlayedTimestamp;
    unsigned int bestPlayedScore;
};

struct ModeStatistic
{
    ModeType type;
    StatisticData data;
};

class Statistic
{
private:
    ModeStatistic stats[STATS_MODES_NUM];

    void loadDefault();

public:
    Statistic();
    ~Statistic() = default;

    void UpdateSessionData( const SessionData &sessionData, const ModeType type );

    void ReadStats( const std::string &fileName );
    void WriteStats( const std::string &fileName );

    const StatisticData &GetStatistic( const ModeType type ) const;
};

inline const StatisticData &Statistic::GetStatistic( const ModeType type ) const
{
    for( const auto &stat : stats )
    {
        if( stat.type == type )
        {
            return stat.data;
        }
    }
}


#endif // STATISTIC_H
