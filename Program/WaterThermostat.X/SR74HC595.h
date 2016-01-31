/** SR74HC595.h
 * v.1.0
 */

#ifndef SR74HC595_H
#define	SR74HC595_H

#include "Pins.h"

#ifndef SR74HC595ClockPin
    #error "SR74HC595ClockPin symbol is not defined, you should define it in file Pins.h before using this library"
#endif

#ifndef SR74HC595ClockPinDirection
    #error "SR74HC595ClockPinDirection symbol is not defined, you should define it in file Pins.h before using this library"
#endif

#ifndef SR74HC595DataPin
    #error "SR74HC595DataPin symbol is not defined, you should define it in file Pins.h before using this library"
#endif

#ifndef SR74HC595DataPinDirection
    #error "SR74HC595DataPinDirection symbol is not defined, you should define it in file Pins.h before using this library"
#endif

/** Инициализирует сдвиговый регистр.
 */
void SR74HC595Initialize();

/** Отправляет байт в сдвиговый регистр, начиная со старшего бита.
 * @param byte Отправляемый байт.
 */
void SR74HC595SendByte(unsigned char value);

#endif	/* SR74HC595_H */
