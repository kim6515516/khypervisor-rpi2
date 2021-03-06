/*
 * boot.S - simple register setup code for stand-alone Linux booting
 *
 * Copyright (C) 2011 ARM Limited. All rights reserved.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE.txt file.
 */

#define NS_BIT	0x1
#include <k-hypervisor-config.h>
    .syntax	unified
    .arch_extension sec
    .arch_extension virt
    .text

.macro enter_hyp
    @ We assume we're entered in Secure Supervisor mode. To
    @ get to Hyp mode we have to pass through Monitor mode
    @ and NS-Supervisor mode. Note that there is no way to
    @ return to the Secure world once we've done this.
    @
    @ This will trash r10 and r11.
    ldr	r10, =vectors
    mcr	p15, 0, r10, c12, c0, 1		@ Monitor vector base address
    @ Switch to monitor mode, which will set up the HVBAR and
    @ then return to us in NS-SVC
    smc	#0
    @ Now we're in NS-SVC, make a Hyp call to get into Hyp mode
    hvc	#0
    @ We will end up here in NS-Hyp.
.endm


.align 5
/* We use the same vector table for Hyp and Monitor mode, since
 * we will only use each once and they don't overlap.
 */
vectors:
    .word 0	/* reset */
    .word 0	/* undef */
    b	2f /* smc */
    .word 0 /* pabt */
    .word 0 /* dabt */
    b	1f
    .word 0 /* irq */
    .word 0 /* fiq */

/* Return directly back to the caller without leaving Hyp mode: */
1:
    mrs	lr, elr_hyp
    mov	pc, lr

/* In monitor mode, set up HVBAR and SCR then return to caller in NS-SVC. */
2:
    @ Set up HVBAR
    mrc	p15, 0, r10, c1, c1, 0		@ SCR
    @ Set SCR.NS=1 (needed for setting HVBAR and also returning to NS state)
    @        .IRQ,FIQ,EA=0 (don't take aborts/exceptions to Monitor mode)
    @        .FW,AW=1 (CPSR.A,F modifiable in NS state)
    @        .nET=0 (early termination OK)
    @        .SCD=1 (SMC in NS mode is UNDEF, so accidental SMCs don't
    @                cause us to leap back into this code confusingly)
    @        .HCE=1 (HVC does Hyp call)
    bic	r10, r10, #0x07f
    ldr	r11, =0x1b1
    orr	r10, r10, r11
    mcr	p15, 0, r11, c1, c1, 0
    isb
    ldr	r11, =vectors
    mcr	p15, 4, r11, c12, c0, 0		@ set HVBAR
    @ ...and return to calling code in NS state
    movs	pc, lr

    .globl	start
start:
    @
    @ Program architected timer frequency
    @
    mrc	p15, 0, r0, c0, c1, 1		@ CPUID_EXT_PFR1
    lsr	r0, r0, #16
    @ Check generic timer support
#    and	r0, r0, #1
#    beq	1f

#    ldr	r0, = CFG_CNTFRQ
#    mcr	p15, 0, r0, c14, c0, 0		@ CNTFRQ

1:
    @
    @ CPU initialisation
    @
    mrc	p15, 0, r4, c0, c0, 5		@ MPIDR (ARMv7 only)
    and	r4, r4, #15			@ CPU number

    @
    @ Hypervisor / TrustZone initialization
    @

    @ Set all interrupts to be non-secure
#    ldr	r0, = CFG_GIC_BASE_PA+0x1000	@ Dist GIC base
#    ldr	r1, [r0, #0x04]			@ Type Register
#    cmp	r4, #0
#    andeq	r1, r1, #0x1f
#    movne	r1, #0
#    add	r2, r0, #0x080			@ Security Register 0
#    mvn	r3, #0
2:
#    str	r3, [r2]
#    sub	r1, r1, #1
#    add	r2, r2, #4			@ Next security register
#    cmp	r1, #-1
#    bne	2b

    @ Set GIC priority mask bit [7] = 1
#    ldr	r0, = CFG_GIC_BASE_PA+0x2000	@ CPU GIC base
#    mov	r1, #0x80
#    str	r1, [r0, #0x4]			@ GIC ICCPMR

    @ Set NSACR to allow coprocessor access from non-secure
#    mrc	p15, 0, r0, c1, c1, 2
#    ldr	r1, =0x43fff
#    orr	r0, r0, r1
#    mcr	p15, 0, r0, c1, c1, 2

    @ Check CPU nr again
    @ MPIDR (ARMv7 only)
    mrc	p15, 0, r0, c0, c0, 5
    @ CPU number, taking multicluster into account
    bfc	r0, #24, #8
    cmp	r0, #0				@ primary CPU?
    beq	2f

2:
    @
    @ UART initialisation (38400 8N1)
    @
#    ldr	r0, =0x1c090000			@ UART base (Versatile Express)

/* Initialize bss section */
    ldr  r2, =begin_bss
    ldr  r3, =end_bss
    mov  r0, #0
1:  str  r0, [r2], #4    @ clear bss
    cmp  r2, r3
    blo  1b
    @ Now we've got rid of the secondary CPUs, set up a stack
    @ for CPU 0 so we can write most of this in C.
#    ldr     sp, =sec_stacklimit



	@ make CNTP_* and CNTPCT accessible from PL1
	mrc	p15, 0, r7, c0, c1, 1	@ ID_PFR1
	lsr	r7, #16
	and	r7, #0xf
	cmp	r7, #1
	bne	1f
	mrc	p15, 4, r7, c14, c1, 0	@ CNTHCTL
	orr	r7, r7, #3		@ PL1PCEN | PL1PCTEN
	mcr	p15, 4, r7, c14, c1, 0	@ CNTHCTL
	mov	r7, #0
	mcrr	p15, 4, r7, r7, c14	@ CNTVOFF

	@ Disable virtual timer in case it was counting
	mrc	p15, 0, r7, c14, c3, 1	@ CNTV_CTL
	bic	r7, #1			@ Clear ENABLE
	mcr	p15, 0, r7, c14, c3, 1	@ CNTV_CTL
1:








#    bl mon_init
    bl mon_to_hyp   // Secure -> NS.Hyp -> hyp_main()

    @ Never reached
1:
    b 1b

#ifdef _SMP_
    .globl  init_secondary
init_secondary:

    cpsid aif                    /* Disable all interrupts */
    @ Check CPU nr again
    mrc p15, 0, r0, c0, c0, 5       @ MPIDR (ARMv7 only)
    @ CPU number, taking multicluster into account
    bfc r0, #24, #8
    cmp r0, #0              @ primary CPU?
    beq 1f
    ldr     sp, =mon_stacklimit
    mrc     p15, 0, r0, c0, c0, 5
    ands    r0, r0, #0xFF
    mov r1, #MON_CORE_STACK_SIZE
    mul r1, r1, r0
    sub sp, sp, r1
    /* Place the right Hyp exception vector for the first time */
    ldr r0, = hyp_init_vectors
    mcr p15, 4, r0, c12, c0, 0
    bl main
    @ never come back
    1:
    b 1b

.globl reset_for_debugging
reset_for_debugging:
   @ Check CPU nr again
    mrc p15, 0, r0, c0, c0, 5
    @ CPU number, taking multicluster into account
    bfc r0, #24, #8
    cmp r0, #0              @ primary CPU?
    beq 1f
    /* Base address of system controller */
#define EXYNOS5_PA_PMU              0x10040000
#define EXYNOS5_SWRESET             0x0400      /* Relative to PA_PMU */

    ldr r0, =EXYNOS5_PA_PMU
    mov r1, #0x01
    str r1, [r0, #EXYNOS5_SWRESET]
    mov pc, lr
.type reset_for_debugging, % function
#endif

/*
 * Initialize vector and stack pointers for modes:
 * Secure SVC, Non-secure SVC, and Hyp mode
 */
.global mon_init
mon_init:
    push {r0 - r2, lr}
    @ Setup Exception Vector
    ldr r1, = monitor_secure_vectors
    mcr p15, 0, r1, c12, c0, 1      @ Monitor vector base address
    @ regular return
    pop {r0 - r2, pc}
.type mon_init, % function

.global mon_to_hyp
mon_to_hyp:
#    smc #0  @ -> trap_smc
#    hvc #0  @ -> trap_hyp_entry

    /* Place the right Hyp exception vector for the first time */
    ldr r0, = hyp_init_vectors
    mcr p15, 4, r0, c12, c0, 0
    bl main
    @ never come back
    1:
    b 1b
.type mon_to_hyp, % function

.global __mon_switch_to_guest_context
__mon_switch_to_guest_context:    @ struct arch_regs *r0
    ldmfd    r0!, {r1, r3, lr}
    msr     spsr_hyp, r1
    msr     elr_hyp, r3
    ldm     r0, {r0-r12}
    clrex
    eret
.type __mon_switch_to_guest_context, %function



.globl PUT32
PUT32:
    str r1,[r0]
    bx lr

.globl PUT16
PUT16:
    strh r1,[r0]
    bx lr

.globl PUT8
PUT8:
    strb r1,[r0]
    bx lr

.globl GET32
GET32:
    ldr r0,[r0]
    bx lr

.globl GETPC
GETPC:
    mov r0,lr
    bx lr

.globl BRANCHTO
BRANCHTO:
    bx r0



put_str:
	ldr r3, [r0, #FR] // get flag register
	ands r3, r3, #FR_TXFF // transmit buffer full?
	bne put_str // try again
	strb	r2, [r0, #DR] // transmit char
check_0:
	ldrb r2, [r1, #1]! // get next char and increment
	cmp	r2, #0 // 0 byte?
	bne put_str // keep printing if not
// echo input
loop:
wait_receive:
	ldr r3, [r0, #FR] // get flag register
	ands r3, r3, #FR_RXFE // receive buffer empty?
	bne wait_receive // try again
	ldrb r2, [r0, #DR] // receive char
wait_transmit:
	ldr r3, [r0, #FR] // get flag register
	ands r3, r3, #FR_TXFF // transmit buffer full?
	bne wait_transmit // try again
	strb	r2, [r0, #DR] // transmit char
	b loop	// keep going

// constants from ldr macros
constants:
.align 2

str:    .asciz " boot khypervisor...\n"
