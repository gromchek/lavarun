#ifndef KEYBINDS_H
#define KEYBINDS_H

#include <string>
#include <array>
#include <map>

using Binds = std::map<unsigned int, std::string>;
using KeyNames = std::array<std::string, 256>;

class KeyBinds
{
private:
    KeyNames keyNames;
    Binds binds;

    void initKeyNames();
    void loadDefaultBinds();

public:
	KeyBinds();
    ~KeyBinds() = default;

    void SetBind( std::string_view keyName, std::string_view command );
    unsigned int GetBind( std::string_view command ) const;

    void ReadBinds( const std::string &fileName );
    void WriteBinds( const std::string &fileName ) const;

    unsigned int StringToKeyName( std::string_view str ) const;
    std::string_view KeyNameToString( const unsigned int scancode ) const;
};

extern KeyBinds keyBinds;

#endif // KEYBINDS_H
