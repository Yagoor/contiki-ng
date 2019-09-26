/*
 * Copyright (C) 2019 Yago Fontoura do Rosario <yago.rosario@hotmail.com.br>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*---------------------------------------------------------------------------*/

/**
 * \file
 *      An implementation of the Simple Network Management Protocol (RFC 3411-3418)
 * \author
 *      Yago Fontoura do Rosario <yago.rosario@hotmail.com.br
 */

#include "contiki.h"

#include "comp.h"
#include "comp-oid.h"

#include "sys/log.h"
#define LOG_MODULE "COMP [oid]"
#define LOG_LEVEL LOG_LEVEL_COMP

/*---------------------------------------------------------------------------*/

int
comp_oid_cmp_oid(uint32_t *oid1, uint32_t *oid2)
{
  uint8_t i;

  i = 0;
  while(oid1[i] != ((uint32_t)-1) &&
        oid2[i] != ((uint32_t)-1)) {
    if(oid1[i] != oid2[i]) {
      if(oid1[i] < oid2[i]) {
        return -1;
      }
      return 1;
    }
    i++;
  }

  if(oid1[i] == ((uint32_t)-1) &&
     oid2[i] != ((uint32_t)-1)) {
    return -1;
  }

  if(oid1[i] != ((uint32_t)-1) &&
     oid2[i] == ((uint32_t)-1)) {
    return 1;
  }

  return 0;
}
/*---------------------------------------------------------------------------*/

int
comp_oid_decode(CborValue *it, uint32_t *dst)
{
  int i;
  uint64_t result;

  i = 0;
  while(!cbor_value_at_end(it)) {
    if(!cbor_value_is_unsigned_integer(it) ||
       cbor_value_get_uint64(it, &result) != CborNoError) {
      return 0;
    }
    if(dst) {
      dst[i++] = result;
    }

    if(cbor_value_advance_fixed(it) != CborNoError) {
      return 0;
    }
  }

  if(dst) {
    dst[i++] = -1;
  }

  return 1;
}
/*---------------------------------------------------------------------------*/

void
comp_oid_copy(uint32_t *dst, uint32_t *src)
{
  uint8_t i;

  i = 0;
  while(src[i] != ((uint32_t)-1)) {
    dst[i] = src[i];
    i++;
  }
  /*
   * Copy the "null" terminator
   */
  dst[i] = src[i];
}
/*---------------------------------------------------------------------------*/

#if LOG_LEVEL == LOG_LEVEL_DBG
void
comp_oid_print(uint32_t *oid)
{
  uint8_t i;

  i = 0;
  LOG_DBG("{");
  while(oid[i] != ((uint32_t)-1)) {
    LOG_DBG_("%lu", (unsigned long)oid[i]);
    i++;
    if(oid[i] != ((uint32_t)-1)) {
      LOG_DBG_(".");
    }
  }
  LOG_DBG_("}\n");
}
#endif /* LOG_LEVEL == LOG_LEVEL_DBG */
