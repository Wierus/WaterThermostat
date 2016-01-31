/** SR74HC595.c
 * v.1.0
 */

#include "SR74HC595.h"

void SR74HC595Initialize() {
    SR74HC595ClockPinDirection = OutputPinDirection;
    SR74HC595DataPinDirection  = OutputPinDirection;
}

void SR74HC595SendByte(unsigned char value) {
    unsigned char index = 8;
    while (1) {
        index--;
        SR74HC595ClockPin = LowPinLevel;
        SR74HC595DataPin = (value & (1 << index)) ? LowPinLevel : HighPinLevel;
        SR74HC595ClockPin = HighPinLevel;
        if (!index) {
            SR74HC595ClockPin = LowPinLevel;
            SR74HC595ClockPin = HighPinLevel;
            break;
        }
    }
}
