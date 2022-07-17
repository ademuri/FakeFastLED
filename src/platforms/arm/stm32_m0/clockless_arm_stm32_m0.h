#ifndef __INC_CLOCKLESS_ARM_STM32_M0_H
#define __INC_CLOCKLESS_ARM_STM32_M0_H

#include "../common/m0clockless.h"

FASTLED_NAMESPACE_BEGIN
// Definition for a single channel clockless controller for the stm32 M0+ family
// of chips. This family does not have the DWT counter, so uses the common
// m0clockless assembly code. See clockless.h for detailed info on how the
// template parameters are used.

#define FASTLED_HAS_CLOCKLESS 1

#define ADJ 20

template <int DATA_PIN, int T1, int T2, int T3, EOrder RGB_ORDER = RGB,
          int XTRA0 = 0, bool FLIP = false, int WAIT_TIME = 50>
class ClocklessController : public CPixelLEDController<RGB_ORDER> {
  typedef typename FastPin<DATA_PIN>::port_ptr_t data_ptr_t;
  typedef typename FastPin<DATA_PIN>::port_t data_t;

  data_t mPinMask;
  data_ptr_t mPort;
  CMinWait<WAIT_TIME> mWait;

 public:
  virtual void init() {
    FastPin<DATA_PIN>::setOutput();
    mPinMask = FastPin<DATA_PIN>::mask();
    mPort = FastPin<DATA_PIN>::port();
  }

  virtual uint16_t getMaxRefreshRate() const { return 400; }

 protected:
  virtual void showPixels(PixelController<RGB_ORDER>& pixels) {
    mWait.wait();
    cli();
    if (!showRGBInternal(pixels)) {
      sei();
      delayMicroseconds(WAIT_TIME);
      cli();
      showRGBInternal(pixels);
    }
    sei();
    mWait.mark();
  }

  // This method is made static to force making register Y available to use for
  // data on AVR - if the method is non-static, then gcc will use register Y for
  // the this pointer.
  static uint32_t showRGBInternal(PixelController<RGB_ORDER> pixels) {
    // Get access to the clock
    // CoreDebug->DEMCR  |= CoreDebug_DEMCR_TRCENA_Msk;
    // DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    // DWT->CYCCNT = 0;
    struct M0ClocklessData data;
    data.d[0] = pixels.d[0];
    data.d[1] = pixels.d[1];
    data.d[2] = pixels.d[2];
    data.s[0] = pixels.mScale[0];
    data.s[1] = pixels.mScale[1];
    data.s[2] = pixels.mScale[2];
    data.e[0] = pixels.e[0];
    data.e[1] = pixels.e[1];
    data.e[2] = pixels.e[2];
    data.adj = pixels.mAdvance;

    typename FastPin<DATA_PIN>::port_ptr_t portBase = FastPin<DATA_PIN>::port();
    // Offsets: ODR is at 0x14 (base), BSRR is at 0x18 (0x4 offset), BRR is at
    // 0x28 (0x14 offset)
    return showLedData<0x4, 0x14, T1, T2, T3, RGB_ORDER, WAIT_TIME>(
        portBase, FastPin<DATA_PIN>::mask(), pixels.mData, pixels.mLen, &data);
  }
};

FASTLED_NAMESPACE_END

#endif  // __INC_CLOCKLESS_ARM_STM32_M0_H
