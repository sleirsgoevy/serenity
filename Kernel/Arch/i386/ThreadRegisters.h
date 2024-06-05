/*
 * Copyright (c) 2021, Gunnar Beutner <gbeutner@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Platform.h>
#include <AK/Types.h>
#include <Kernel/Memory/AddressSpace.h>

namespace Kernel {

struct ThreadRegisters {
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
    FlatPtr eip;
    FlatPtr esp0;
    FlatPtr ss0;
    FlatPtr cs;

    FlatPtr eflags;
    FlatPtr flags() const { return eflags; }
    void set_flags(FlatPtr value) { eflags = value; }
    void set_sp(FlatPtr value) { esp = value; }
    void set_sp0(FlatPtr value) { esp0 = value; }
    void set_ip(FlatPtr value) { eip = value; }

    FlatPtr cr3;

    FlatPtr ip() const
    {
        return eip;
    }

    FlatPtr sp() const
    {
        return esp;
    }

    FlatPtr frame_pointer() const
    {
        return ebp;
    }

    void set_initial_state(bool is_kernel_process, Memory::AddressSpace& space, FlatPtr kernel_stack_top)
    {
        // Only IF is set when a process boots.
        set_flags(0x0202);

        if (is_kernel_process)
            cs = GDT_SELECTOR_CODE0;
        else
            cs = GDT_SELECTOR_CODE3 | 3;

        cr3 = space.page_directory().cr3();

        if (is_kernel_process) {
            set_sp(kernel_stack_top);
            set_sp0(kernel_stack_top);
        } else {
            // Ring 3 processes get a separate stack for ring 0.
            // The ring 3 stack will be assigned by exec().
            set_sp0(kernel_stack_top);
        }
    }

    void set_entry_function(FlatPtr entry_ip, FlatPtr entry_data)
    {
        set_ip(entry_ip);
        (void)entry_data;
        TODO();
        //rdi = entry_data; // entry function argument is expected to be in regs.rdi
    }

    void set_exec_state(FlatPtr entry_ip, FlatPtr userspace_sp, Memory::AddressSpace& space)
    {
        cs = GDT_SELECTOR_CODE3 | 3;
        (void)entry_ip;
        (void)userspace_sp;
        (void)space;
        TODO();
        //rip = entry_ip;
        //rsp = userspace_sp;
        //cr3 = space.page_directory().cr3();
    }
};

}
