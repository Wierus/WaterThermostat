#ifndef CONFIG_H
#define	CONFIG_H

#include <xc.h>

/** Device: PIC16F688
 * ("16f688.html" and "41203E.pdf")
 */

/** CONFIG: bit 2-0
 * FOSC<2:0>: Oscillator Selection bits
 * 111 = EXTRCCLK  = EXTRC oscillator: External RC on RA5/OSC1/CLKIN, CLKOUT function on RA4/OSC2/CLKOUT pin
 * 110 = EXTRCIO   = EXTRCIO oscillator: External RC on RA5/OSC1/CLKIN, I/O function on RA4/OSC2/CLKOUT pin
 * 101 = INTOSCCLK = INTOSC oscillator: CLKOUT function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN
 * 100 = INTOSCIO  = INTOSCIO oscillator: I/O function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN
 * 011 = EC        = EC: I/O function on RA4/OSC2/CLKOUT pin, CLKIN on RA5/OSC1/CLKIN
 * 010 = HS        = HS oscillator: High-speed crystal/resonator on RA4/OSC2/CLKOUT and RA5/OSC1/CLKIN
 * 001 = XT        = XT oscillator: Crystal/resonator on RA4/OSC2/CLKOUT and RA5/OSC1/CLKIN
 * 000 = LP        = LP oscillator: Low-power crystal on RA4/OSC2/CLKOUT and RA5/OSC1/CLKIN
 */
#pragma config FOSC = INTOSCIO

/** CONFIG: bit 3
 * WDTE: Watchdog Timer Enable bit
 * 1 = ON  = WDT enabled
 * 0 = OFF = WDT disabled
 */
#pragma config WDTE = OFF

/** CONFIG: bit 4
 * PWRTE (not): Power-up Timer Enable bit
 * 1 = OFF = PWRT disabled
 * 0 = ON  = PWRT enabled
 */
#pragma config PWRTE = OFF

/** CONFIG: bit 5
 * MCLRE: MCLR Pin Function Select bit
 * 1 = ON  = MCLR pin function is MCLR
 * 0 = OFF = MCLR pin function is digital input, MCLR internally tied to VDD
 */
#pragma config MCLRE = OFF

/** CONFIG: bit 6
 * CP (not): Code Protection bit
 * 1 = OFF = Program memory code protection is disabled
 * 0 = ON  = Program memory code protection is enabled
 */
#pragma config CP = OFF

/** CONFIG: bit 7
 * CPD (not): Data Code Protection bit
 * 1 = OFF = Data memory code protection is disabled
 * 0 = ON  = Data memory code protection is enabled
 */
#pragma config CPD = OFF

/** CONFIG: bit 9-8
 * BOREN<1:0>: Brown-out Reset Selection bits
 * 11 = ON     = BOR enabled
 * 10 = NSLEEP = BOR enabled during operation and disabled in Sleep
 * 01 = SBODEN = BOR controlled by SBOREN bit of the PCON register
 * 00 = OFF    = BOR disabled
 */
#pragma config BOREN = OFF

/** CONFIG: bit 10
 * IESO: Internal External Switchover bit
 * 1 = ON  = Internal External Switchover mode is enabled
 * 0 = OFF = Internal External Switchover mode is disabled
 */
#pragma config IESO = OFF

/** CONFIG: bit 11
 * FCMEN: Fail-Safe Clock Monitor Enabled bit
 * 1 = ON  = Fail-Safe Clock Monitor is enabled
 * 0 = OFF = Fail-Safe Clock Monitor is disabled
 */
#pragma config FCMEN = OFF

/** CONFIG: bit 15-12
 * Reserved: Reserved bits. Do Not Use
 */

/** Тактовая частота контроллера (Гц).
 */
#define F_OSC 8000000.0

/** Коэффициент предделителя для TMR0.
 */
#define PRESCALER_RATE (1.0 / 256.0)

/** Длина очереди задач для диспетчера.
 */
#define TaskManagerQueueSize 8

/** Тип таймера задач для диспетчера.
 */
#define TaskManagerTimerType unsigned char
        
#endif /* CONFIG_H */
