/*
 * Copyright (c) 2018-2021, Andreas Kling <kling@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Types.h>
#include <sys/arch/regs.h>

#include <Kernel/Arch/CPU.h>
#include <Kernel/Arch/x86_64/ASM_wrapper.h>
#include <Kernel/Security/ExecutionMode.h>

#include <AK/Platform.h>
VALIDATE_IS_X86()

namespace Kernel {

struct [[gnu::packed]] RegisterState {
    FlatPtr ss;
    FlatPtr gs;
    FlatPtr fs;
    FlatPtr es;
    FlatPtr ds;
    FlatPtr edi;
    FlatPtr esi;
    FlatPtr ebp;
    FlatPtr esp;
    FlatPtr ebx;
    FlatPtr edx;
    FlatPtr ecx;
    FlatPtr eax;
    u16 exception_code;
    u16 isr_number;
    FlatPtr eip;
    FlatPtr cs;
    FlatPtr eflags;
    FlatPtr userspace_esp;
    FlatPtr userspace_ss;

    FlatPtr userspace_sp() const
    {
        return userspace_esp;
    }
    void set_userspace_sp(FlatPtr value) { userspace_esp = value; }
    FlatPtr ip() const { return eip; }
    void set_ip(FlatPtr value) { eip = value; }
    void set_dx(FlatPtr value) { edx = value; }
    FlatPtr bp() const { return ebp; }
    void set_bp(FlatPtr value) { ebp = value; }
    FlatPtr flags() const { return eflags; }
    void set_flags(FlatPtr value) { eflags = value; }
    void set_return_reg(FlatPtr value) { eax = value; }

    void capture_syscall_params(FlatPtr& function, FlatPtr& arg1, FlatPtr& arg2, FlatPtr& arg3, FlatPtr& arg4) const
    {
        function = eax;
        arg1 = edx;
        arg2 = ecx;
        arg3 = ebx;
        arg4 = esi;
    }

    ExecutionMode previous_mode() const
    {
        return ((cs & 3) != 0) ? ExecutionMode::User : ExecutionMode::Kernel;
    }
};

#define REGISTER_STATE_SIZE (19 * 4)
static_assert(AssertSize<RegisterState, REGISTER_STATE_SIZE>());

inline void copy_kernel_registers_into_ptrace_registers(PtraceRegisters& ptrace_regs, RegisterState const& kernel_regs)
{
    ptrace_regs.eax = kernel_regs.eax;
    ptrace_regs.ecx = kernel_regs.ecx;
    ptrace_regs.edx = kernel_regs.edx;
    ptrace_regs.ebx = kernel_regs.ebx;
    ptrace_regs.esp = kernel_regs.userspace_esp;
    ptrace_regs.ebp = kernel_regs.ebp;
    ptrace_regs.esi = kernel_regs.esi;
    ptrace_regs.edi = kernel_regs.edi;
    ptrace_regs.eip = kernel_regs.eip;
    ptrace_regs.eflags = kernel_regs.eflags;
    ptrace_regs.cs = 0;
    ptrace_regs.ss = 0;
    ptrace_regs.ds = 0;
    ptrace_regs.es = 0;
    ptrace_regs.fs = 0;
    ptrace_regs.gs = 0;
}

inline void copy_ptrace_registers_into_kernel_registers(RegisterState& kernel_regs, PtraceRegisters const& ptrace_regs)
{
    kernel_regs.eax = ptrace_regs.eax;
    kernel_regs.ecx = ptrace_regs.ecx;
    kernel_regs.edx = ptrace_regs.edx;
    kernel_regs.ebx = ptrace_regs.ebx;
    kernel_regs.esp = ptrace_regs.esp;
    kernel_regs.ebp = ptrace_regs.ebp;
    kernel_regs.esi = ptrace_regs.esi;
    kernel_regs.edi = ptrace_regs.edi;
    kernel_regs.eip = ptrace_regs.eip;
    kernel_regs.eflags = (kernel_regs.eflags & ~safe_eflags_mask) | (ptrace_regs.eflags & safe_eflags_mask);
}

struct [[gnu::packed]] DebugRegisterState {
    FlatPtr dr0;
    FlatPtr dr1;
    FlatPtr dr2;
    FlatPtr dr3;
    FlatPtr dr6;
    FlatPtr dr7;
};

inline void read_debug_registers_into(DebugRegisterState& state)
{
    state.dr0 = read_dr0();
    state.dr1 = read_dr1();
    state.dr2 = read_dr2();
    state.dr3 = read_dr3();
    state.dr6 = read_dr6();
    state.dr7 = read_dr7();
}

inline void write_debug_registers_from(DebugRegisterState const& state)
{
    write_dr0(state.dr0);
    write_dr1(state.dr1);
    write_dr2(state.dr2);
    write_dr3(state.dr3);
    write_dr6(state.dr6);
    write_dr7(state.dr7);
}

inline void clear_debug_registers()
{
    write_dr0(0);
    write_dr1(0);
    write_dr2(0);
    write_dr3(0);
    write_dr7(1 << 10); // Bit 10 is reserved and must be set to 1.
}

}
