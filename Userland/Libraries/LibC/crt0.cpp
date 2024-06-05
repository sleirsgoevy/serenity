/*
 * Copyright (c) 2018-2020, Andreas Kling <kling@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Types.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/internals.h>
#include <unistd.h>

#ifndef _DYNAMIC_LOADER
extern "C" {

int main(int, char**, char**);

// Tell the compiler that this may be called from somewhere else.
int _entry(int argc, char** argv) __attribute__((used));
void _start(int, char**, char**) __attribute__((used));

NAKED void _start(int, char**, char**)
{
#    if ARCH(AARCH64)
    asm(
        "mov x29, 0\n"
        "mov x30, 0\n"
        "bl _entry\n");
#    elif ARCH(RISCV64)
    asm(
        "li fp, 0\n"
        "li ra, 0\n"
        "tail _entry@plt\n");
#    elif ARCH(X86_64) || ARCH(I386)
    asm(
        "push $0\n"
        "jmp _entry@plt\n");
#    else
#        error "Unknown architecture"
#    endif
}

int _entry(int argc, char** argv)
{
    __begin_atexit_locking();

    int status = main(argc, argv, environ);

    exit(status);

    return 20150614;
}
}
#endif

#if ARCH(I386)
//__stack_chk_fail_local is forced by GCC to be a hidden symbol
//this means that it must be present in every library, and not just included in libc.so

extern "C" [[gnu::noreturn, gnu::no_stack_protector]] void __stack_chk_fail();

extern "C" [[gnu::noreturn, gnu::no_stack_protector]] void __stack_chk_fail_local()
{
    __stack_chk_fail();
}
#endif
