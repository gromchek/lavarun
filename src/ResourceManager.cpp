#include <iostream>

#include "Misc.h"
#include "ResourceManager.h"


void ResourceManager::Init()
{
	sf::Image defaultImage;
	defaultImage.create( 64, 64, { 0, 255, 0 } );

	defaultTex.handle.loadFromImage( defaultImage );
	defaultTex.handle.setRepeated( true );
    defaultTex.name = "_defaultTex";

    textures.reserve( 16 );
    textures.push_back( defaultTex );

    defaultFont.handle.loadFromFile( baseDir + "alegreya.otf" );
    defaultFont.name = "_defaultFont";
    fonts.reserve( 2 );
    fonts.push_back( defaultFont );
}

std::optional<unsigned int> ResourceManager::LoadTexture( std::string_view name, const bool smooth, const bool repeat )
{
	auto idx = GetTextureIndex( name );

    if( idx )
	{
		return idx;
	}

	textures.push_back( Texture( name ) );

    if( !textures.back().handle.loadFromFile( baseDir + std::string( name ) + ".png" ) )
	{
        std::cout << "ResourceManager: Error load texture: " << name << '\n';
        textures.back().handle = defaultTex.handle;
        return std::nullopt;
	}

	textures.back().handle.setSmooth( smooth );
	textures.back().handle.setRepeated( repeat );

	idx = textures.size() - 1;

    return idx;
}

std::optional<unsigned int> ResourceManager::LoadFont( std::string_view name )
{
    auto idx = GetFontIndex( name );

    if( idx )
    {
        return idx;
    }

    fonts.push_back( Font( name ) );

    if( !fonts.back().handle.loadFromFile( baseDir + std::string( name ) + ".otf" ) )
    {
        std::cout << "ResourceManager: Error load font: " << name << '\n';
        fonts.back().handle = defaultFont.handle;
        return std::nullopt;
    }

    idx = fonts.size() - 1;

    return idx;
}

const sf::Texture &ResourceManager::GetTextureByName( std::string_view name ) const
{
	for( const auto &texture : textures )
	{
		if( texture.name == name )
		{
			return texture.handle;
		}
	}

	return defaultTex.handle;
}

const sf::Texture &ResourceManager::GetTextureByIndex( const std::optional<unsigned int> index ) const
{
    if( index )
    {
        if( *index < textures.size() )
        {
            return textures[*index].handle;
        }

        return defaultTex.handle;
    }

    return defaultTex.handle;
}

const sf::Font &ResourceManager::GetFontByName( std::string_view name ) const
{
    for( const auto &font : fonts )
    {
        if( font.name == name )
        {
            return font.handle;
        }
    }

    return defaultFont.handle;
}

const sf::Font &ResourceManager::GetFontByIndex( const std::optional<unsigned int> index ) const
{
    if( index )
    {
        if( *index < fonts.size() )
        {
            return fonts[*index].handle;
        }

        return defaultFont.handle;
    }

    return defaultFont.handle;
}

std::optional<unsigned int> ResourceManager::GetTextureIndex( std::string_view name ) const
{
	for( auto i = 0u; i < textures.size(); i++ )
	{
		if( textures[i].name == name )
		{
            return { i };
		}
	}

    return std::nullopt;
}

std::optional<unsigned int> ResourceManager::GetFontIndex( std::string_view name ) const
{
    for( auto i = 0u; i < fonts.size(); i++ )
    {
        if( fonts[i].name == name )
        {
            return { i };
        }
    }

    return std::nullopt;
}

void ResourceManager::ValidateTextureIndex( std::optional<unsigned int> &index, std::string_view name ) const
{
    if( !index )
    {
        index = GetTextureIndex( name );
    }
}

void ResourceManager::ValidateFontIndex( std::optional<unsigned int> &index, std::string_view name ) const
{
    if( !index )
    {
        index = GetFontIndex( name );
    }
}

ResourceManager resourceManager;
