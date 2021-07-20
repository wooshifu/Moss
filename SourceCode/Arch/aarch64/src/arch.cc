#include "kconfig.hh"
#include "kernel/compiler.hh"
#include "libcxx/types.hh"

struct arm64_sp_info_t {
  uint64_t mpid;
  void* sp;                  // Stack pointer points to arbitrary data.
  uintptr_t* shadow_call_sp; // SCS pointer points to array of addresses.

  // This part of the struct itself will serve temporarily as the
  // fake arch_thread in the thread pointer, so that safe-stack
  // and stack-protector code can work early.  The thread pointer
  // (TPIDR_EL1) points just past arm64_sp_info_t.
  uintptr_t stack_guard;
  void* unsafe_sp;
};

static_assert(sizeof(arm64_sp_info_t) == 40, "check arm64_get_secondary_sp assembly");
static_assert(offsetof(arm64_sp_info_t, sp) == 8, "check arm64_get_secondary_sp assembly");
static_assert(offsetof(arm64_sp_info_t, mpid) == 0, "check arm64_get_secondary_sp assembly");

// one for each CPU
arm64_sp_info_t arm64_secondary_sp_list[CONFIG_SMP_MAX_CPUS];

#if 0
extern uint64_t arch_boot_el;  // Defined in start.S.

uint64_t arm64_get_boot_el() { return arch_boot_el >> 2; }

zx_status_t arm64_create_secondary_stack(cpu_num_t cpu_num, uint64_t mpid) {
  // Allocate a stack, indexed by CPU num so that |arm64_secondary_entry| can find it.
  DEBUG_ASSERT_MSG(cpu_num > 0 && cpu_num < SMP_MAX_CPUS, "cpu_num: %u", cpu_num);
  KernelStack* stack = &_init_thread[cpu_num - 1].stack();
  DEBUG_ASSERT(stack->base() == 0);
  zx_status_t status = stack->Init();
  if (status != ZX_OK) {
    return status;
  }

  // Get the stack pointers.
  void* sp = reinterpret_cast<void*>(stack->top());
  void* unsafe_sp = nullptr;
  uintptr_t* shadow_call_sp = nullptr;
#if __has_feature(safe_stack)
  DEBUG_ASSERT(stack->unsafe_base() != 0);
  unsafe_sp = reinterpret_cast<void*>(stack->unsafe_top());
#endif
#if __has_feature(shadow_call_stack)
  DEBUG_ASSERT(stack->shadow_call_base() != 0);
  // The shadow call stack grows up.
  shadow_call_sp = reinterpret_cast<uintptr_t*>(stack->shadow_call_base());
#endif

  // Find an empty slot for the low-level stack info.
  uint32_t i = 0;
  while ((i < SMP_MAX_CPUS) && (arm64_secondary_sp_list[i].mpid != 0)) {
    i++;
  }
  if (i == SMP_MAX_CPUS) {
    return ZX_ERR_NO_RESOURCES;
  }

  // Store it.
  LTRACEF("set mpid 0x%lx sp to %p\n", mpid, sp);
#if __has_feature(safe_stack)
  LTRACEF("set mpid 0x%lx unsafe-sp to %p\n", mpid, unsafe_sp);
#endif
#if __has_feature(shadow_call_stack)
  LTRACEF("set mpid 0x%lx shadow-call-sp to %p\n", mpid, shadow_call_sp);
#endif
  arm64_secondary_sp_list[i].mpid = mpid;
  arm64_secondary_sp_list[i].sp = sp;
  arm64_secondary_sp_list[i].stack_guard = Thread::Current::Get()->arch().stack_guard;
  arm64_secondary_sp_list[i].unsafe_sp = unsafe_sp;
  arm64_secondary_sp_list[i].shadow_call_sp = shadow_call_sp;

  return ZX_OK;
}

zx_status_t arm64_free_secondary_stack(cpu_num_t cpu_num) {
  DEBUG_ASSERT(cpu_num > 0 && cpu_num < SMP_MAX_CPUS);
  return _init_thread[cpu_num - 1].stack().Teardown();
}

static void arm64_cpu_early_init() {
  // Make sure the per cpu pointer is set up.
  arm64_init_percpu_early();

  // Set the vector base.
  __arm_wsr64("vbar_el1", (uint64_t)&arm64_el1_exception_base);
  __isb(ARM_MB_SY);

  // Set some control bits in sctlr.
  arch::ArmSctlrEl1::Modify([](auto& sctlr) {
    sctlr.set_uci(true)
        .set_span(true)
        .set_ntwe(true)
        .set_uct(true)
        .set_dze(true)
        .set_sa0(true)
        .set_sa(true)
        .set_ntwi(false)  // Disable WFI in EL0
        .set_a(false);    // Disable alignment checking for EL1, EL0.
  });
  __isb(ARM_MB_SY);

  // Save all of the features of the cpu.
  arm64_feature_init();

  // Enable cycle counter.
  __arm_wsr64("pmcr_el0", PMCR_EL0_ENABLE_BIT | PMCR_EL0_LONG_COUNTER_BIT);
  __isb(ARM_MB_SY);
  __arm_wsr64("pmcntenset_el0", PMCNTENSET_EL0_ENABLE);
  __isb(ARM_MB_SY);

  // Enable user space access to cycle counter.
  __arm_wsr64("pmuserenr_el0", PMUSERENR_EL0_ENABLE);
  __isb(ARM_MB_SY);

  // Enable Debug Exceptions by Disabling the OS Lock. The OSLAR_EL1 is a WO
  // register with only the low bit defined as OSLK. Write 0 to disable.
  __arm_wsr64("oslar_el1", 0x0);
  __isb(ARM_MB_SY);

  // Enable user space access to virtual counter (CNTVCT_EL0).
  __arm_wsr64("cntkctl_el1", CNTKCTL_EL1_ENABLE_VIRTUAL_COUNTER);
  __isb(ARM_MB_SY);

  __arm_wsr64("mdscr_el1", MSDCR_EL1_INITIAL_VALUE);
  __isb(ARM_MB_SY);

  arch_enable_fiqs();
}

void arch_early_init() {
  // put the cpu in a working state and read the feature flags
  arm64_cpu_early_init();

  // give the mmu code a chance to do some bookkeeping
  arm64_mmu_early_init();
}

void arch_prevm_init() {}

void arch_init() TA_NO_THREAD_SAFETY_ANALYSIS {
  arch_mp_init_percpu();

  dprintf(INFO, "ARM boot EL%lu\n", arm64_get_boot_el());

  arm64_feature_debug(true);

  uint32_t max_cpus = arch_max_num_cpus();
  uint32_t cmdline_max_cpus = gCmdline.GetUInt32(kernel_option::kSmpMaxCpus, max_cpus);
  if (cmdline_max_cpus > max_cpus || cmdline_max_cpus <= 0) {
    printf("invalid kernel.smp.maxcpus value, defaulting to %u\n", max_cpus);
    cmdline_max_cpus = max_cpus;
  }

  secondaries_to_init = cmdline_max_cpus - 1;

  lk_init_secondary_cpus(secondaries_to_init);

  LTRACEF("releasing %d secondary cpus\n", secondaries_to_init);
  secondaries_released.store(true);

  // Flush the signaling variable since the secondary cpus may have not yet enabled their caches.
  arch_clean_cache_range((vaddr_t)&secondaries_released, sizeof(secondaries_released));
}

void arch_late_init_percpu(void) {
  arm64_read_percpu_ptr()->should_invalidate_bp_on_context_switch =
      !gBootOptions->arm64_disable_spec_mitigations && arm64_uarch_needs_spectre_v2_mitigation();
}

__NO_RETURN int arch_idle_thread_routine(void*) {
  for (;;) {
    __asm__ volatile("wfi");
  }
}

void arch_setup_uspace_iframe(iframe_t* iframe, uintptr_t pc, uintptr_t sp, uintptr_t arg1,
                              uintptr_t arg2) {
  // Set up a default spsr to get into 64bit user space:
  //  - Zeroed NZCV.
  //  - No SS, no IL, no D.
  //  - All interrupts enabled.
  //  - Mode 0: EL0t.
  //
  // TODO: (hollande,travisg) Need to determine why some platforms throw an
  //         SError exception when first switching to uspace.
  uint32_t spsr = 1 << 8;  // Mask SError exceptions (currently unhandled).

  iframe->r[0] = arg1;
  iframe->r[1] = arg2;
  iframe->usp = sp;
  iframe->elr = pc;
  iframe->spsr = spsr;

  iframe->mdscr = MSDCR_EL1_INITIAL_VALUE;
}

// Switch to user mode, set the user stack pointer to user_stack_top, put the svc stack pointer to
// the top of the kernel stack.
void arch_enter_uspace(iframe_t* iframe) {
  Thread* ct = Thread::Current::Get();

  LTRACEF("arm_uspace_entry(%#" PRIxPTR ", %#" PRIxPTR ", %#" PRIxPTR ", %#" PRIxPTR ", %#" PRIxPTR
          ", 0, %#" PRIxPTR ")\n",
          iframe->r[0], iframe->r[1], iframe->spsr, ct->stack().top(), iframe->usp, iframe->elr);

  arch_disable_ints();

  ASSERT(arch_is_valid_user_pc(iframe->elr));

  arm64_uspace_entry(iframe, ct->stack().top());
  __UNREACHABLE;
}
#endif

// called from assembly.
// todo: 此处未初始化 secondary cpu, 因此 SMP 未实现
extern "C" [[noreturn]] void arm64_secondary_entry() {
#if 0
  arm64_cpu_early_init();

  // Wait until the primary has finished setting things up.
  while (!secondaries_released.load()) {
    arch::Yield();
  }

  cpu_num_t cpu = arch_curr_cpu_num();
  _init_thread[cpu - 1].SecondaryCpuInitEarly();
  // Run early secondary cpu init routines up to the threading level.
  lk_init_level(LK_INIT_FLAG_SECONDARY_CPUS, LK_INIT_LEVEL_EARLIEST, LK_INIT_LEVEL_THREADING - 1);

  arch_mp_init_percpu();

  const bool full_dump = arm64_feature_current_is_first_in_cluster();
  arm64_feature_debug(full_dump);

  lk_secondary_cpu_entry();
#endif
  while (true) {
    asm volatile(R"asm(
      nop
      nop
      nop
      nop
      nop
      nop
      nop
    )asm");
  }
}

#if 0
static int cmd_cpu(int argc, const cmd_args* argv, uint32_t flags) {
  auto usage = [cmd_name = argv[0].str]() -> int {
    printf("usage:\n");
    printf("%s sev                              : issue a SEV (Send Event) instruction\n",
           cmd_name);
    return ZX_ERR_INTERNAL;
  };

  if (argc < 2) {
    printf("not enough arguments\n");
    return usage();
  }

  if (!strcmp(argv[1].str, "sev")) {
    __asm__ volatile("sev");
    printf("done\n");
  } else {
    printf("unknown command\n");
    return usage();
  }

  return ZX_OK;
}

STATIC_COMMAND_START
STATIC_COMMAND("cpu", "cpu diagnostic commands", &cmd_cpu)
STATIC_COMMAND_END(cpu)

#endif
