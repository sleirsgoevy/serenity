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
    NONE = 10000,
    ABSOLUTE,
    COPY,
    GLOB_DAT,
    JUMP_SLOT,
    RELATIVE,
    TLS_DTPMOD,
    TLS_DTPREL,
    TLS_TPREL,
    TLSDESC,
    IRELATIVE,
};

}
