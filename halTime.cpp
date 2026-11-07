#include "halTime.h"

namespace hal {

/*
uint64 GetSystickUs()
{
    // Needs to be implemented!
}
*/

uint64 GetSystickMs()
{
    return (GetSystickUs() / 1000);
}

void DelayMs(uint32 milisec)
{
    uint64 tickStart = GetSystickUs();
    while ((uint32)(GetSystickUs() - tickStart) < milisec);
}

}