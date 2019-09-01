#include "UserInput.h"

#include <algorithm>

UserInput::UserInput() : mouseX( 0 ), mouseY( 0 )
{
    ReleaseAllKey();
}

void UserInput::ReleaseAllKey()
{
    std::fill_n( keyState, 256, 0 );

    anyKey = false;
}

UserInput userInput;
