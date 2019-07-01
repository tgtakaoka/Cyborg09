#ifndef __pins_map_h__
#define __pins_map_h__

#include <stdint.h>

#if defined(ARDUINO_AVR_ATmega32) || defined(ARDUINO_AVR_ATmega1284)

#define HALT_BV    PB0
#define HALT_PIN   PINB
#define HALT_PORT  PORTB
#define HALT_DDR   DDRB

#define BS_BV      PC7
#define BS_PIN     PINC
#define BS_PORT    PORTC
#define BS_DDR     DDRC

#define BA_BV      PC5
#define BA_PIN     PINC
#define BA_PORT    PORTC
#define BA_DDR     DDRC

#define LIC_BV     PD5
#define LIC_PIN    PIND
#define LIC_PORT   PORTD
#define LIC_DDR    DDRD

#define AVMA_BV    PC4
#define AVMA_PIN   PINC
#define AVMA_PORT  PORTC
#define AVMA_DDR   DDRC

#define RESET_BV   PB2
#define RESET_PIN  PINB
#define RESET_PORT PORTB
#define RESET_DDR  DDRB

#define STEP_BV    PD7
#define STEP_PIN   PIND
#define STEP_PORT  PORTD
#define STEP_DDR   DDRD

#define ACK_BV     PD6
#define ACK_PIN    PIND
#define ACK_PORT   PORTD
#define ACK_DDR    DDRD

#define RD_WR_BV   PB3
#define RD_WR_PIN  PINB
#define RD_WR_PORT PORTB
#define RD_WR_DDR  DDRB

#define RAM_E_BV   PC6
#define RAM_E_PIN  PINC
#define RAM_E_PORT PORTC
#define RAM_E_DDR  DDRC

#define ADR0_BV    PC3
#define ADR0_PIN   PINC
#define ADR0_PORT  PORTC
#define ADR0_DDR   DDRC

#define ADR1_BV    PC2
#define ADR0_PIN   PINC
#define ADR0_PORT  PORTC
#define ADR0_DDR   DDRC

#define DB0_BV     PA0
#define DB0_PIN    PINA
#define DB0_PORT   PORTA
#define DB0_DDR    DDRA

#define DB1_BV     PA1
#define DB1_PIN    PINA
#define DB1_PORT   PORTA
#define DB1_DDR    DDRA

#define DB2_BV     PA2
#define DB2_PIN    PINA
#define DB2_PORT   PORTA
#define DB2_DDR    DDRA

#define DB3_BV     PA3
#define DB3_PIN    PINA
#define DB3_PORT   PORTA
#define DB3_DDR    DDRA

#define DB4_BV     PA4
#define DB4_PIN    PINA
#define DB4_PORT   PORTA
#define DB4_DDR    DDRA

#define DB5_BV     PA5
#define DB5_PIN    PINA
#define DB5_PORT   PORTA
#define DB5_DDR    DDRA

#define DB6_BV     PA6
#define DB6_PIN    PINA
#define DB6_PORT   PORTA
#define DB6_DDR    DDRA

#define DB7_BV     PA7
#define DB7_PIN    PINA
#define DB7_PORT   PORTA
#define DB7_DDR    DDRA

#endif

/* Adafruit ItsyBitsy 32u4 5V */
#if defined(ARDUINO_AVR_ITSYBITSY32U4_5V)

#define HALT_BV    PF7
#define HALT_PIN   PINF
#define HALT_PORT  PORTF
#define HALT_DDR   DDRF

#define BS_BV      PF6
#define BS_PIN     PINF
#define BS_PORT    PORTF
#define BS_DDR     DDRF

#define BA_BV      PF5
#define BA_PIN     PINF
#define BA_PORT    PORTF
#define BA_DDR     DDRF

#define LIC_BV     PF4
#define LIC_PIN    PINF
#define LIC_PORT   PORTF
#define LIC_DDR    DDRF

#define AVMA_BV    PF1
#define AVMA_PIN   PINF
#define AVMA_PORT  PORTF
#define AVMA_DDR   DDRF

#define RESET_BV   PB1
#define RESET_PIN  PINB
#define RESET_PORT PORTB
#define RESET_DDR  DDRB

#define STEP_BV    PB2
#define STEP_PIN   PINB
#define STEP_PORT  PORTB
#define STEP_DDR   DDRB

#define ACK_BV     PB3
#define ACK_PIN    PINB
#define ACK_PORT   PORTB
#define ACK_DDR    DDRB

#define RD_WR_BV   PB4
#define RD_WR_PIN  PINB
#define RD_WR_PORT PORTB
#define RD_WR_DDR  DDRB

#define RAM_E_BV   PB7
#define RAM_E_PIN  PINB
#define RAM_E_PORT PORTB
#define RAM_E_DDR  DDRB

#define DB0_BV     PB6
#define DB0_PIN    PINB
#define DB0_PORT   PORTB
#define DB0_DDR    DDRB

#define DB1_BV     PB5
#define DB1_PIN    PINB
#define DB1_PORT   PORTB
#define DB1_DDR    DDRB

#define DB2_BV     PE6
#define DB2_PIN    PINE
#define DB2_PORT   PORTE
#define DB2_DDR    DDRE

#define DB3_BV     PC6
#define DB3_PIN    PINC
#define DB3_PORT   PORTC
#define DB3_DDR    DDRC

#define DB4_BV     PD0
#define DB4_PIN    PIND
#define DB4_PORT   PORTD
#define DB4_DDR    DDRD

#define DB5_BV     PD1
#define DB5_PIN    PIND
#define DB5_PORT   PORTD
#define DB5_DDR    DDRD

#define DB6_BV     PD3
#define DB6_PIN    PIND
#define DB6_PORT   PORTD
#define DB6_DDR    DDRD

#define DB7_BV     PD2
#define DB7_PIN    PIND
#define DB7_PORT   PORTD
#define DB7_DDR    DDRD

#endif

#endif