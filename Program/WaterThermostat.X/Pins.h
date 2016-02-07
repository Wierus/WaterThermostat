#ifndef PINS_H
#define	PINS_H

#include <xc.h>

#define OutputPinDirection 0
#define InputPinDirection  1

#define LowPinLevel  0
#define HighPinLevel 1

// Выводы индикатора (слева направо)
#define Indicator0Pin          RA0
#define Indicator0PinDirection TRISA0
#define Indicator1Pin          RC1
#define Indicator1PinDirection TRISC1
#define Indicator2Pin          RC0
#define Indicator2PinDirection TRISC0

// Маски сегментов индикатора
#define SevenSegmentDisplaySAMask 0b00100000
#define SevenSegmentDisplaySBMask 0b01000000
#define SevenSegmentDisplaySCMask 0b00000010
#define SevenSegmentDisplaySDMask 0b00001000
#define SevenSegmentDisplaySEMask 0b00010000
#define SevenSegmentDisplaySFMask 0b10000000
#define SevenSegmentDisplaySGMask 0b00000001
#define SevenSegmentDisplaySHMask 0b00000100

// Выводы сдвигового регистра
#define SR74HC595ClockPin          RC3
#define SR74HC595ClockPinDirection TRISC3
#define SR74HC595DataPin           RA1
#define SR74HC595DataPinDirection  TRISA1

// Выводы энкодера
#define EncoderLeftPin            RA2
#define EncoderLeftPinDirection   TRISA2
#define EncoderRightPin           RC2
#define EncoderRightPinDirection  TRISC2
#define EncoderButtonPin          RC4
#define EncoderButtonPinDirection TRISC4

// Значение вывода кнопки энкодера при нажатом состоянии
#define EncoderButtonPressed 0

// Вывод шины 1-wire
#define OneWirePin          RC4
#define OneWirePinDirection TRISC4

// Выводы датчиков уровней
#define LowLevelSensorPin           RA3
#define LowLevelSensorPinDirection  TRISA3
#define HighLevelSensorPin          RA5
#define HighLevelSensorPinDirection TRISA5

// Значение вывода сработавшего датчика уровня (в воде)
#define LevelSensorActive 0

// Выводы реле
#define WaterRelayPin           RA4
#define WaterRelayPinDirection  TRISA4
#define HeaterRelayPin          RC5
#define HeaterRelayPinDirection TRISC5

// Значение выводов реле
#define RelayOff 0
#define RelayOn  1

#endif /* PINS_H */
