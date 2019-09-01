#include "Ninepatch.h"

#include <SFML/Graphics/RenderTarget.hpp>

#include "ResourceManager.h"

void Ninepatch::updatePatch()
{
    if( !texture )
    {
        return;
    }

    sf::Vertex vertexData[16];

    sf::FloatRect rect( 0.0f, 0.0f, size.x, size.y );

    const float texCropW[3] = { texture->getSize().x * 0.25f, texture->getSize().x * 0.5f,
                                texture->getSize().x * 0.25f };
    const float texCropH[3] = { texture->getSize().y * 0.25f, texture->getSize().y * 0.5f,
                                texture->getSize().y * 0.25f };

    const float rectW[4] = { 0.0f, texCropW[0], rect.width - texCropW[0], rect.width };
    const float rectH[4] = { 0.0f, texCropH[0], rect.height - texCropH[0], rect.height };

    for( int i = 0; i < 16; i++ )
    {
        const int column = i % 4;
        const int row = i / 4;

        vertexData[i].position.x = rect.left + rectW[column];
        vertexData[i].position.y = rect.top + rectH[row];
    }

    const float cutW[4] = { 0.0f, texCropW[0], texCropW[0] + texCropW[1], texCropW[0] + texCropW[1] + texCropW[2] };
    const float cutH[4] = { 0.0f, texCropH[0], texCropH[0] + texCropH[1], texCropH[0] + texCropH[1] + texCropH[2] };

    for( int i = 0; i < 16; i++ )
    {
        const int column = i % 4;
        const int row = i / 4;

        vertexData[i].texCoords.x = cutW[column];
        vertexData[i].texCoords.y = cutH[row];
    }

    const int indices[] = { 0, 4, 1, 5, 2, 6, 3, 7, 7, 11, 6, 10, 5, 9, 4, 8, 8, 12, 9, 13, 10, 14, 11, 15 };

    for( auto i = 0u; i < verts.getVertexCount(); i++ )
    {
        verts[i].position = vertexData[indices[i]].position;
        verts[i].texCoords = vertexData[indices[i]].texCoords;
        verts[i].color = color;
    }
}

void Ninepatch::updateColor()
{
    for( auto i = 0u; i < verts.getVertexCount(); i++ )
    {
        verts[i].color = color;
    }
}

Ninepatch::Ninepatch( const sf::Vector2f &patchSize, const sf::Texture *newTexture ) :
    size( patchSize ),
    texture( newTexture ),
    color( sf::Color::White )
{
    verts.setPrimitiveType( sf::PrimitiveType::TrianglesStrip );
    verts.resize( 24 );

    updatePatch();
}

void Ninepatch::setFillColor( const sf::Color &fillColor )
{
    color = fillColor;

    updateColor();
}

void Ninepatch::setTexture( const sf::Texture *newTexture )
{
    if( !newTexture || texture == newTexture )
    {
        return;
    }

    texture = newTexture;

    updatePatch();
}

void Ninepatch::setSize( const sf::Vector2f &newSize )
{
    if( size == newSize || newSize.x <= 0.0f || newSize.y <= 0.0f )
    {
        return;
    }

    size = newSize;

    updatePatch();
}

void Ninepatch::draw( sf::RenderTarget &target, sf::RenderStates states ) const
{
    if( !texture )
    {
        return;
    }

    states.transform *= getTransform();
    states.texture = texture;

    target.draw( verts, states );
}
