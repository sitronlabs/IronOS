# Solder Ninja Pen BSP

Board Support Package for the [Solder Ninja Pen](https://www.crowdsupply.com/sitron-labs/solder-ninja-pen), a USB-C soldering iron based on the RP2040 microcontroller.

## Hardware Summary

| Component | Details |
|-----------|---------|
| MCU | RP2040 (Dual Cortex-M0+, 125MHz, 264KB SRAM, 2MB Flash) |
| Display | SSD1306 OLED, 96×16 px, I2C @ 0x3C |
| USB-PD PHY | FUSB302, I2C @ 0x22 |
| Charger Detection | PI3USB9281C, I2C @ 0x25 |
| USB Mux | FSUSB43 |
| Thermocouple AFE | MAX31855, SPI1 @ 5MHz |
| Buck Converter DAC | DAC5311, SPI1 @ 8MHz |
| Accelerometer | LIS2DH12, I2C @ 0x18 |
| EEPROM | M24C64, I2C @ 0x50 |
| Heater | 2.1Ω element, time-proportional burst via buck enable |

## Pin Map

See `Pins.h` for the complete GPIO assignment.

## Build

```bash
make model=SolderNinjaPen firmware-EN
```

## Dependencies

The Pico SDK must be available and its path set via the `PICO_SDK_PATH` environment variable. The FreeRTOS Cortex-M0 port files are included in the `Vendor/` directory.

## Status

This is an initial port scaffold. See [GitHub Issue #1989](https://github.com/Ralim/IronOS/issues/1989) for discussion.
