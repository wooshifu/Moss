# aarch64 specific compiler flags
# --target=aarch64-unknown-linux-elf
# -march=armv8-a+crc+fp
# -mcpu=cortex-a53
# -mfpu=fp-armv8
# -mfloat-abi=soft

# -DARCH_ARM=1 # arch arm
# -DARCH_AARCH64=1 # arch aarch64
# -DBITS_64=1 # 64 bits

function(get_compiler_flags OUT_flags)
    set(flags "-DARCH_ARM=1 -DARCH_AARCH64=1 -DBITS_64=1 -D__aarch64__=1")
    if (USING_CLANG_COMPILER)
        # TODO: set(${OUT_flags} "${flags} --target=aarch64-unknown-none-elf -march=armv8-a+crc+fp" PARENT_SCOPE) will cause lld failed
        # add -U__linux__ to indicate that this is not a linux system
        set(${OUT_flags} "${flags} -U__linux__ --target=aarch64-unknown-linux-elf -march=armv8-a+crc+fp" PARENT_SCOPE)
    elseif (USING_GNU_COMPILER)
        set(${OUT_flags} "${flags} -mcpu=cortex-a53" PARENT_SCOPE)
    endif ()
endfunction()
