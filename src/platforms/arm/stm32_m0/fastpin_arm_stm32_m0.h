#ifndef __FASTPIN_ARM_STM32_M0_H
#define __FASTPIN_ARM_STM32_M0_H

FASTLED_NAMESPACE_BEGIN

#if defined(FASTLED_FORCE_SOFTWARE_PINS)
#warning "Software pin support forced, pin access will be slightly slower."
#define NO_HARDWARE_PIN_SUPPORT
#undef HAS_HARDWARE_PIN_SUPPORT

#else

/// Template definition for STM32 style ARM pins, providing direct access to the
/// various GPIO registers.  Note that this uses the full port GPIO registers.
/// In theory, in some way, bit-band register access -should- be faster, however
/// I have found that something about the way gcc does register allocation
/// results in the bit-band code being slower.  It will need more fine tuning.
/// The registers are data output, set output, clear output, toggle output,
/// input, and direction

template <uint8_t PIN, uint8_t _BIT, uint32_t _MASK, typename _GPIO>
class _ARMPIN {
 public:
  typedef volatile uint32_t* port_ptr_t;
  typedef uint32_t port_t;

#if 0
    inline static void setOutput() {
        if(_BIT<8) {
            _CRL::r() = (_CRL::r() & (0xF << (_BIT*4)) | (0x1 << (_BIT*4));
        } else {
            _CRH::r() = (_CRH::r() & (0xF << ((_BIT-8)*4))) | (0x1 << ((_BIT-8)*4));
        }
    }
    inline static void setInput() { /* TODO */ } // TODO: preform MUX config { _PDDR::r() &= ~_MASK; }
#endif

  inline static void setOutput() {
    pinMode(PIN, OUTPUT);
  }  // TODO: perform MUX config { _PDDR::r() |= _MASK; }
  inline static void setInput() {
    pinMode(PIN, INPUT);
  }  // TODO: preform MUX config { _PDDR::r() &= ~_MASK; }

  inline static void hi() __attribute__((always_inline)) {
    _GPIO::r()->BSRR = _MASK;
  }
  inline static void lo() __attribute__((always_inline)) {
    _GPIO::r()->BRR = _MASK;
  }
  inline static void set(register port_t val) __attribute__((always_inline)) {
    _GPIO::r()->ODR = val;
  }

  inline static void strobe() __attribute__((always_inline)) {
    toggle();
    toggle();
  }

  inline static void toggle() __attribute__((always_inline)) {
    if (_GPIO::r()->ODR & _MASK) {
      lo();
    } else {
      hi();
    }
  }

  inline static void hi(register port_ptr_t port)
      __attribute__((always_inline)) {
    hi();
  }
  inline static void lo(register port_ptr_t port)
      __attribute__((always_inline)) {
    lo();
  }
  inline static void fastset(register port_ptr_t port, register port_t val)
      __attribute__((always_inline)) {
    *port = val;
  }

  inline static port_t hival() __attribute__((always_inline)) {
    return _GPIO::r()->ODR | _MASK;
  }
  inline static port_t loval() __attribute__((always_inline)) {
    return _GPIO::r()->ODR & ~_MASK;
  }
  inline static port_ptr_t port() __attribute__((always_inline)) {
    return &_GPIO::r()->ODR;
  }

  inline static port_ptr_t sport() __attribute__((always_inline)) {
    return &_GPIO::r()->BSRR;
  }
  inline static port_ptr_t cport() __attribute__((always_inline)) {
    return &_GPIO::r()->BRR;
  }

  inline static port_t mask() __attribute__((always_inline)) { return _MASK; }
};

#define _R(T) struct __gen_struct_##T
#define _FL_DEFPIN(PIN, BIT, L) \
  template <>                   \
  class FastPin<PIN> : public _ARMPIN<PIN, BIT, 1 << BIT, _R(GPIO##L)> {};

#if defined(STM32G0xx)
// stm32duino
#define _RD32(T)                                                              \
  struct __gen_struct_##T {                                                   \
    static __attribute__((always_inline)) inline volatile GPIO_TypeDef* r() { \
      return T;                                                               \
    }                                                                         \
  };
#define _FL_IO(L, C) _RD32(GPIO##L);
#else  // STM32G0xx
#error "Platform not supported"
#endif

#ifdef GPIOA
_FL_IO(A, 0);
#endif
#ifdef GPIOB
_FL_IO(B, 1);
#endif
#ifdef GPIOC
_FL_IO(C, 2);
#endif
#ifdef GPIOD
_FL_IO(D, 3);
#endif
#ifdef GPIOE
_FL_IO(E, 4);
#endif
#ifdef GPIOF
_FL_IO(F, 5);
#endif
#ifdef GPIOG
_FL_IO(G, 6);
#endif

// Actual pin definitions
#if defined(ARDUINO_GENERIC_G030C8TX) || defined(ARDUINO_GENERIC_G070CBTX) || defined(ARDUINO_GENERIC_G070KBTX)
#define MAX_PIN 44

// PA0-PA15
_FL_DEFPIN(0, 0, A);
_FL_DEFPIN(1, 1, A);
_FL_DEFPIN(2, 2, A);
_FL_DEFPIN(3, 3, A);
_FL_DEFPIN(4, 4, A);
_FL_DEFPIN(5, 5, A);
_FL_DEFPIN(6, 6, A);
_FL_DEFPIN(7, 7, A);
_FL_DEFPIN(8, 8, A);
_FL_DEFPIN(9, 9, A);
_FL_DEFPIN(10, 10, A);
_FL_DEFPIN(11, 11, A);
_FL_DEFPIN(12, 12, A);
_FL_DEFPIN(13, 13, A);
_FL_DEFPIN(14, 14, A);
_FL_DEFPIN(15, 15, A);

// PB0-PB15
_FL_DEFPIN(16, 0, B);
_FL_DEFPIN(17, 1, B);
_FL_DEFPIN(18, 2, B);
_FL_DEFPIN(19, 3, B);
_FL_DEFPIN(20, 4, B);
_FL_DEFPIN(21, 5, B);
_FL_DEFPIN(22, 6, B);
_FL_DEFPIN(23, 7, B);
_FL_DEFPIN(24, 8, B);
_FL_DEFPIN(25, 9, B);
_FL_DEFPIN(26, 10, B);
_FL_DEFPIN(27, 11, B);
_FL_DEFPIN(28, 12, B);
_FL_DEFPIN(29, 13, B);
_FL_DEFPIN(30, 14, B);
_FL_DEFPIN(31, 15, B);

// PC6-PC7, PC13-PC15
_FL_DEFPIN(32, 6, C);
_FL_DEFPIN(33, 7, C);
_FL_DEFPIN(34, 13, C);
_FL_DEFPIN(35, 14, C);
_FL_DEFPIN(36, 15, C);

// PD0-PD3
_FL_DEFPIN(37, 0, D);
_FL_DEFPIN(38, 1, D);
_FL_DEFPIN(39, 2, D);
_FL_DEFPIN(40, 3, D);

// PF0-PF1
_FL_DEFPIN(41, 0, F);
_FL_DEFPIN(42, 1, F);

// PA9-PA10 (alt)
_FL_DEFPIN(43, 9, A);
_FL_DEFPIN(44, 10, A);

#define HAS_HARDWARE_PIN_SUPPORT

#endif  // ARDUINO_GENERIC_G030C8TX || ARDUINO_GENERIC_G070CBTX || ARDUINO_GENERIC_G070KBTX

#endif  // FASTLED_FORCE_SOFTWARE_PINS

FASTLED_NAMESPACE_END

#endif  // __FASTPIN_ARM_STM32_M0_H
