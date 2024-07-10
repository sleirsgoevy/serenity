#include <Kernel/Arch/i386/DescriptorTable.h>
#include <Kernel/Arch/x86_64/TrapFrame.h>

extern "C" void syscall_entry();
NEVER_INLINE NAKED void syscall_entry()
{
    // clang-format off
    asm(
        "    pushl $0x0\n"
        "    pusha\n"
        "    pushl %ds\n"
        "    pushl %es\n"
        "    pushl %fs\n"
        "    pushl %gs\n"
        "    pushl %ss\n"
        "    mov $" __STRINGIFY(GDT_SELECTOR_DATA0) ", %ax\n"
        "    mov %ax, %ds\n"
        "    mov %ax, %es\n"
        "    mov $" __STRINGIFY(GDT_SELECTOR_PROC) ", %ax\n"
        "    mov %ax, %gs\n"
        "    cld\n"
        "    xor %esi, %esi\n"
        "    xor %edi, %edi\n"
        "    pushl %esp \n" // set TrapFrame::regs
        "    subl $" __STRINGIFY(TRAP_FRAME_SIZE - 4) ", %esp \n"
        "    movl %esp, %ebx \n"
        "    pushl %ebx \n" // push pointer to TrapFrame
        "    call enter_trap_no_irq \n"
        "    movl %ebx, 0(%esp) \n" // push pointer to TrapFrame
        "    call syscall_handler \n"
        "    movl %ebx, 0(%esp) \n" // push pointer to TrapFrame
        "    jmp common_trap_exit \n");
    // clang-format on
}
