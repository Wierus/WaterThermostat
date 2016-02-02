#include "Config.h"
#include "Delay.h"
#include "DS18B20.h"
#include "Math.h"
#include "Pins.h"
#include "SevenSegmentDisplay.h"
#include "SevenSegmentDisplayDS18B20.h"
#include "SR74HC595.h"
#include "TaskManager.h"

/** Время свечения одного индикатора (мс).
 */
#define IndicatorLightingTime 1

#define IndicatorCA

#ifdef IndicatorCA

    /** Значение вывода индикатора для его включения.
     */
    #define IndicatorOn  IndicatorCAOn

    /** Значение вывода индикатора для его выключения.
     */
    #define IndicatorOff IndicatorCAOff

    /** Значение вывода сегмента индикатора для его включения.
     */
    #define SegmentOn    SegmentCAOn

    /** Значение вывода сегмента индикатора для его выключения.
     */
    #define SegmentOff   SegmentCAOff

#else

    /** Значение вывода индикатора для его включения.
     */
    #define IndicatorOn  IndicatorCCOn

    /** Значение вывода индикатора для его выключения.
     */
    #define IndicatorOff IndicatorCCOff

    /** Значение вывода сегмента индикатора для его включения.
     */
    #define SegmentOn    SegmentCCOn

    /** Значение вывода сегмента индикатора для его выключения.
     */
    #define SegmentOff   SegmentCCOff

#endif

/** Количество индикаторов.
 */
#define IndicatorsCount 3

/** Символы на индикаторах (слева направо).
 */
unsigned char indicatorValues[IndicatorsCount];

/** Устанавливаемая разрешающая способность датчика DS18B20.
 */
#define DS18B20Resolution DS18B20Resolution12Bit

/** Состояния энкодера.
 */
typedef enum EncoderStates {
    
    EncoderLeft0Right0State,
    EncoderLeft0Right1State,
    EncoderLeft1Right0State,
    EncoderLeft1Right1State,

} EncoderStates;

/** Текущее состояние энкодера.
 */
EncoderStates encoderState = EncoderLeft0Right0State;

/** Необходимое время удержания кнопки энкодера для совершения действия (в единицах T_INT, параметр - в секундах).
 */
#define EncoderButtonPressedTime GetTaskManagerTimerTime(0.1)

/** Текущее время удержания кнопки энкодера (в единицах T_INT).
 */
unsigned char encoderButtonTimer = 0;

/** Флаг, показывающий, что кнопка энкодера уже была нажата и она удерживается дольше времени обычного нажатия.
 */
bit wasEncoderButtonPressed;

/** Время, прошедшее с момента предыдущего изменения значения энкодера, по истечению которого сбрасывается счетчик щелчков энкодера для выполнения действия (в единицах T_INT, параметр - в секундах).
 */
#define EncoderValueChangeTimeout GetTaskManagerTimerTime(0.5)

/** Текущее время, прошедшее с момента предыдущего изменения значения энкодера (в единицах T_INT).
 */
unsigned char encoderValueChangeTimer = 0;

/** Пороговое количество щелчков энкодера (изменений значения энкодера), при достижении которого выполняется действие.
 */
#define EncoderValueChangesThreshold 5

/** Текущее количество щелчков энкодера (изменений значения энкодера), по часовой (с плюсом) или против часовой (с минусом) стрелки.
 */
signed char encoderValueChanges = 0;

/** Минимальная желаемая температура.
 */
#define MinDesiredTemperature 5

/** Максимальная желаемая температура.
 */
#define MaxDesiredTemperature 55

/** Желаемая температура при включении (по умолчанию).
 */
#define DefaultDesiredTemperature 35

/** Гистерезис желаемой температуры.
 */
#define TemperatureHysteresis 1

/** Установленная желаемая температура.
 */
unsigned char desiredTemperature = DefaultDesiredTemperature;

/** Время бездействия, по истечению которого происходит возврат в режим отображения температуры (в единицах T_INT, параметр - в секундах).
 */
#define InactivityShowTemperatureModeTimeout GetTaskManagerTimerTime(5.0)

/** Текущее время бездействия в режиме, отличном от режима отображения температуры (в единицах T_INT).
 */
unsigned char inactivityShowTemperatureModeTimer = 0;

/** Время отображения значения температуры в режиме установки (при мигании) (в единицах T_INT, параметр - в секундах).
 */
#define SetTemperatureModeShowTime GetTaskManagerTimerTime(0.7)

/** Время скрытия значения температуры в режиме установки (при мигании) (в единицах T_INT, параметр - в секундах).
 */
#define SetTemperatureModeHideTime GetTaskManagerTimerTime(0.3)

/** Текущее время отображения или скрытия значения температуры в режиме установки (в единицах T_INT).
 */
unsigned char setTemperatureModeBlinkTimer = 0;

/** Флаг отображения значения температуры в режиме установки.
 */
bit isSetTemperatureModeShow;

/** Задержка до выполнения задачи DrawIndicatorsTask (в единицах T_INT, параметр - в секундах).
 */
#define DrawIndicatorsTaskDelay                       GetTaskManagerTimerTime(0.0)

/** Задержка до выполнения задачи FillIndicatorsTask (в единицах T_INT, параметр - в секундах).
 */
#define FillIndicatorsTaskDelay                       GetTaskManagerTimerTime(0.0)

/** Задержка до выполнения задачи FillIndicatorsWithCurrentTemperatureTask (в единицах T_INT, параметр - в секундах).
 */
#define FillIndicatorsWithCurrentTemperatureTaskDelay GetTaskManagerTimerTime(0.0)

/** Задержка до выполнения задачи FillIndicatorsWithDesiredTemperatureTask (в единицах T_INT, параметр - в секундах).
 */
#define FillIndicatorsWithDesiredTemperatureTaskDelay GetTaskManagerTimerTime(0.0)

/** Задержка до выполнения задачи ScanEncoderTask (в единицах T_INT, параметр - в секундах).
 */
#define ScanEncoderTaskDelay                          GetTaskManagerTimerTime(0.0)

/** Задержка до выполнения задачи ScanEncoderButtonTask (в единицах T_INT, параметр - в секундах).
 */
#define ScanEncoderButtonTaskDelay                    GetTaskManagerTimerTime(0.0)

/** Задержка до выполнения задачи ProcessWaterRelayTask (в единицах T_INT, параметр - в секундах).
 */
#define ProcessWaterRelayTaskDelay                    GetTaskManagerTimerTime(0.0)

/** Задержка до выполнения задачи ProcessHeaterRelayTask (в единицах T_INT, параметр - в секундах).
 */
#define ProcessHeaterRelayTaskDelay                   GetTaskManagerTimerTime(0.0)

/** Задержка до выполнения задачи RefreshDS18B20Task (в единицах T_INT, параметр - в секундах).
 */
#define RefreshDS18B20TaskDelay                       GetTaskManagerTimerTime(5.0)

/** Задержка до выполнения задачи ConvertTemperatureTask (в единицах T_INT, параметр - в секундах).
 */
#define ConvertTemperatureTaskDelay                   GetTaskManagerTimerTime(0.0)

/** Задержка до выполнения задачи GetTemperatureTask (в единицах T_INT, параметр - в секундах).
 */
#define GetTemperatureTaskDelay                       GetTaskManagerTimerTime(1.0)

/** Режимы работы.
 */
typedef enum WorkMode {
    
    /**
     * Режим отображения температуры.
     */
    ShowTemperatureMode,
    
    /**
     * Режим установки температуры.
     */
    SetTemperatureMode,

} WorkMode;

/** Текущий режим работы.
 */
unsigned char workMode = ShowTemperatureMode;

void ProcessEncoderTimersInInterrupt();

void ProcessOutputTimersInInterrupt();

void interrupt Interrupt() {
    if (TMR0IE && TMR0IF) {
        TMR0IF = 0;
        TimerService();
        ProcessEncoderTimersInInterrupt();
        ProcessOutputTimersInInterrupt();
    }
}

void ProcessEncoderTimersInInterrupt() {
    encoderButtonTimer++;
    encoderValueChangeTimer++;
}

void ProcessOutputTimersInInterrupt() {
    inactivityShowTemperatureModeTimer++;
    setTemperatureModeBlinkTimer++;
}

// Объявление задач и действий для них

void DrawIndicatorsTask();
void DrawIndicatorsAction();

void FillIndicatorsTask();
void FillIndicatorsAction();

void FillIndicatorsWithCurrentTemperatureTask();
void FillIndicatorsWithCurrentTemperatureAction();

void FillIndicatorsWithDesiredTemperatureTask();
void FillIndicatorsWithDesiredTemperatureAction();

void ScanEncoderTask();
void ScanEncoderAction();

void ScanEncoderButtonTask();
void ScanEncoderButtonAction();

void ProcessWaterRelayTask();
void ProcessWaterRelayAction();

void ProcessHeaterRelayTask();
void ProcessHeaterRelayAction();

void RefreshDS18B20Task();
void RefreshDS18B20Action();

void ConvertTemperatureTask();
void ConvertTemperatureAction();

void GetTemperatureTask();
void GetTemperatureAction();

// Определение задач

void DrawIndicatorsTask() {
    DrawIndicatorsAction();
    AddTask(DrawIndicatorsTask, DrawIndicatorsTaskDelay);
}

void FillIndicatorsTask() {
    FillIndicatorsAction();
    switch (workMode) {
        case ShowTemperatureMode: {
            AddTask(FillIndicatorsWithCurrentTemperatureTask, FillIndicatorsWithCurrentTemperatureTaskDelay);
            break;
        }
        case SetTemperatureMode: {
            AddTask(FillIndicatorsWithDesiredTemperatureTask, FillIndicatorsWithDesiredTemperatureTaskDelay);
            break;
        }
    }
    AddTask(FillIndicatorsTask, FillIndicatorsTaskDelay);
}

void FillIndicatorsWithCurrentTemperatureTask() {
    FillIndicatorsWithCurrentTemperatureAction();
}

void FillIndicatorsWithDesiredTemperatureTask() {
    FillIndicatorsWithDesiredTemperatureAction();
}

void ScanEncoderTask() {
    ScanEncoderAction();
    AddTask(ScanEncoderTask, ScanEncoderTaskDelay);
}

void ScanEncoderButtonTask() {
    ScanEncoderButtonAction();
    AddTask(ScanEncoderButtonTask, ScanEncoderButtonTaskDelay);
}

void ProcessWaterRelayTask() {
    ProcessWaterRelayAction();
    AddTask(ProcessWaterRelayTask, ProcessWaterRelayTaskDelay);
}

void ProcessHeaterRelayTask() {
    ProcessHeaterRelayAction();
    AddTask(ProcessHeaterRelayTask, ProcessHeaterRelayTaskDelay);
}

void RefreshDS18B20Task() {
    RefreshDS18B20Action();
    AddTask(RefreshDS18B20Task, RefreshDS18B20TaskDelay);
    AddTask(ConvertTemperatureTask, ConvertTemperatureTaskDelay);
}

void ConvertTemperatureTask() {
    ConvertTemperatureAction();
    AddTask(GetTemperatureTask, GetTemperatureTaskDelay);
}

void GetTemperatureTask() {
    GetTemperatureAction();
}

// Определение действий для задач

void DrawIndicatorsAction() {
    SR74HC595SendByte(indicatorValues[0]);
    Indicator0Pin = IndicatorOn;
    __delay_ms(IndicatorLightingTime);
    Indicator0Pin = IndicatorOff;
    SR74HC595SendByte(indicatorValues[1]);
    Indicator1Pin = IndicatorOn;
    __delay_ms(IndicatorLightingTime);
    Indicator1Pin = IndicatorOff;
    SR74HC595SendByte(indicatorValues[2]);
    Indicator2Pin = IndicatorOn;
    __delay_ms(IndicatorLightingTime);
    Indicator2Pin = IndicatorOff;
}

void FillIndicatorsAction() {
    if (workMode != ShowTemperatureMode) {
        if (inactivityShowTemperatureModeTimer >= InactivityShowTemperatureModeTimeout) {
            workMode = ShowTemperatureMode;
        }
    }
}

void FillIndicatorsWithCurrentTemperatureAction() {
    FillIndicators3WithDS18B20Temperature(indicatorValues);
}

void FillIndicatorsWithDesiredTemperatureAction() {
    if (isSetTemperatureModeShow) {
        FillIndicators3WithNumber(indicatorValues, 0, desiredTemperature);
        if (setTemperatureModeBlinkTimer >= SetTemperatureModeShowTime) {
            setTemperatureModeBlinkTimer = 0;
            isSetTemperatureModeShow = 0;
        }
    }
    else {
        FillIndicators3WithSymbolNull(indicatorValues);
        if (setTemperatureModeBlinkTimer >= SetTemperatureModeHideTime) {
            setTemperatureModeBlinkTimer = 0;
            isSetTemperatureModeShow = 1;
        }
    }
}

void ScanEncoderAction() {
    unsigned char isRotateCW = 0;
    unsigned char isRotateCCW = 0;
    switch (encoderState) {
        case EncoderLeft0Right0State: {
            if (EncoderLeftPin == 1) {
                encoderState = EncoderLeft1Right0State;
            }
            else if (EncoderRightPin == 1) {
                encoderState = EncoderLeft0Right1State;
            }
            break;
        }
        case EncoderLeft0Right1State: {
            if (EncoderLeftPin == 1) {
                encoderState = EncoderLeft1Right1State;
                isRotateCCW = 1;
            }
            else if (EncoderRightPin == 0) {
                encoderState = EncoderLeft0Right0State;
                isRotateCW = 1;
            }
            break;
        }
        case EncoderLeft1Right0State: {
            if (EncoderLeftPin == 0) {
                encoderState = EncoderLeft0Right0State;
                isRotateCCW = 1;
            }
            else if (EncoderRightPin == 1) {
                encoderState = EncoderLeft1Right1State;
                isRotateCW = 1;
            }
            break;
        }
        case EncoderLeft1Right1State: {
            if (EncoderLeftPin == 0) {
                encoderState = EncoderLeft0Right1State;
            }
            else if (EncoderRightPin == 0) {
                encoderState = EncoderLeft1Right0State;
            }
            break;
        }
    }
    switch (workMode) {
        case ShowTemperatureMode: {
            if (isRotateCW) {
                encoderValueChangeTimer = 0;
                encoderValueChanges++;
            }
            else if (isRotateCCW) {
                encoderValueChangeTimer = 0;
                encoderValueChanges--;
            }
            if (encoderValueChangeTimer >= EncoderValueChangeTimeout) {
                encoderValueChanges = 0;
            }
            // выполнить действие для вращения по часовой стрелке (включение налива)
            if (encoderValueChanges >= EncoderValueChangesThreshold) {
                encoderValueChanges = 0;
                // если датчик верхнего уровня без воды
                if (HighLevelSensorPin != LevelSensorActive) {
                    // включить налив
                    WaterRelayPin = RelayOn;
                }
            }
            // выполнить действие для вращения против часовой стрелке (выключение налива)
            else if (encoderValueChanges <= -EncoderValueChangesThreshold) {
                encoderValueChanges = 0;
                // если датчик нижнего уровня в воде
                if (LowLevelSensorPin == LevelSensorActive) {
                    // выключить налив
                    WaterRelayPin = RelayOff;
                }
            }
            break;
        }
        case SetTemperatureMode: {
            if (isRotateCW) {
                if (desiredTemperature < MaxDesiredTemperature) {
                    desiredTemperature++;
                }
                inactivityShowTemperatureModeTimer = 0;
            }
            if (isRotateCCW) {
                if (desiredTemperature > MinDesiredTemperature) {
                    desiredTemperature--;
                }
                inactivityShowTemperatureModeTimer = 0;
            }
            break;
        }
    }
}

void ScanEncoderButtonAction() {
    EncoderButtonPinDirection = InputPinDirection;
    if (EncoderButtonPin == EncoderButtonPressed) {
        if (encoderButtonTimer == EncoderButtonPressedTime) {
            encoderButtonTimer = 0;
            if (!wasEncoderButtonPressed) {
                switch (workMode) {
                    case ShowTemperatureMode: {
                        setTemperatureModeBlinkTimer = 0;
                        isSetTemperatureModeShow = 0;
                        workMode = SetTemperatureMode;
                        break;
                    }
                    case SetTemperatureMode: {
                        workMode = ShowTemperatureMode;
                        break;
                    }
                }
            }
            inactivityShowTemperatureModeTimer = 0;
            wasEncoderButtonPressed = 1;
        }
    }
    else {
        encoderButtonTimer = 0;
        wasEncoderButtonPressed = 0;
    }
}

void ProcessWaterRelayAction() {
    // если датчик нижнего уровня в воде
    if (LowLevelSensorPin == LevelSensorActive) {
        // если датчик верхнего уровня в воде
        if (HighLevelSensorPin == LevelSensorActive) {
            // выключить налив
            WaterRelayPin = RelayOff;
        }
    }
    // если датчик нижнего уровня без воды
    else {
        // если датчик верхнего уровня в воде
        if (HighLevelSensorPin == LevelSensorActive) {
            // выключить налив
            WaterRelayPin = RelayOff;
        }
        // если датчик верхнего уровня без воды
        else {
            // включить налив
            WaterRelayPin = RelayOn;
        }
    }
}

void ProcessHeaterRelayAction() {
    // если датчик нижнего уровня в воде
    if (LowLevelSensorPin == LevelSensorActive) {
        // если корректное значение температуры уже было получено
        if (DS18B20TemperatureValueIsCorrect) {
            if (DS18B20TemperatureValue.integerPart <= (desiredTemperature - TemperatureHysteresis)) {
                // если не идет налив
                if (WaterRelayPin == RelayOff) {
                    // включить нагрев
                    HeaterRelayPin = RelayOn;
                }
            }
            if (DS18B20TemperatureValue.integerPart >= (desiredTemperature + TemperatureHysteresis)) {
                // выключить нагрев
                HeaterRelayPin = RelayOff;
            }
        }
        // если значение температуры еще не было получено или датчик температуры отключен
        else {
            // выключить нагрев
            HeaterRelayPin = RelayOff;
        }
    }
    // если датчик нижнего уровня без воды
    else {
        // выключить нагрев
        HeaterRelayPin = RelayOff;
    }
}

void RefreshDS18B20Action() {
}

void ConvertTemperatureAction() {
    DS18B20ConvertTemperature();
}

void GetTemperatureAction() {
    DS18B20GetTemperature();
}

void InitOptionReg() {
    /** OPTION_REG: bit 2-0
     * PS<2:0>: Prescaler Rate Select bits
     * BIT VALUE | TMR0 RATE | WDT RATE
     * 000       | 1 :   2   | 1 :   1
     * 001       | 1 :   4   | 1 :   2
     * 010       | 1 :   8   | 1 :   4
     * 011       | 1 :  16   | 1 :   8
     * 100       | 1 :  32   | 1 :  16
     * 101       | 1 :  64   | 1 :  32
     * 110       | 1 : 128   | 1 :  64
     * 111       | 1 : 256   | 1 : 128
     */
    OPTION_REGbits.PS = 0b111;
    /** OPTION_REG: bit 3
     * PSA: Prescaler Assignment bit
     * 1 = Prescaler is assigned to the WDT
     * 0 = Prescaler is assigned to the Timer0 module
     */
    OPTION_REGbits.PSA = 0;
    /** OPTION_REG: bit 4
     * T0SE: TMR0 Source Edge Select bit
     * 1 = Increment on high-to-low transition on RA2/T0CKI pin
     * 0 = Increment on low-to-high transition on RA2/T0CKI pin
     */
    OPTION_REGbits.T0SE = 0;
    /** OPTION_REG: bit 5
     * T0CS: TMR0 Clock Source Select bit
     * 1 = Transition on RA2/T0CKI pin
     * 0 = Internal instruction cycle clock (FOSC / 4)
     */
    OPTION_REGbits.T0CS = 0;
    /** OPTION_REG: bit 6
     * INTEDG: Interrupt Edge Select bit
     * 1 = Interrupt on rising edge of RA2/INT pin
     * 0 = Interrupt on falling edge of RA2/INT pin
     */
    OPTION_REGbits.INTEDG = 1;
    /** OPTION_REG: bit 7
     * RAPU (not): PORTA Pull-up Enable bit
     * 1 = PORTA pull-ups are disabled
     * 0 = PORTA pull-ups are enabled by individual port latch values
     */
    OPTION_REGbits.nRAPU = 1;
}

void InitIntConReg(void) {
    /** INTCON: bit 0
     * RAIF: PORTA Change Interrupt Flag bit
     * 1 = When at least one of the PORTA <5:0> pins changed state (must be cleared in software)
     * 0 = None of the PORTA <5:0> pins have changed state
     */
    /** INTCON: bit 1
     * INTF: RA2/INT External Interrupt Flag bit
     * 1 = The RA2/INT external interrupt occurred (must be cleared in software)
     * 0 = The RA2/INT external interrupt did not occur
     */
    /** INTCON: bit 2
     * T0IF: TMR0 Overflow Interrupt Flag bit
     * 1 = TMR0 register has overflowed (must be cleared in software)
     * 0 = TMR0 register did not overflow
     */
    /** INTCON: bit 3
     * RAIE: PORTA Change Interrupt Enable bit
     * 1 = Enables the PORTA change interrupt
     * 0 = Disables the PORTA change interrupt
     */
    INTCONbits.RAIE = 0;
    /** INTCON: bit 4
     * INTE: RA2/INT External Interrupt Enable bit
     * 1 = Enables the RA2/INT external interrupt
     * 0 = Disables the RA2/INT external interrupt
     */
    INTCONbits.INTE = 0;
    /** INTCON: bit 5
     * T0IE: TMR0 Overflow Interrupt Enable bit
     * 1 = Enables the TMR0 interrupt
     * 0 = Disables the TMR0 interrupt
     */
    INTCONbits.T0IE = 1;
    /** INTCON: bit 6
     * PEIE: Peripheral Interrupt Enable bit
     * 1 = Enables all unmasked peripheral interrupts
     * 0 = Disables all peripheral interrupts
     */
    INTCONbits.PEIE = 0;
    /** INTCON: bit 6
     * GIE: Global Interrupt Enable bit
     * 1 = Enables all unmasked interrupts
     * 0 = Disables all interrupts
     */
    INTCONbits.GIE = 0;
}

void InitOscConReg() {
    /** OSCCON: bit 0
     * SCS: System Clock Select bit
     * 1 = Internal oscillator is used for system clock
     * 0 = Clock source defined by FOSC<2:0> of the Configuration Word
     */
    OSCCONbits.SCS = 1;
    /** OSCCON: bit 1
     * LTS: LFINTOSC Stable bit (Low Frequency – 31 kHz)
     * 1 = LFINTOSC is stable
     * 0 = LFINTOSC is not stable
     */
    OSCCONbits.LTS = 0;
    /** OSCCON: bit 2
     * HTS: HFINTOSC Status bit (High Frequency – 8 MHz to 125 kHz)
     * 1 = HFINTOSC is stable
     * 0 = HFINTOSC is not stableising edge of INT pin
     */
    OSCCONbits.HTS = 1;
    /** OSCCON: bit 3
     * OSTS: Oscillator Start-up Time-out Status bit (Bit resets to '0' with Two-Speed Start-up and LP, XT or HS selected as the Oscillator mode or Fail-Safe mode is enabled.)
     * 1 = Device is running from the external clock defined by FOSC<2:0> of the Configuration Word
     * 0 = Device is running from the internal oscillator (HFINTOSC or LFINTOSC)
     */
    OSCCONbits.OSTS = 0;
    /** OSCCON: bit 6-4
     * IRCF<2:0>: Internal Oscillator Frequency Select bits
     * 111 =   8 MHz
     * 110 =   4 MHz (default)
     * 101 =   2 MHz
     * 100 =   1 MHz
     * 011 = 500 kHz
     * 010 = 250 kHz
     * 001 = 125 kHz
     * 000 =  31 kHz (LFINTOSC)
     */
    OSCCONbits.IOSCF = 0b111;
    /** OSCCON: bit 7
     * Unimplemented: Read as '0'
     */
}

void InitAnSelReg() {
    /** ANSEL: bit 7-0
     * ANS<7:0>: Analog Select bits. Analog select between analog or digital function on pins AN<7:0>, respectively
     * 1 = Analog input. Pin is assigned as analog input
     * 0 = Digital I/O. Pin is assigned to port or special function
     */
    ANSEL = 0b00000000;
}

void InitCmCon0Reg() {
    /** CMCON0: bit 2-0
     * CM<2:0>: Comparator Mode bits
     * 000 = Comparators off. CxIN pins are configured as analog
     * 001 = Three inputs multiplexed to two comparators
     * 010 = Four inputs multiplexed to two comparators
     * 011 = Two common reference comparators
     * 100 = Two independent comparators
     * 101 = One independent comparator
     * 110 = Two common reference comparators with outputs
     * 111 = Comparators off. CxIN pins are configured as digital I/O
     */
    CMCON0bits.CM = 0b111;
    /** CMCON0: bit 3
     * CIS: Comparator Input Switch bit
     * When CM<2:0> = 010:
     * 1 = C1IN+ connects to C1 VIN-
     *     C2IN+ connects to C2 VIN-
     * 0 = C1IN- connects to C1 VIN-
     *     C2IN- connects to C2 VIN-
     * When CM<2:0> = 001:
     * 1 = C1IN+ connects to C1 VIN-
     * 0 = C1IN- connects to C1 VIN-
     */
    /** CMCON0: bit 4
     * C1INV: Comparator 1 Output Inversion bit
     * 1 = C1 Output inverted
     * 0 = C1 Output not inverted
     */
    /** CMCON0: bit 5
     * C2INV: Comparator 2 Output Inversion bit
     * 1 = C2 output inverted
     * 0 = C2 output not inverted
     */
    /** CMCON0: bit 6
     * C1OUT: Comparator 1 Output bit
     * When C1INV = 0:
     * 1 = C1 VIN+ > C1 VIN-
     * 0 = C1 VIN+ < C1 VIN-
     * When C1INV = 1:
     * 1 = C1 VIN+ < C1 VIN-
     * 0 = C1 VIN+ > C1 VIN-
     */
    /** CMCON0: bit 7
     * C2OUT: Comparator 2 Output bit
     * When C2INV = 0:
     * 1 = C2 VIN+ > C2 VIN-
     * 0 = C2 VIN+ < C2 VIN-
     * When C2INV = 1:
     * 1 = C2 VIN+ < C2 VIN-
     * 0 = C2 VIN+ > C2 VIN-
     */
}

void InitADCon0Reg() {
    /** ADCON0: bit0
     * ADON: ADC Enable bit
     * 1 = ADC is enabled
     * 0 = ADC is disabled and consumes no operating current
     */
    ADCON0bits.ADON = 0;
    /** ADCON0: bit 1
     * GO/DONE (not): A/D Conversion Status bit
     * 1 = A/D Conversion cycle in progress. Setting this bit starts an A/D Conversion cycle. This bit is automatically cleared by hardware when the A/D Conversion has completed.
     * 0 = A/D Conversion completed/not in progress
     */
    /** ADCON0: bit 4-2
     * CHS<2:0>: Analog Channel Select bits
     * 000 = AN0
     * 001 = AN1
     * 010 = AN2
     * 011 = AN3
     * 100 = AN4
     * 101 = AN5
     * 110 = AN6
     * 111 = AN7
     */
    /** ADCON0: bit 5
     * Unimplemented: Read as '0'
     */
    /** ADCON0: bit 6
     * VCFG: Voltage Reference bit
     * 1 = VREF pin
     * 0 = VDD
     */
    /** ADCON0: bit 7
     * ADFM: A/D Conversion Result Format Select bit
     * 1 = Right justified
     * 0 = Left justified
     */
}

void InitRegisters() {
    InitOptionReg();
    InitIntConReg();
    InitOscConReg();
    InitAnSelReg();
    InitCmCon0Reg();
    InitADCon0Reg();
}

void InitPins() {
    Indicator0Pin = IndicatorOff;
    Indicator1Pin = IndicatorOff;
    Indicator2Pin = IndicatorOff;
    Indicator0PinDirection = OutputPinDirection;
    Indicator1PinDirection = OutputPinDirection;
    Indicator2PinDirection = OutputPinDirection;
    EncoderLeftPinDirection   = InputPinDirection;
    EncoderRightPinDirection  = InputPinDirection;
    EncoderButtonPinDirection = InputPinDirection;
    LowLevelSensorPinDirection  = InputPinDirection;
    HighLevelSensorPinDirection = InputPinDirection;
    WaterRelayPin  = RelayOff;
    HeaterRelayPin = RelayOff;
    WaterRelayPinDirection  = OutputPinDirection;
    HeaterRelayPinDirection = OutputPinDirection;
}

void main() {
    TRISA = 0b11111111;
    TRISC = 0b11111111;
    InitRegisters();
    InitPins();
    SR74HC595Initialize();
    DS18B20SetResolution(DS18B20Resolution);
    InitRTOS();
    RunRTOS();
    AddTask(DrawIndicatorsTask, 0);
    AddTask(FillIndicatorsTask, 0);
    AddTask(ScanEncoderTask, 0);
    AddTask(ScanEncoderButtonTask, 0);
    AddTask(ProcessWaterRelayTask, 0);
    AddTask(ProcessHeaterRelayTask, 0);
    AddTask(RefreshDS18B20Task, 0);
    while (1) {
        di();
        TaskManager();
        ei();
    }
}
