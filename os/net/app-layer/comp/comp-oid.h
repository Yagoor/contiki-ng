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

/**
 * \addtogroup comp
 * @{
 */

#ifndef COMP_OID_H_
#define COMP_OID_H_

#include "comp.h"
#include "cbor.h"

/**
 * @brief Compares to oids
 *
 * @param oid1 First Oid
 * @param oid2 Second Oid
 *
 * @return < 0 if oid1 < oid2, > 0 if oid1 > oid2 and 0 if they are equal
 */
int
comp_oid_cmp_oid(uint32_t *oid1, uint32_t *oid2);

/**
 * @brief Copies a Oid
 *
 * @param dst A pointer to the destination array
 * @param src A pointer to the source array
 */
void
comp_oid_copy(uint32_t *dst, uint32_t *src);

/* #if LOG_LEVEL == LOG_LEVEL_DBG */
/**
 * @brief Prints a oid
 *
 * @param oid A oid
 */
void
comp_oid_print(uint32_t *oid);
/* #endif / * LOG_LEVEL == LOG_LEVEL_DBG * / */

int
comp_oid_decode(CborValue *it, uint32_t *dst);

#endif /* COMP_OID_H_ */
/** @} */
