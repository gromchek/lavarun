#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include <string>
#include <SFML/Graphics/Rect.hpp>
#include <optional>

struct AnimFrame
{
    sf::IntRect frameCoord;
    unsigned int frameTime;
};


class AnimationSet
{
private:
    unsigned int currentFrame;
    unsigned int time;
    std::optional<unsigned int> texHandle;

    std::vector<AnimFrame> frames;

public:
    AnimationSet();
    ~AnimationSet() = default;

    void AddFrame( const AnimFrame &frame );

    void Update( const unsigned int dt );

    const AnimFrame &GetCurrentFrame() const;

    void SetTextureHandle( const std::optional<unsigned int> handle );
    std::optional<unsigned int> GetTextureHandle() const;
};

inline AnimationSet::AnimationSet() : currentFrame( 0 ), time( 0 )
{
    frames.reserve( 8 );
}

inline void AnimationSet::AddFrame( const AnimFrame &frame )
{
    frames.push_back( frame );
}

inline const AnimFrame &AnimationSet::GetCurrentFrame() const
{
    return frames[currentFrame];
}

inline void AnimationSet::SetTextureHandle( const std::optional<unsigned int> handle )
{
    texHandle = handle;
}

inline std::optional<unsigned int> AnimationSet::GetTextureHandle() const
{
    return texHandle;
}

#endif // ANIMATION_H
