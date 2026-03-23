# Solder Ninja Pen BSP - Porting Notes

## Status

Firmware links with the vendored Pico SDK 2.2.0 (not stubs):

```text
docker compose -f Env.yml run --rm builder sh -c \
  'cd source && make -j$(nproc) model=SolderNinjaPen firmware-EN'
```

Full debug info: add `debug=1` (default SNP build uses `-g`, not `-g3`).

Artifacts: `source/Hexfile/SolderNinjaPen_EN.{elf,hex,bin,dfu}`.

## FreeRTOS

RP2040 is Cortex-M0+. The V11.1.0 ARM_CM0 port (`port.c`, `portmacro.h`, `portasm.c`, `mpu_wrappers_v2_asm.c`) lives in this BSP.

`portmacro.h` includes `FreeRTOSConfig.h`, matching Miniware / Sequre. MPU is off. Tick width is 32-bit (`configTICK_TYPE_WIDTH_IN_BITS` is the numeric value 1). The `TICK_TYPE_WIDTH_*` constants are also defined in `FreeRTOSConfig.h` because `Buttons.hpp` and `ScrollMessage.hpp` include `portmacro.h` before `FreeRTOS.h`.

### Heaps

IronOS tasks are static (`configSUPPORT_DYNAMIC_ALLOCATION` is 0). There is no `heap_4.c` and no `configTOTAL_HEAP_SIZE` — that macro would not reserve RAM.

Pico `crt0` still reserves a small libc heap (`PICO_HEAP_SIZE`, often 2 KB) for `malloc` / `calloc`. Leave that at the SDK default.

### LTO

SNP strips `-flto=auto` from the shared Makefile `OPTIM` list and adds `-fno-lto`. That is the only switch. LTO stays off because ARM_CM0 `portasm` uses a short branch to `vPortSVCHandler_C` (JUMP11 when the linker moves the target), and because a mixed LTO compile / non-LTO link drops `main`.

## Pico SDK

`Vendor/pico-sdk/` is a pruned, unpatched 2.2.0 subset (`src/` + license). Do not edit files under it.

Make overlays:

- `pico_overlay/pico/config_autogen.h` (included ahead of any SDK copy)
- `pico_overlay/board_solder_ninja_pen.h` (not the stock Pico board: no LED on GP25)

Include paths stay curated (SDK `include` roots only). Sources under Vendor are discovered with `find`, excluding `boot_stage2` and `boards`.

Startup is Pico `crt0.S`, padded `bs2_default_padded_checksummed.S`, and `irq_handler_chain.S`. Linker script: `rp2040.ld`. Scratch X/Y stay unused (single core).

## I2C

I2C0 is the hardware controller at 400 kHz (GP4/GP5), wrapped by `FRToSI2C` in `I2C_Wrapper.cpp`. Do not enable `I2C_SOFT_BUS_*`. IronOS passes 8-bit addresses; the wrapper shifts to 7-bit. `unstick_I2C()` bit-bangs SCL if a slave holds SDA, then calls `setup_i2c()` again.

## Upstream issue (draft)

`Buttons.hpp` and `ScrollMessage.hpp` include `portmacro.h` only to get `TickType_t`. That couples app headers to the BSP portable layer and breaks the V11 ARM_CM0 port unless `FreeRTOSConfig.h` is pulled in early (undefined `TICK_TYPE_WIDTH_*` tokens become 0 in `#if` and silently select 16-bit ticks).

Preferred fix in IronOS: include `FreeRTOS.h` (or a small shared typedef) instead of `portmacro.h`. SNP does not change those shared headers; it follows the other BSPs and includes `FreeRTOSConfig.h` from `portmacro.h`.

## Known TODOs

1. **Buck converter DAC**: `setTipPWM()` only toggles buck enable. Needs SPI to the DAC5311.
2. **EEPROM**: settings are in internal flash; optional I2C EEPROM at 0x50.
3. **PI3USB9281C**: BC1.2 / HVDCP over I2C.
4. **FSUSB43 mux**: GPIO routing of USB data to the QC network.
5. **LIS2DH12 IRQs**: GPIO 12/13 are inputs; no IRQ handler yet.
6. **MAX31855 calibration**: verify coefficients on hardware.
7. **PID**: tune for time-proportional burst heating.
8. **FUSB302**: polled (`getFUS302IRQLow()` always true). Confirm that stays acceptable.
9. **VIN / handle temp**: `getInputVoltageX10` and `getHandleTemperature` are still stubs. See `PEER_BSP_COMPARISON.md`.
