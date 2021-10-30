//
// Created by shifu on 2021/10/17.
//

asm(R"(
.include "aarch64/asm_macros.hh"

.macro entry_invalid_exception
.align 7
    // If we got to here, the exception came from the host or EL2.
    //
    // Hang, relying on the lockup detector or watchdog to kick in.
    1:
    b 1b
.endm

.align 11
    FUNCTION_LABEL arm64_el2_init_table
    /* exceptions from current EL, using SP0 */
    entry_invalid_exception
    entry_invalid_exception
    entry_invalid_exception
    entry_invalid_exception

    /* exceptions from current EL, using SPx */
    entry_invalid_exception
    entry_invalid_exception
    entry_invalid_exception
    entry_invalid_exception

    /* exceptions from lower EL, running arm64 */
    entry_invalid_exception // todo: currently unimplemented
    entry_invalid_exception
    entry_invalid_exception
    entry_invalid_exception

    /* exceptions from lower EL, running arm32 */
    entry_invalid_exception
    entry_invalid_exception
    entry_invalid_exception
    entry_invalid_exception
)");
