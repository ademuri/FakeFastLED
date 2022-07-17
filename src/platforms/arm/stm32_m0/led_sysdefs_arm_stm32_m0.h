#ifndef __INC_LED_SYSDEFS_ARM_STM32_M0_H
#define __INC_LED_SYSDEFS_ARM_STM32_M0_H

#if defined(STM32G0xx)
// stm32duino

#define cli() noInterrupts()
#define sei() interrupts()

#define FASTLED_ARM_M0_PLUS

#else  // STM32G0xx
#error "Platform not supported"
#endif

#define FASTLED_ARM

#ifndef INTERRUPT_THRESHOLD
#define INTERRUPT_THRESHOLD 1
#endif

// Default to allowing interrupts
#ifndef FASTLED_ALLOW_INTERRUPTS
#define FASTLED_ALLOW_INTERRUPTS 1
#endif

#if FASTLED_ALLOW_INTERRUPTS == 1
#define FASTLED_ACCURATE_CLOCK
#endif

// pgmspace definitions
#define PROGMEM

// Default to NOT using PROGMEM here
#ifndef FASTLED_USE_PROGMEM
#define FASTLED_USE_PROGMEM 0
#endif

// data type defs
typedef volatile uint8_t
    RoReg; /**< Read only 8-bit register (volatile const unsigned int) */
typedef volatile uint8_t
    RwReg; /**< Read-Write 8-bit register (volatile unsigned int) */

#define FASTLED_NO_PINMAP

#if defined(STM32G0xx)
// F_CPU is already defined on stm32duino, but it's not constant.
#undef F_CPU
#define F_CPU 64000000
#define VARIANT_MCK F_CPU
#endif
#endif  // __INC_LED_SYSDEFS_ARM_SAM_H
