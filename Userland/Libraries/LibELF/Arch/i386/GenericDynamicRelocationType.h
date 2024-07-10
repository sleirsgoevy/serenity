/*
 * Copyright (c) 2024, Sönke Holz <sholz8530@gmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <Userland/Libraries/LibELF/ELFABI.h>

#include <AK/Platform.h>
VALIDATE_IS_X86()

namespace ELF {

enum class GenericDynamicRelocationType : unsigned {
    RELATIVE = R_386_RELATIVE,
    ABSOLUTE = R_386_32,
    GLOB_DAT = R_386_GLOB_DAT,
    JUMP_SLOT = R_386_JMP_SLOT,
    TLS_DTPMOD = R_386_TLS_DTPMOD32,
    TLS_DTPREL = R_386_TLS_DTPOFF32,
    IRELATIVE = R_386_IRELATIVE,
    NONE = 10000,
    COPY,
    TLS_TPREL,
    TLSDESC,
};

}
