/*********************************************************************
  Blosc - Blocked Shuffling and Compression Library

  Copyright (C) 2021  The Blosc Developers <blosc@blosc.org>
  https://blosc.org
  License: BSD 3-Clause (see LICENSE.txt)

  See LICENSE.txt for details about copyright and rights to use.
**********************************************************************/

/* AVX2-accelerated shuffle/unshuffle routines. */

#ifndef BITSHUFFLE_AVX2_H
#define BITSHUFFLE_AVX2_H

#include "blosc2-common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
  AVX2-accelerated bitshuffle routine.
*/
BLOSC_NO_EXPORT int64_t
    bshuf_trans_bit_elem_avx2(void* in, void* out, const size_t size,
                              const size_t elem_size, void* tmp_buf);

/**
  AVX2-accelerated bitunshuffle routine.
*/
BLOSC_NO_EXPORT int64_t
    bshuf_untrans_bit_elem_avx2(void* in, void* out, const size_t size,
                                const size_t elem_size, void* tmp_buf);

#ifdef __cplusplus
}
#endif

#endif /* BITSHUFFLE_AVX2_H */
