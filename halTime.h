#pragma once
#include "common.h"

namespace hal {
    uint64 GetSystickUs();
    uint64 GetSystickMs();
    void DelayMs(uint32 milisec);
}