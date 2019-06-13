#include <Arduino.h>
#include "SPILEDs.h"

#include "SPI.h"

/**
 * @brief Constructor
 * @param spi_bus - SPI bus to write to (e.g. HSPI, VSPI as defined in SPI.h)
 * @param cs_pin_in - Chip Select pin to use
 * @param num_leds_in - number of LEDs in strip
 */
SPILEDs::SPILEDs(uint8_t spi_bus, uint8_t cs_pin_in, uint8_t num_leds_in) {
    cs_pin = cs_pin_in;
    num_leds = num_leds_in;

    clear(); // set color and brightness values to 0

    spi = new SPIClass(spi_bus);
    spi->begin();
    pinMode(cs_pin, OUTPUT);
}

void SPILEDs::set_led(uint16_t led_num, uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness) {
    uint32_t led_frame = 0x00000000;
    led_frame |= (0b111 << 29); // frame start bits
    uint16_t brightness_bits = (static_cast<uint16_t>(brightness) * 0b11111 / 0xff) & 0b11111; // scale to 5 bits
    led_frame |= (brightness_bits << 24);
    led_frame |= (blue << 16);
    led_frame |= (green << 8);
    led_frame |= red;
    data_buffer[led_num + 1] = led_frame; // offset by 4 bytes for start frame
}

void SPILEDs::show() {
    data_buffer[0] = start_frame;
    data_buffer[num_leds + 1] = end_frame;
    spi->beginTransaction(SPISettings(spi_clk, MSBFIRST, SPI_MODE0));
    digitalWrite(cs_pin, LOW); // pull SS pin LOW
    spi->transferBytes((uint8_t *)data_buffer, NULL, (4 * num_leds) + 8); // 4 byte start frame, 4 bytes for each LED, 4 byte end frame
    digitalWrite(cs_pin, HIGH);
    spi->endTransaction();
}

void SPILEDs::clear() {
    for (size_t i = 0; i < num_leds; i++) {
        set_led(i, 0, 0, 0, 0);
    }
}

