# aarch64 required compiler flags
# compiler flags could be put in one line, or put it in separate line
# "#" started line is comment
# "-key=value # comment" is also valid comment

# aarch64 specific compiler flags
#--target=aarch64-unknown-linux-elf
#-march=armv8-a+crc+fp
# -mcpu=cortex-a53
# -mfpu=fp-armv8
# -mfloat-abi=soft

#-DARCH_ARM=1 # arch arm
#-DARCH_AARCH64=1 # arch aarch64
#-DBITS_64=1 # 64 bits

function(get_compiler_flags OUT_flags)
    set(flags "-DARCH_ARM=1 -DARCH_AARCH64=1 -DBITS_64=1")
    if (USING_CLANG_COMPILER)
        set(${OUT_flags} "${flags} --target=aarch64-unknown-linux-elf -march=armv8-a+crc+fp" PARENT_SCOPE)
    elseif (USING_GNU_COMPILER)
        set(${OUT_flags} "${flags} -mcpu=cortex-a53" PARENT_SCOPE)
    endif ()
endfunction()
