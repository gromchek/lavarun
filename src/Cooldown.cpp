#include "Cooldown.h"
#include "Game.h"
#include "FontStyles.h"

#include <cmath>

#include <SFML/Graphics/RenderTarget.hpp>

#include <optional>

// http://www.vbforums.com/showthread.php?726071-World-of-Warcraft-Spell-Cooldown-Effect-VB-Net-2008-2010-amp-VB6-DirectX

const float DEG2RAD = M_PI / 180.0f;

const int MS_TO_FLOAT_FORMAT = 1000;

std::optional<sf::Vector2f> LineIntersect( const sf::Vector2f &rot, const sf::Vector2f &pos, const sf::Vector2f &clipX,
                                           const sf::Vector2f &clipY )
{
    float d = ( ( clipY.y - clipX.y ) * ( pos.x - rot.x ) ) - ( ( clipY.x - clipX.x ) * ( pos.y - rot.y ) );

    float n_a = ( ( clipY.x - clipX.x ) * ( rot.y - clipX.y ) ) - ( ( clipY.y - clipX.y ) * ( rot.x - clipX.x ) );
    float n_b = ( ( pos.x - rot.x ) * ( rot.y - clipX.y ) ) - ( ( pos.y - rot.y ) * ( rot.x - clipX.x ) );

    if( d == 0.0f )
    {
        return std::nullopt;
    }

    float ua = n_a / d;
    float ub = n_b / d;

    if( ( ua >= 0.0f ) && ( ua <= 1.0f ) && ( ub >= 0.0f ) && ( ub <= 1.0f ) )
    {
        return { { rot.x + ua * ( pos.x - rot.x ), rot.y + ua * ( pos.y - rot.y ) } };
    }

    return std::nullopt;
}

void Cooldown::initClipRegion()
{
    const float X = size / 2.0f;
    const float Y = size / 2.0f;

    clipRegion[0] = sf::Vector2f( X - size, Y - size );
    clipRegion[1] = sf::Vector2f( X + size, Y - size );
    clipRegion[2] = sf::Vector2f( X - size, Y - ( size / 2.0f ) );
    clipRegion[3] = sf::Vector2f( X + size, Y - ( size / 2.0f ) );

    clipRegion[4] = sf::Vector2f( X + ( size / 2.0f ), Y - size );
    clipRegion[5] = sf::Vector2f( X + size, Y - size );
    clipRegion[6] = sf::Vector2f( X + ( size / 2.0f ), Y + size );
    clipRegion[7] = sf::Vector2f( X + size, Y + size );

    clipRegion[8] = sf::Vector2f( X - size, Y + ( size / 2.0f ) );
    clipRegion[9] = sf::Vector2f( X + size, Y + ( size / 2.0f ) );
    clipRegion[10] = sf::Vector2f( X - size, Y + size );
    clipRegion[11] = sf::Vector2f( X + size, Y + size );

    clipRegion[12] = sf::Vector2f( X - size, Y - size );
    clipRegion[13] = sf::Vector2f( X - ( size / 2.0f ), Y - size );
    clipRegion[14] = sf::Vector2f( X - size, Y + size );
    clipRegion[15] = sf::Vector2f( X - ( size / 2.0f ), Y + size );
}

void Cooldown::updateVertex()
{
    const float X = size / 2.0f;
    const float Y = size / 2.0f;
    const float halfSize = size / 2.0f;

    sf::Vector2f rot;

    if( angle < 90.0f )
    {
        rot.x = ( X + ( 0.0f * cosf( angle * DEG2RAD ) ) - ( ( -size ) * sinf( angle * DEG2RAD ) ) );
        rot.y = ( Y + ( 0.0f * sinf( angle * DEG2RAD ) ) + ( ( -size ) * cosf( angle * DEG2RAD ) ) );

        if( angle >= 0.0f && angle < 45.0f )
        {
            if( auto clipped = LineIntersect( rot, { X, Y }, clipRegion[2], clipRegion[3] ) )
            {
                verts[0].position = sf::Vector2f( clipped->x, clipped->y );
                verts[1].position = sf::Vector2f( X + halfSize, Y - halfSize );
                verts[2].position = sf::Vector2f( X, Y );

                verts[3].position = sf::Vector2f( X + halfSize, Y - halfSize );
                verts[4].position = sf::Vector2f( X + halfSize, Y );
                verts[5].position = sf::Vector2f( X, Y );

                verts[6].position = sf::Vector2f( X + halfSize, Y );
                verts[7].position = sf::Vector2f( X + halfSize, Y + halfSize );
                verts[8].position = sf::Vector2f( X, Y );

                verts[9].position = sf::Vector2f( X + halfSize, Y + halfSize );
                verts[10].position = sf::Vector2f( X, Y + halfSize );
                verts[11].position = sf::Vector2f( X, Y );

                verts[12].position = sf::Vector2f( X, Y + halfSize );
                verts[13].position = sf::Vector2f( X - halfSize, Y + halfSize );
                verts[14].position = sf::Vector2f( X, Y );

                verts[15].position = sf::Vector2f( X - halfSize, Y + halfSize );
                verts[16].position = sf::Vector2f( X - halfSize, Y );
                verts[17].position = sf::Vector2f( X, Y );

                verts[18].position = sf::Vector2f( X - halfSize, Y );
                verts[19].position = sf::Vector2f( X - halfSize, Y - halfSize );
                verts[20].position = sf::Vector2f( X, Y );

                verts[21].position = sf::Vector2f( X - halfSize, Y - halfSize );
                verts[22].position = sf::Vector2f( X, Y - halfSize );
                verts[23].position = sf::Vector2f( X, Y );
            }
        }
        else
        {
            if( angle >= 45.0f && angle < 90.0f )
            {
                if( auto clipped = LineIntersect( rot, { X, Y }, clipRegion[4], clipRegion[6] ) )
                {
                    verts[0].position = sf::Vector2f( clipped->x, clipped->y );
                    verts[1].position = sf::Vector2f( X + halfSize, Y );
                    verts[2].position = sf::Vector2f( X, Y );

                    verts[3].position = sf::Vector2f( X + halfSize, Y );
                    verts[4].position = sf::Vector2f( X + halfSize, Y + halfSize );
                    verts[5].position = sf::Vector2f( X, Y );

                    verts[6].position = sf::Vector2f( X + halfSize, Y + halfSize );
                    verts[7].position = sf::Vector2f( X, Y + halfSize );
                    verts[8].position = sf::Vector2f( X, Y );

                    verts[9].position = sf::Vector2f( X, Y + halfSize );
                    verts[10].position = sf::Vector2f( X - halfSize, Y + halfSize );
                    verts[11].position = sf::Vector2f( X, Y );

                    verts[12].position = sf::Vector2f( X - halfSize, Y + halfSize );
                    verts[13].position = sf::Vector2f( X - halfSize, Y );
                    verts[14].position = sf::Vector2f( X, Y );

                    verts[15].position = sf::Vector2f( X - halfSize, Y );
                    verts[16].position = sf::Vector2f( X - halfSize, Y - halfSize );
                    verts[17].position = sf::Vector2f( X, Y );

                    verts[18].position = sf::Vector2f( X - halfSize, Y - halfSize );
                    verts[19].position = sf::Vector2f( X, Y - halfSize );
                    verts[20].position = sf::Vector2f( X, Y );
                }
            }
        }
    }
    else
    {
        if( angle >= 90.0f && angle < 180.0f )
        {
            rot.x = ( ( X + ( ( size * cosf( ( angle - 90.0f ) * DEG2RAD ) ) -
                              ( 0.0f * sinf( ( angle - 90.0f ) * DEG2RAD ) ) ) ) );
            rot.y = ( ( Y + ( ( size * sinf( ( angle - 90.0f ) * DEG2RAD ) ) +
                              ( 0.0f * cosf( ( angle - 90.0f ) * DEG2RAD ) ) ) ) );

            if( ( angle >= 90.0f ) && ( angle < 135.0f ) )
            {
                if( auto clipped = LineIntersect( rot, { X, Y }, clipRegion[4], clipRegion[6] ) )
                {
                    verts[0].position = sf::Vector2f( clipped->x, clipped->y );
                    verts[1].position = sf::Vector2f( X + halfSize, Y + halfSize );
                    verts[2].position = sf::Vector2f( X, Y );

                    verts[3].position = sf::Vector2f( X + halfSize, Y + halfSize );
                    verts[4].position = sf::Vector2f( X, Y + halfSize );
                    verts[5].position = sf::Vector2f( X, Y );

                    verts[6].position = sf::Vector2f( X, Y + halfSize );
                    verts[7].position = sf::Vector2f( X - halfSize, Y + halfSize );
                    verts[8].position = sf::Vector2f( X, Y );

                    verts[9].position = sf::Vector2f( X - halfSize, Y + halfSize );
                    verts[10].position = sf::Vector2f( X - halfSize, Y );
                    verts[11].position = sf::Vector2f( X, Y );

                    verts[12].position = sf::Vector2f( X - halfSize, Y );
                    verts[13].position = sf::Vector2f( X - halfSize, Y - halfSize );
                    verts[14].position = sf::Vector2f( X, Y );

                    verts[15].position = sf::Vector2f( X - halfSize, Y - halfSize );
                    verts[16].position = sf::Vector2f( X, Y - halfSize );
                    verts[17].position = sf::Vector2f( X, Y );
                }
            }
            else if( ( angle >= 135.0f ) && ( angle < 180.0f ) )
            {
                if( auto clipped = LineIntersect( rot, { X, Y }, clipRegion[8], clipRegion[9] ) )
                {
                    verts[0].position = sf::Vector2f( clipped->x, clipped->y );
                    verts[1].position = sf::Vector2f( X, Y + halfSize );
                    verts[2].position = sf::Vector2f( X, Y );

                    verts[3].position = sf::Vector2f( X, Y + halfSize );
                    verts[4].position = sf::Vector2f( X - halfSize, Y + halfSize );
                    verts[5].position = sf::Vector2f( X, Y );

                    verts[6].position = sf::Vector2f( X - halfSize, Y + halfSize );
                    verts[7].position = sf::Vector2f( X - halfSize, Y );
                    verts[8].position = sf::Vector2f( X, Y );

                    verts[9].position = sf::Vector2f( X - halfSize, Y );
                    verts[10].position = sf::Vector2f( X - halfSize, Y - halfSize );
                    verts[11].position = sf::Vector2f( X, Y );

                    verts[12].position = sf::Vector2f( X - halfSize, Y - halfSize );
                    verts[13].position = sf::Vector2f( X, Y - halfSize );
                    verts[14].position = sf::Vector2f( X, Y );
                }
            }
        }
        else
        {
            if( ( angle >= 180.0f ) && ( angle <= 270.0f ) )
            {
                rot.x = ( ( X + ( ( 0.0f * cosf( ( angle - 180.0f ) * DEG2RAD ) ) -
                                  ( size * sinf( ( angle - 180.0f ) * DEG2RAD ) ) ) ) );
                rot.y = ( ( Y + ( ( 0.0f * sinf( ( angle - 180.0f ) * DEG2RAD ) ) +
                                  ( size * cosf( ( angle - 180.0f ) * DEG2RAD ) ) ) ) );

                if( ( angle >= 180.0f ) && ( angle < 225.0f ) )
                {
                    if( auto clipped = LineIntersect( rot, { X, Y }, clipRegion[8], clipRegion[9] ) )
                    {
                        verts[0].position = sf::Vector2f( clipped->x, clipped->y );
                        verts[1].position = sf::Vector2f( X - halfSize, Y + halfSize );
                        verts[2].position = sf::Vector2f( X, Y );

                        verts[3].position = sf::Vector2f( X - halfSize, Y + halfSize );
                        verts[4].position = sf::Vector2f( X - halfSize, Y );
                        verts[5].position = sf::Vector2f( X, Y );

                        verts[6].position = sf::Vector2f( X - halfSize, Y );
                        verts[7].position = sf::Vector2f( X - halfSize, Y - halfSize );
                        verts[8].position = sf::Vector2f( X, Y );

                        verts[9].position = sf::Vector2f( X - halfSize, Y - halfSize );
                        verts[10].position = sf::Vector2f( X, Y - halfSize );
                        verts[11].position = sf::Vector2f( X, Y );
                    }
                }
                else if( ( angle >= 225.0f ) && ( angle < 270.0f ) )
                {
                    if( auto clipped = LineIntersect( rot, { X, Y }, clipRegion[13], clipRegion[15] ) )
                    {
                        verts[0].position = sf::Vector2f( clipped->x, clipped->y );
                        verts[1].position = sf::Vector2f( X - halfSize, Y );
                        verts[2].position = sf::Vector2f( X, Y );

                        verts[3].position = sf::Vector2f( X - halfSize, Y );
                        verts[4].position = sf::Vector2f( X - halfSize, Y - halfSize );
                        verts[5].position = sf::Vector2f( X, Y );

                        verts[6].position = sf::Vector2f( X - halfSize, Y - halfSize );
                        verts[7].position = sf::Vector2f( X, Y - halfSize );
                        verts[8].position = sf::Vector2f( X, Y );
                    }
                }
            }
            else
            {
                rot.x = ( ( X + ( ( -size * cosf( ( angle - 270.0f ) * DEG2RAD ) ) -
                                  ( 0.0f * sinf( ( angle - 270.0f ) * DEG2RAD ) ) ) ) );
                rot.y = ( ( Y + ( ( -size * sinf( ( angle - 270.0f ) * DEG2RAD ) ) +
                                  ( 0.0f * cosf( ( angle - 270.0f ) * DEG2RAD ) ) ) ) );

                if( ( angle >= 270.0f ) && ( angle < 315.0f ) )
                {
                    if( auto clipped = LineIntersect( rot, { X, Y }, clipRegion[13], clipRegion[15] ) )
                    {
                        verts[0].position = sf::Vector2f( clipped->x, clipped->y );
                        verts[1].position = sf::Vector2f( X - halfSize, Y - halfSize );
                        verts[2].position = sf::Vector2f( X, Y );

                        verts[3].position = sf::Vector2f( X - halfSize, Y - halfSize );
                        verts[4].position = sf::Vector2f( X, Y - halfSize );
                        verts[5].position = sf::Vector2f( X, Y );
                    }
                }
                else if( ( angle >= 315.0f ) && ( angle < 360.0f ) )
                {
                    if( auto clipped = LineIntersect( rot, { X, Y }, clipRegion[2], clipRegion[3] ) )
                    {
                        verts[0].position = sf::Vector2f( clipped->x, clipped->y );
                        verts[1].position = sf::Vector2f( X, Y - halfSize );
                        verts[2].position = sf::Vector2f( X, Y );
                    }
                }
            }
        }
    }
}

void Cooldown::updateCountdown( const int start, const int duration )
{
    const int cd = ( ( start + duration ) - game.thisTime );

    if( cd <= MS_TO_FLOAT_FORMAT )
    {
        char buf[8];
        snprintf( buf, sizeof( buf ), "%2.1f", float( cd / 1000.0f ) );

        label.setFillColor( GAME_COOLDOWN_FONT_SOONISH.fillColor );
        label.setString( buf );
    }
    else
    {
        label.setFillColor( GAME_COOLDOWN_FONT_NORMAL.fillColor );
        label.setString( std::to_string( int( cd / 1000 ) + 1 ) );
    }

    label.setOrigin( { label.getLocalBounds().width / 2.0f, label.getLocalBounds().height / 2.0f } );

    label.setPosition( { size / 2.0f, size / 4.0f } );
}

Cooldown::Cooldown( const float btnSize ) : size( btnSize ), angle( 0.0 ), onCooldown( false )
{
    verts.setPrimitiveType( sf::PrimitiveType::Triangles );
    verts.resize( 24 );

    initClipRegion();

    ApplyFontStyle( label, GAME_COOLDOWN_FONT_NORMAL );
}

void Cooldown::SetCooldown( const int start, const int duration )
{
    if( game.thisTime > ( start + duration ) )
    {
        onCooldown = false;
        return;
    }

    updateCountdown( start, duration );

    const float a = ( game.thisTime - start ) / float( duration );

    if( a > 1.0f )
    {
        onCooldown = false;
        return;
    }

    angle = a * 360.0f + 0.001f;
    onCooldown = true;
}

void Cooldown::Update()
{
    if( !onCooldown )
    {
        return;
    }

    for( auto i = 0u; i < verts.getVertexCount(); i++ )
    {
        verts[i].position = sf::Vector2f( 0.0f, 0.0f );
        verts[i].color = sf::Color( 0, 0, 0, 153 );
    }

    updateVertex();
}

void Cooldown::draw( sf::RenderTarget &target, sf::RenderStates states ) const
{
    if( !onCooldown )
    {
        return;
    }

    states.transform *= getTransform();

    target.draw( verts, states );
    target.draw( label, states );
}
