#ifndef RND_H
#define RND_H

#include <random>

class Random
{
private:
    unsigned int seed;
    std::default_random_engine engine;
    static const int RND_MAX = 0xFFFFF;

public:
    Random();
    ~Random() = default;

    void SetSeed( const unsigned int newSeed );
    unsigned int GetSeed() const;

    int GetInt();
    int GetInt( const int max );
    int GetInt( const int min, const int max );
};

inline Random::Random() : seed( 0 )
{
    engine.seed( seed );
}

inline void Random::SetSeed( const unsigned int newSeed )
{
    seed = newSeed;
    engine.seed( seed );
}

inline unsigned int Random::GetSeed() const
{
    return seed;
}

inline int Random::GetInt()
{
    return std::uniform_int_distribution<int>( 0, RND_MAX )( engine );
}

inline int Random::GetInt( const int max )
{
    if( max == 0 )
    {
        return 0;
    }

    return GetInt() % max;
}

inline int Random::GetInt( const int min, const int max )
{
    if( max == 0 || min >= max )
    {
        return 0;
    }

    return std::uniform_int_distribution<int>( min, ( max >= RND_MAX ) ? RND_MAX : max )( engine );
}

#endif // RND_H
