#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>

#include <optional>

#include <vector>
#include <string>


class ResourceManager
{
private:
	struct Texture
	{
		std::string name;
		sf::Texture handle;

		Texture() = default;
        explicit Texture( std::string_view textureName ) : name( textureName )
		{
		}
	};

    struct Font
    {
        std::string name;
        sf::Font handle;

        Font() = default;
        explicit Font( std::string_view fontName ) : name( fontName )
        {
        }
    };

	Texture defaultTex;
	std::vector<Texture> textures;
    Font defaultFont;
    std::vector<Font> fonts;

public:
    ResourceManager() = default;
    ~ResourceManager() = default;

	void Init();

    std::optional<unsigned int> LoadTexture( std::string_view name, const bool smooth = true,
                                             const bool repeat = true );
    std::optional<unsigned int> LoadFont( std::string_view name );

    const sf::Texture &GetTextureByName( std::string_view name ) const;
    const sf::Texture &GetTextureByIndex( const std::optional<unsigned int> index ) const;

    const sf::Font &GetFontByName( std::string_view name ) const;
    const sf::Font &GetFontByIndex( const std::optional<unsigned int> index ) const;

    std::optional<unsigned int> GetTextureIndex( std::string_view name ) const;
    std::optional<unsigned int> GetFontIndex( std::string_view name ) const;

    void ValidateTextureIndex( std::optional<unsigned int> &index, std::string_view name ) const;
    void ValidateFontIndex( std::optional<unsigned int> &index, std::string_view name ) const;
};

extern ResourceManager resourceManager;

#endif // TEXTUREMANAGER_H
