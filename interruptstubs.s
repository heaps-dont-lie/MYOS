.set IRQ_BASE, 0x20

.section text

# This is actually the name given to the handleInterrupt function in interrupts.cpp
# We are externing it so that we can jump to that function from assembly. Implemented in "int_bottom"
.extern _ZN16InterruptManager15handleInterruptEhj

.global _ZN16InterruptManager22IgnoreInterruptRequestEv

.macro HandleException num
.global _ZN16InterruptManager16HandleException\num\()Ev
_ZN16InterruptManager16HandleException\num\()Ev:
    movb $\num, (interruptNumber)
    jmp int_bottom
.endm

.macro HandleInterruptRequest num
.global _ZN16InterruptManager26HandleInterruptRequest\num\()Ev
_ZN16InterruptManager26HandleInterruptRequest\num\()Ev:
    movb $\num + IRQ_BASE, (interruptNumber)
    jmp int_bottom
.endm

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01

int_bottom:

    # Before we jump we need to save all the registers to remember the context
    # After the call return we can pop them.
    pusha
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    pushl %esp
    pushl (interruptNumber)
    call _ZN16InterruptManager15handleInterruptEhj
    movl %eax, %esp     # In the handleInterrupt function we are returning esp we dont need to pop the stack pointer.

    popl %gs
    popl %fs
    popl %es
    popl %ds
    popa

    iret

_ZN16InterruptManager22IgnoreInterruptRequestEv:
    iret


.data
    interruptNumber: .byte 0