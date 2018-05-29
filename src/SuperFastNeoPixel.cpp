#include "SuperFastNeoPixel.h"

bool SuperFastNeoPixel::Begin() {
    uint32_t Divisor, PortConfig, HW_Trigger;
    KINETISK_UART_t *UART;

    switch (Pin) {
#if defined(KINETISK) // Teensy 3.x
        case 1: // Serial1
        case 5:
#if defined(__MK64FX512__) || defined(__MK66FX1M0__)
            case 26:
#endif
            UART = &KINETISK_UART0;
            Divisor = BAUD2DIV(4000000);
            PortConfig = PORT_PCR_DSE | PORT_PCR_SRE | PORT_PCR_MUX(3);
            HW_Trigger = DMAMUX_SOURCE_UART0_TX;
            SIM_SCGC4 |= SIM_SCGC4_UART0;
            break;

        case 10: // Serial2
#if defined(__MK20DX128__) || defined(__MK20DX256__)
        case 31:
#elif defined(__MK64FX512__) || defined(__MK66FX1M0__)
            case 58:
#endif
            UART = &KINETISK_UART1;
            Divisor = BAUD2DIV2(4000000);
            PortConfig = PORT_PCR_DSE | PORT_PCR_SRE | PORT_PCR_MUX(3);
            HW_Trigger = DMAMUX_SOURCE_UART1_TX;
            SIM_SCGC4 |= SIM_SCGC4_UART1;
            break;

        case 8: // Serial3
        case 20:
            UART = &KINETISK_UART2;
            Divisor = BAUD2DIV3(4000000);
            PortConfig = PORT_PCR_DSE | PORT_PCR_SRE | PORT_PCR_MUX(3);
            HW_Trigger = DMAMUX_SOURCE_UART2_TX;
            SIM_SCGC4 |= SIM_SCGC4_UART2;
            break;

#if defined(__MK64FX512__) || defined(__MK66FX1M0__)
        case 32: // Serial4
        case 62:
          UART = &KINETISK_UART3;
          Divisor = BAUD2DIV3(4000000);
          PortConfig = PORT_PCR_DSE | PORT_PCR_SRE | PORT_PCR_MUX(3);
          HW_Trigger = DMAMUX_SOURCE_UART3_TX;
          SIM_SCGC4 |= SIM_SCGC4_UART3;
          break;

        case 33: // Serial5
          UART = &KINETISK_UART4;
          Divisor = BAUD2DIV3(4000000);
          PortConfig = PORT_PCR_DSE | PORT_PCR_SRE | PORT_PCR_MUX(3);
          HW_Trigger = DMAMUX_SOURCE_UART4_RXTX;
          SIM_SCGC1 |= SIM_SCGC1_UART4;
          break;
#endif
#if defined(__MK64FX512__)
        case 48: // Serial6
          UART = &KINETISK_UART5;
          Divisor = BAUD2DIV3(4000000);
          PortConfig = PORT_PCR_DSE | PORT_PCR_SRE | PORT_PCR_MUX(3);
          HW_Trigger = DMAMUX_SOURCE_UART5_RXTX;
          SIM_SCGC1 |= SIM_SCGC1_UART5;
          break;
#endif

#elif defined(KINETISL)    // Teensy LC
        case 1: // Serial1
        case 5:
          UART = &KINETISK_UART0;
          Divisor = 1;
          PortConfig = PORT_PCR_DSE | PORT_PCR_SRE | PORT_PCR_MUX(3);
          HW_Trigger = DMAMUX_SOURCE_UART0_TX;
          SIM_SCGC4 |= SIM_SCGC4_UART0;
          break;
        case 4:
          UART = &KINETISK_UART0;
          Divisor = 1;
          PortConfig = PORT_PCR_DSE | PORT_PCR_SRE | PORT_PCR_MUX(2);
          HW_Trigger = DMAMUX_SOURCE_UART0_TX;
          SIM_SCGC4 |= SIM_SCGC4_UART0;
          break;
        case 24:
          UART = &KINETISK_UART0;
          Divisor = 1;
          PortConfig = PORT_PCR_DSE | PORT_PCR_SRE | PORT_PCR_MUX(4);
          HW_Trigger = DMAMUX_SOURCE_UART0_TX;
          SIM_SCGC4 |= SIM_SCGC4_UART0;
          break;
#endif
        default:
            return false; // Pin not supported
    }
    if (!DMA) {
        DMA = new DMAChannel;
        if (!DMA) return false; // unable to allocate DMA channel
    }
#if defined(KINETISK)
    if (Divisor < 32) Divisor = 32;
    UART->BDH = (Divisor >> 13) & 0x1F;
    UART->BDL = (Divisor >> 5) & 0xFF;
    UART->C4 = Divisor & 0x1F;
#elif defined(KINETISL)
    UART->BDH = (Divisor >> 8) & 0x1F;
    UART->BDL = Divisor & 0xFF;
    UART->C4 = 11;
#endif
    UART->C1 = 0;
    UART->C2 = UART_C2_TE | UART_C2_TIE;
    UART->C3 = UART_C3_TXINV;
    UART->C5 = UART_C5_TDMAS;
#if defined(KINETISK)
    UART->PFIFO = 0; // TODO: is this ok for Serial3-6?
#endif
    *(portConfigRegister(Pin)) = PortConfig;
    DMA->destination(UART->D);
    DMA->triggerAtHardwareEvent(HW_Trigger);
    memset(DrawBuffer, 0, NumLed * 3);
    return true;
}

bool SuperFastNeoPixel::IsBusy() {
#if defined(KINETISK)
    return ((DMA_ERQ & (1 << DMA->channel)));
#elif defined(KINETISL)
    return ((DMA->CFG->DCR & DMA_DCR_ERQ)) ;
#endif
}

int SuperFastNeoPixel::GetUpdateRate() {
    static u_int32_t LastCall;
    u_int32_t Now = millis();
    u_int32_t Delta = Now - LastCall;
    int Rate = (1000 * FrameCount) / Delta;
    FrameCount = 0;
    LastCall = Now;
    return Rate;
}

void SuperFastNeoPixel::ShowBlocking(){
    // wait if prior DMA still in progress
    while (IsBusy()) { yield(); };
    ShowNonBlocking();
}

void SuperFastNeoPixel::ShowNonBlocking() {
    FrameCount++;
    // copy drawing buffer to frame buffer
    const uint8_t *CurrentPtr = DrawBuffer;
    const uint8_t *EndPtr = CurrentPtr + (NumLed * 3);
    uint8_t *CurFrameBuffer = FrameBuffer;
    while (CurrentPtr < EndPtr) {
        uint8_t b = (float)(*CurrentPtr++)*_Brightness;
        uint8_t g = (float)(*CurrentPtr++)*_Brightness;
        uint8_t r = (float)(*CurrentPtr++)*_Brightness;
        uint32_t n = 0;
        switch (Config) {
            case PIXELORDER::RGB:
                n = (r << 16) | (g << 8) | b;
                break;
            case PIXELORDER::RBG:
                n = (r << 16) | (b << 8) | g;
                break;
            case PIXELORDER::GRB:
                n = (g << 16) | (r << 8) | b;
                break;
            case PIXELORDER::GBR:
                n = (g << 16) | (b << 8) | r;
                break;
            case PIXELORDER::BRG:
                n = (b << 16) | (r << 8) | g;
                break;
            case PIXELORDER::BGR:
                n = (b << 16) | (g << 8) | r;
                break;
        }
        const uint8_t *Stop = CurFrameBuffer + 12;
        do {
            uint8_t x = 0x08;
            if (!(n & 0x00800000)) x |= 0x07;
            if (!(n & 0x00400000)) x |= 0xE0;
            n <<= 2;
            *CurFrameBuffer++ = x;
        } while (CurFrameBuffer < Stop);
    }
    // wait 300us WS28xx reset time
    uint32_t MinimumElapsed = (NumLed * 30) + 300;
    if (MinimumElapsed < 2500) MinimumElapsed = 2500;
    uint32_t Now;
    while (1) {
        Now = micros();
        if ((Now - Prior_Micros) > MinimumElapsed) break;
        yield();
    }
    Prior_Micros = Now;
    // start DMA transfer to Update LEDs  :-)
#if defined(KINETISK)
    DMA->sourceBuffer(FrameBuffer, NumLed * 12);
    DMA->transferSize(1);
    DMA->transferCount(NumLed * 12);
    DMA->disableOnCompletion();
    DMA->enable();
#elif defined(KINETISL)
    DMA->CFG->SAR = FrameBuffer;
    DMA->CFG->DSR_BCR = 0x01000000;
    DMA->CFG->DSR_BCR = NumLed * 12;
    DMA->CFG->DCR = DMA_DCR_ERQ | DMA_DCR_CS | DMA_DCR_SSIZE(1) |
        DMA_DCR_SINC | DMA_DCR_DSIZE(1) | DMA_DCR_D_REQ;
#endif
}

