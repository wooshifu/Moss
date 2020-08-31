.globl init_exception_vector_table
init_exception_vector_table:
    adr    x0, exception_vectors_table        // load VBAR_EL1 with virtual
    msr    vbar_el1, x0 /* Vector Base Address Register (EL1) */
    ret

.globl enable_irq
enable_irq:
    msr    daifclr, #2
    ret

.globl disable_irq
disable_irq:
    msr    daifset, #2
    ret
