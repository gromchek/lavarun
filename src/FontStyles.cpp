#include "FontStyles.h"

#include "ResourceManager.h"

void ApplyFontStyle( sf::Text &text, const FontStyle &fontStyle )
{
    text.setFont( resourceManager.GetFontByName( fontStyle.fontName ) );

    text.setCharacterSize( fontStyle.characterSize );

    text.setFillColor( fontStyle.fillColor );

    text.setOutlineThickness( fontStyle.outlineThickness );
    text.setOutlineColor( fontStyle.outlineColor );

    text.setStyle( fontStyle.style );
}
