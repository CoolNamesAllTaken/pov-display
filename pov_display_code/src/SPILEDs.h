#ifndef SPI_LEDS_H
#define SPI_LEDS_H

#include <SPI.h>

#define MAX_NUM_LEDS 500

class SPILEDs {

public:
    /** Public Variables **/
    static const uint32_t spi_clk = 1200e3; // 1200 kHz = APA102 max clock rate
    static const uint32_t start_frame = 0x00000000;
    static const uint32_t end_frame = 0x11111111;

    /** Public Functions **/
    SPILEDs(uint8_t spi_bus, uint8_t cs_pin_in, uint8_t num_leds_in);
    void set_led(uint16_t led_num, uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness);
    void show();
    void clear();

private:
    /** Private Variables **/
    SPIClass * spi;
    uint8_t cs_pin;
    uint8_t num_leds;
    uint32_t data_buffer[MAX_NUM_LEDS + 2]; // 4 byte start frame, 4 bytes for each LED, 4 byte end frame
};

#endif /* SPI_LEDS_H */