.section ".text.startup"

.global _start
.global _get_stack_pointer
.global _exception_table
.global _enable_interrupts

.equ    CPSR_MODE_USER,                 0x10
.equ    CPSR_MODE_FIQ,                  0x11
.equ    CPSR_MODE_IRQ,                  0x12
.equ    CPSR_MODE_SVR,                  0x13
.equ    CPSR_MODE_ABORT,                0x17
.equ    CPSR_MODE_HYPERVISOR,           0x1A
.equ    CPSR_MODE_UNDEFINED,            0x1B
.equ    CPSR_MODE_SYSTEM,               0x1F
.equ    CPSR_MODE_MASK,                 0x1F

.equ    CPSR_IRQ_INHIBIT,               0x80
.equ    CPSR_FIQ_INHIBIT,               0x40
.equ    CPSR_THUMB,                     0x20

.equ    MAINID_ARMV6,                   0x410FB767
.equ    MAINID_ARMV7,                   0x410FC073
.equ    MAINID_ARMV8,                   0x410FD034

.equ    GIC_DISTB,                      0xff841000
.equ    GICC_PMR,                       0x4
.equ    GICD_CTRLR,                     0x0
.equ    GICD_IGROUPR,                   0x80
.equ    GIC_CPUB_offset,                0x1000

.equ    SCTLR_ENABLE_DATA_CACHE,        0x4
.equ    SCTLR_ENABLE_BRANCH_PREDICTION, 0x800
.equ    SCTLR_ENABLE_INSTRUCTION_CACHE, 0x1000

#define PRESCALER_2711                  0xff800008
#define MBOX_2711                       0xff8000cc

/* Jump Tamble */
_start:
    ldr pc, _reset_h
    ldr pc, _undefined_instruction_vector_h
    ldr pc, _software_interrupt_vector_h
    ldr pc, _prefetch_abort_vector_h
    ldr pc, _data_abort_vector_h
    ldr pc, _unused_handler_h
    ldr pc, _interrupt_vector_h
    ldr pc, _fast_interrupt_vector_h

_reset_h:                           .word    _reset_
_undefined_instruction_vector_h:    .word    undefined_instruction_vector
_software_interrupt_vector_h:       .word    software_interrupt_vector
_prefetch_abort_vector_h:           .word    prefetch_abort_vector
_data_abort_vector_h:               .word    data_abort_vector
_unused_handler_h:                  .word    _reset_
_interrupt_vector_h:                .word    interrupt_vector
_fast_interrupt_vector_h:           .word    fast_interrupt_vector

_reset_:
    mrs r12, CPSR
    and r12, #CPSR_MODE_MASK
    ldr r11, =_cpsr_startup_mode
    str r12, [r11]
    mrc p15, 0, r11, c0, c0, 0
    ldr r10, =#MAINID_ARMV6
    cmp r11, r10
    beq _setup_interrupt_table
    cmp r12, #CPSR_MODE_HYPERVISOR
    bne _multicore_park
    mrs r12, CPSR
    bic r12, r12, #CPSR_MODE_MASK
    orr r12, r12, #(CPSR_MODE_SVR | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT)
    msr SPSR_cxsf, r12
    add lr, pc, #4
    .word 0xE12EF30E
    .word 0xE160006E

/* Disable Multicore */
_multicore_park:
    mrc p15, 0, r12, c0, c0, 5
    ands r12, #0x3
    bne _inf_loop

_setup_interrupt_table:
    mov r0, #0x8000
    mov r1, #0x0000
    ldmia r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
    stmia r1!, {r2, r3, r4, r5, r6, r7, r8, r9}
    ldmia r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
    stmia r1!, {r2, r3, r4, r5, r6, r7, r8, r9}
    mov r0, #(CPSR_MODE_IRQ | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT)
    msr cpsr_c, r0
    ldr sp, =0x7000
    mov r0, #(CPSR_MODE_SVR | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT)
    msr cpsr_c, r0
    ldr sp, =0x8000

    /* Enable L1 Cache */
    mrc p15, 0, r0, c1, c0, 0
    orr r0, #SCTLR_ENABLE_BRANCH_PREDICTION
    orr r0, #SCTLR_ENABLE_DATA_CACHE
    orr r0, #SCTLR_ENABLE_INSTRUCTION_CACHE
    mcr p15, 0, r0, c1, c0, 0

    /* Enable VFP */
    mrc p15, #0, r1, c1, c0, #2
    orr r1, r1, #(0xf << 20)
    mcr p15, #0, r1, c1, c0, #2
    mov r1, #0
    mcr p15, #0, r1, c7, c5, #4
    mov r0,#0x40000000
    fmxr FPEXC, r0
    bl _cstartup

_inf_loop:
    b _inf_loop

_cpsr_startup_mode:    .word    0x0
_osc:                  .word    54000000
_value:                .word    0x63fff
_mbox:                 .word    MBOX_2711

_get_stack_pointer:
    str sp, [sp]
    ldr r0, [sp]
    mov pc, lr

_enable_interrupts:
    mrs r0, cpsr
    bic r0, r0, #CPSR_IRQ_INHIBIT
    msr cpsr_c, r0
    cpsie i
    mov pc, lr
