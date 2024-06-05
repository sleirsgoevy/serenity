/*
 * Copyright (c) 2020, the SerenityOS developers.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Types.h>
#include <assert.h>
#include <stdio.h>
#include <sys/internals.h>
#include <unistd.h>

#ifndef _DYNAMIC_LOADER
void* __dso_handle __attribute__((__weak__));
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
