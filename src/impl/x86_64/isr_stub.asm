bits 64
extern idt_handler
global common_interrupt_handler

%macro isr_no_err_stub 1
global isr%1
isr%1:
    push qword 0
    push qword %1
    jmp common_interrupt_handler
%endmacro

%macro isr_err_stub 1
global isr%1
isr%1:
    push qword %1
    jmp common_interrupt_handler
%endmacro

isr_no_err_stub 0 ; Divide by zero
isr_no_err_stub 1 ; Debug
isr_no_err_stub 2 ; Non-Maskable Interrupt
isr_no_err_stub 3 ; Breakpoint
isr_no_err_stub 4 ; Overflow
isr_no_err_stub 5 ; Bound Range Exceeded
isr_no_err_stub 6 ; Invalid Opcode
isr_no_err_stub 7 ; Device Not Available
isr_err_stub 8 ; Double Fault
isr_no_err_stub 9 ; Co-processor Segment Overrun
isr_err_stub 10 ; Invalid TSS
isr_err_stub 11 ; Segment No Present
isr_err_stub 12 ; Stack-Segment Fault
isr_err_stub 13 ; General Protection Fault
isr_err_stub 14 ; Page Fault
isr_no_err_stub 15 ; Reserved
isr_no_err_stub 16 ; x87 Floating-Point Exception
isr_err_stub 17 ; Alignment Check
isr_no_err_stub 18 ; Machine Check
isr_err_stub 30 ; Security Exception

isr_no_err_stub 32 ; IRQ 0 - Timer
isr_no_err_stub 33 ; IRQ 1 - Keyboard

common_interrupt_handler:
    ; 1. Save all registers (The "State")
    push r15
    push r14
    push r13
    push r12
    push r11
    push r10
    push r9
    push r8
    push rbp
    push rdi
    push rsi
    push rdx
    push rcx
    push rbx
    push rax

    ; 2. Call the C handler
    mov rdi, rsp    ; Pass the stack pointer as the first argument (registers)
    call idt_handler

    ; 3. Restore all registers
    pop rax
    pop rbx
    pop rcx
    pop rdx
    pop rsi
    pop rdi
    pop rbp
    pop r8
    pop r9
    pop r10
    pop r11
    pop r12
    pop r13
    pop r14
    pop r15

    ; 4. Return from interrupt
    add rsp, 16
    iretq