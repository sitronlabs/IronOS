/* Linker script for RP2040 (Solder Ninja Pen)
 *
 * RP2040 memory map:
 *   Flash (XIP): 0x10000000, 2MB
 *   SRAM:        0x20000000, 264KB (striped across 4 banks + 2x4KB scratch)
 *
 * The RP2040 boot ROM loads the second stage bootloader from flash offset 0,
 * then jumps to the application at offset 0x100 (after the 256-byte boot2).
 *
 * Layout:
 *   0x10000000 - 0x100000FF : Second stage bootloader (boot2)
 *   0x10000100 - ...        : Application code + data
 *   ...        - 0x101FFFFF : Flash end (logo, settings at end)
 */

MEMORY
{
    BOOT2  (rx)  : ORIGIN = 0x10000000, LENGTH = 0x100
    FLASH  (rx)  : ORIGIN = 0x10000100, LENGTH = 2044k
    RAM    (rwx) : ORIGIN = 0x20000000, LENGTH = 256k
    SCRATCH_X (rwx) : ORIGIN = 0x20040000, LENGTH = 4k
    SCRATCH_Y (rwx) : ORIGIN = 0x20041000, LENGTH = 4k
}

ENTRY(_entry_point)

SECTIONS
{
    /* Second stage bootloader */
    .boot2 : {
        __boot2_start__ = .;
        KEEP (*(.boot2))
        __boot2_end__ = .;
    } > BOOT2

    ASSERT(__boot2_end__ - __boot2_start__ == 256,
        "ERROR: Pico second stage bootloader must be 256 bytes in size")

    /* Vector table and code */
    .text : {
        __logical_binary_start = .;
        KEEP (*(.vectors))
        KEEP (*(.binary_info_header))
        __binary_info_header_end = .;
        KEEP (*(.reset))
        *(.time_critical*)
        *(.text*)
        *(.rodata*)
        . = ALIGN(4);
        *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.flashdata*)))
        . = ALIGN(4);
    } > FLASH

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH

    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > FLASH
    __exidx_end = .;

    /* Machine inspectable binary information */
    . = ALIGN(4);
    __binary_info_start = .;
    .binary_info :
    {
        KEEP(*(.binary_info.keep.*))
        *(.binary_info.*)
    } > FLASH
    __binary_info_end = .;

    . = ALIGN(4);
    _etext = .;
    __etext = .;

    .data : {
        __data_start__ = .;
        *(vtable)
        *(.data*)
        . = ALIGN(4);
        *(.after_data.*)
        . = ALIGN(4);
        *(.mutual_exclusion)
        . = ALIGN(4);
        __data_end__ = .;
    } > RAM AT> FLASH

    .uninitialized_data (NOLOAD): {
        . = ALIGN(4);
        *(.uninitialized_data*)
    } > RAM

    /* BSS section */
    .bss  : {
        . = ALIGN(4);
        __bss_start__ = .;
        *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.bss*)))
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
    } > RAM

    .heap (NOLOAD):
    {
        __end__ = .;
        end = __end__;
        *(.heap*)
        __HeapLimit = .;
    } > RAM

    /* .stack_dummy section doesn't contain any symbols. It is only
     * used for linker to calculate size of stack sections, and assign
     * values to stack symbols later */
    .stack_dummy (NOLOAD):
    {
        *(.stack*)
    } > RAM

    /* Set stack top to end of RAM, and stack limit move down by
     * size of stack_dummy section */
    __StackTop = ORIGIN(RAM) + LENGTH(RAM);
    __StackLimit = __StackTop - SIZEOF(.stack_dummy);
    PROVIDE(__stack = __StackTop);

    /* Used for flash operations */
    __flash_binary_end = LOADADDR(.data) + SIZEOF(.data);

    /* Scratch memory for core 0 and core 1 stacks */
    .scratch_x : {
        __scratch_x_start__ = .;
        *(.scratch_x.*)
        . = ALIGN(4);
        __scratch_x_end__ = .;
    } > SCRATCH_X AT > FLASH
    __scratch_x_source__ = LOADADDR(.scratch_x);

    .scratch_y : {
        __scratch_y_start__ = .;
        *(.scratch_y.*)
        . = ALIGN(4);
        __scratch_y_end__ = .;
    } > SCRATCH_Y AT > FLASH
    __scratch_y_source__ = LOADADDR(.scratch_y);

    /* Provide symbols for FLASH_SIZE and BOOTLDR_SIZE if not already defined */
    PROVIDE(__FLASH_SIZE__ = LENGTH(FLASH) + LENGTH(BOOT2));
    PROVIDE(__BOOTLDR_SIZE__ = LENGTH(BOOT2));
}
