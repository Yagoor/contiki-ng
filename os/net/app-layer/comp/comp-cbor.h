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

#ifndef COMP_CBOR_H
#define COMP_CBOR_H

#include <stdlib.h>
#include <stdint.h>

#define CBOR_TOKEN_TYPE_UNSIGNED 0
#define CBOR_TOKEN_TYPE_NEGATIVE 1
#define CBOR_TOKEN_TYPE_BYTES 2
#define CBOR_TOKEN_TYPE_TEXT 3
#define CBOR_TOKEN_TYPE_ARRAY 4
#define CBOR_TOKEN_TYPE_MAP 5
#define CBOR_TOKEN_TYPE_TAG 6
#define CBOR_TOKEN_TYPE_PRIMITIVE 7

struct cbor_token {
  int8_t type;
  uint64_t integer;
  char *string;
};

uint8_t *
cbor_read_token(uint8_t *data, struct cbor_token *token);
#define cbor_write_type_size(data, size, type, type_size) cbor_write_type_size_bytes(data, size, type, NULL, type_size);
uint8_t *
cbor_write_type_size_bytes(uint8_t *data, uint32_t *size, uint32_t type, const char *bytes, uint64_t type_size);

#endif /* COMP_CBOR_H */
