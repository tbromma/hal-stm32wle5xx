#include "halDelay.h"

namespace hal {

void __attribute__((weak)) DelayMs(uint16 milisec)
{
    REPORT_FAT("DelayMs function not implemented!");
}

}