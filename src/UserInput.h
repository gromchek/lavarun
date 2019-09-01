#ifndef USERINPUT_H
#define USERINPUT_H

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

class UserInput
{
private:
	int keyState[256];

	int mouseX;
	int mouseY;

	bool anyKey;

public:
    UserInput();
	~UserInput() = default;

	void ReleaseAllKey();

	void SetMouseKey( const int key, const bool val );
	void SetKey( const int key, const bool val );
    void SetMousePosition( const sf::Vector2i &m );

    sf::Vector2i GetMousePosition() const;
	bool GetMouseState( const int key ) const;
	bool GetKeyState( const int key ) const;

	bool AnyKeyPressed() const;
};

inline void UserInput::SetMouseKey( const int key, const bool val )
{
	const int mkey = sf::Keyboard::KeyCount + key + 1;
	SetKey( mkey, val );
	anyKey = false;
}

inline void UserInput::SetKey( const int key, const bool val )
{
	keyState[key] = val;
	anyKey = val;
}

inline void UserInput::SetMousePosition( const sf::Vector2i &m )
{
    mouseX = m.x;
    mouseY = m.y;
}

inline sf::Vector2i UserInput::GetMousePosition() const
{
    return { mouseX, mouseY };
}

inline bool UserInput::GetMouseState( const int key ) const
{
	const int mkey = sf::Keyboard::KeyCount + key + 1;
	return GetKeyState( mkey );
}

inline bool UserInput::GetKeyState( const int key ) const
{
	return keyState[key];
}

inline bool UserInput::AnyKeyPressed() const
{
    return anyKey;
}

extern UserInput userInput;

#endif // USERINPUT_H
