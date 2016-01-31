/** SevenSegmentDisplayDS18B20.c
 * v.1.0
 */

#include "SevenSegmentDisplayDS18B20.h"
#include "SevenSegmentDisplay.h"
#include "DS18B20.h"

void FillIndicators3WithDS18B20Temperature(unsigned char* indicatorValues) {
    if (DS18B20ResultGetTemperature == DS18B20PrecencePulseNotDetected) {
        FillIndicators3WithSymbolLine(indicatorValues);
    }
    else {
        if (DS18B20ShowValueIfReadError) {
            FillIndicators3WithNumber(indicatorValues, DS18B20TemperatureValue.sign, DS18B20TemperatureValue.integerPart);
        }
        else {
            FillIndicators3WithSymbolLine(indicatorValues);
        }
        if (DS18B20ResultGetTemperature != DS18B20OperationOK) {
            indicatorValues[2] |= SymbolDot;
        }
    }
}
