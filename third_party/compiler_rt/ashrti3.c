/* clang-format off */
/* ===-- ashrti3.c - Implement __ashrti3 -----------------------------------===
 *
 *                     The LLVM Compiler Infrastructure
 *
 * This file is dual licensed under the MIT and the University of Illinois Open
 * Source Licenses. See LICENSE.TXT for details.
 *
 * ===----------------------------------------------------------------------===
 *
 * This file implements __ashrti3 for the compiler_rt library.
 *
 * ===----------------------------------------------------------------------===
 */

__static_yoink("huge_compiler_rt_license");

#include "third_party/compiler_rt/int_lib.h"

#ifdef CRT_HAS_128BIT

/* Returns: arithmetic a >> b */

/* Precondition:  0 <= b < bits_in_tword */

COMPILER_RT_ABI ti_int
__ashrti3(ti_int a, si_int b)
{
    const int bits_in_dword = (int)(sizeof(di_int) * CHAR_BIT);
    twords input;
    twords result;
    input.all = a;
    if (b & bits_in_dword)  /* bits_in_dword <= b < bits_in_tword */
    {
        /* result.s.high = input.s.high < 0 ? -1 : 0 */
        result.s.high = input.s.high >> (bits_in_dword - 1);
        result.s.low = input.s.high >> (b - bits_in_dword);
    }
    else  /* 0 <= b < bits_in_dword */
    {
        if (b == 0)
            return a;
        result.s.high  = input.s.high >> b;
        result.s.low = (input.s.high << (bits_in_dword - b)) | (input.s.low >> b);
    }
    return result.all;
}

#endif /* CRT_HAS_128BIT */
