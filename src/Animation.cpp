#include "Animation.h"

void AnimationSet::Update( const unsigned int dt )
{
    time += dt;

    if( frames[currentFrame].frameTime < time )
    {
        time = 0;
        currentFrame += 1;

        if( currentFrame >= frames.size() )
        {
            currentFrame %= frames.size();
        }
    }
}
