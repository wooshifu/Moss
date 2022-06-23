
    .rodata : ALIGN(4096) {
        __rodata_start = .;
        *(.rodata .rodata.* .gnu.linkonce.r.*)
    }

    /*
     * extra linker scripts tend to insert sections just after .rodata,
     * so we want to make sure this symbol comes after anything inserted above,
     * but not aligned to the next section necessarily.
     */
    .dummy_post_rodata : {
        __rodata_end = .;
    }
