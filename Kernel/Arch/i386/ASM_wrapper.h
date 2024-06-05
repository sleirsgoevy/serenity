#pragma once
#include "../x86_64/ASM_wrapper.h"

ALWAYS_INLINE void set_fs(u16 segment)
{
    asm volatile(
        "mov %%ax, %%fs" ::"a"(segment)
        : "memory");
}

ALWAYS_INLINE void set_gs(u16 segment)
{
    asm volatile(
        "mov %%ax, %%gs" ::"a"(segment)
        : "memory");
}

ALWAYS_INLINE u16 get_fs()
{
    u16 fs;
    asm("mov %%fs, %%eax"
        : "=a"(fs));
    return fs;
}

ALWAYS_INLINE u16 get_gs()
{
    u16 gs;
    asm("mov %%gs, %%eax"
        : "=a"(gs));
    return gs;
}
