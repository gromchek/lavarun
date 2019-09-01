#ifndef MISC_H
#define MISC_H

#include <string>
#include <type_traits>

constexpr int R_VIRTUAL_WIDTH = 1280;
constexpr int R_VIRTUAL_HEIGHT = 1024;
constexpr bool R_FULLSCREEN = true;

const std::string baseDir( "base//" );

constexpr float TILEMAP_TILE_SIZE = 64.0f;
constexpr size_t TILEMAP_TILES_W_NUM = 10;
constexpr size_t TILEMAP_TILES_H_NUM = 10;
constexpr float TILEMAP_WIDTH_SIZE = TILEMAP_TILES_W_NUM * TILEMAP_TILE_SIZE;
constexpr float TILEMAP_HEIGHT_SIZE = TILEMAP_TILES_H_NUM * TILEMAP_TILE_SIZE;
constexpr size_t TILEMAP_TILES_NUM = TILEMAP_TILES_W_NUM * TILEMAP_TILES_H_NUM;
constexpr unsigned int TILEMAP_START_TILE = 0;
constexpr unsigned int TILEMAP_END_TILE = TILEMAP_TILES_NUM - 1;

struct FadingLine
{
    unsigned int thinktime;
    std::string message;
    unsigned char alpha;
};

template <typename T, unsigned int N> constexpr unsigned int size( const T ( & )[N] )
{
    return N;
}

template <typename T, typename = typename std::enable_if<std::is_enum<T>::value, T>::type>
bool inRange( const T &min, const T &max, const int val )
{
    const auto _min = int( min );
    const auto _max = int( max );
    return _min < val && val < _max;
}

inline unsigned int djb2( unsigned char *buffer, unsigned int size )
{
    unsigned int hash = 5381;

    for( auto i = 0u; i < size; i++ )
    {
        int c = buffer[i];
        hash = ( ( hash << 5 ) + hash ) + c;
    }

    return hash;
}

inline void toUpperCase( std::string &str )
{
    for( auto &c : str )
    {
        c = char( toupper( int( c ) ) );
    }
}

inline void toLowerCase( std::string &str )
{
    for( auto &c : str )
    {
        c = char( tolower( int( c ) ) );
    }
}

#endif // MISC_H
