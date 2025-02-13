#ifndef COSMOPOLITAN_DSP_CORE_TWIXT8_H_
#define COSMOPOLITAN_DSP_CORE_TWIXT8_H_
#if !(__ASSEMBLER__ + __LINKER__ + 0)
COSMOPOLITAN_C_START_

/**
 * 8-bit linear interpolation kernel.
 */
__funline unsigned char twixt8(unsigned char al, unsigned char bl,
                               unsigned char p) {
  short bx;
  bx = bl;
  bx -= al;
  bx *= p;
  bx >>= 8;
  bx += al;
  al = bx;
  return al;
}

COSMOPOLITAN_C_END_
#endif /* !(__ASSEMBLER__ + __LINKER__ + 0) */
#endif /* COSMOPOLITAN_DSP_CORE_TWIXT8_H_ */
