#pragma once

void inline enable_interrupt() { asm volatile("msr daifclr, #2"); }
void inline disable_interrupt() { asm volatile("msr daifset, #2"); }
