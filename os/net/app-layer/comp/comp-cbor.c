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

#include "contiki.h"
/* #include "comp.h" */
#include "comp-cbor.h"

#include <string.h>

uint8_t *
cbor_read_token(uint8_t *data, struct cbor_token *token)
{
  uint8_t type = *data;
  data++;
  uint8_t length = 0;

  if(type >> 5 == CBOR_TOKEN_TYPE_UNSIGNED
     || type >> 5 == CBOR_TOKEN_TYPE_TEXT
     || type >> 5 == CBOR_TOKEN_TYPE_ARRAY
     || type >> 5 == CBOR_TOKEN_TYPE_TAG) {
    if((type & 31) < 24) {
      token->integer = type & 31;
    } else if((type & 31) == 24) {
      length = 1;
    } else if((type & 31) == 25) {
      length = 2;
    } else if((type & 31) == 26) {
      length = 4;
    } else if((type & 31) == 27) {
      length = 8;
    } else {
      return NULL;
    }
  } else {
    return NULL;
  }

  if(length > 0) {
    token->integer = 0x00;
    switch(length) {
    case 8:
      token->integer |= ((uint64_t)*data) << 56;
      data++;
      token->integer |= ((uint64_t)*data) << 48;
      data++;
      token->integer |= ((uint64_t)*data) << 40;
      data++;
      token->integer |= ((uint64_t)*data) << 32;
      data++;
    case 4:
      token->integer |= *data << 24;
      data++;
      token->integer |= *data << 16;
      data++;
    case 2:
      token->integer |= *data << 8;
      data++;
    case 1:
      token->integer |= *data;
      data++;
    }
  }

  if(type >> 5 == CBOR_TOKEN_TYPE_TEXT) {
    token->string = (char *)(data);
    return data + token->integer;
  }

  return data;
}
uint8_t *
cbor_write_type_size_bytes(uint8_t *data, uint32_t *size, uint32_t type, const char *bytes, uint64_t type_size)
{
  type <<= 5;
  if(type_size < 24) {
    *data++ = (uint8_t)(type | type_size);
    (*size)++;
  } else if(type_size < 256) {
    *data++ = (uint8_t)(type | 24);
    (*size)++;
    *data++ = (uint8_t)type_size;
    (*size)++;
  } else if(type_size < 65536) {
    *data++ = (uint8_t)(type | 25);
    (*size)++;
    *data++ = (uint8_t)(type_size >> 8);
    (*size)++;
    *data++ = (uint8_t)type_size;
    (*size)++;
  } else if(type_size < 4294967296) {
    *data++ = (uint8_t)(type | 26);
    (*size)++;
    *data++ = (uint8_t)(type_size >> 24);
    (*size)++;
    *data++ = (uint8_t)(type_size >> 16);
    (*size)++;
    *data++ = (uint8_t)(type_size >> 8);
    (*size)++;
    *data++ = (uint8_t)type_size;
    (*size)++;
  } else {
    *data++ = (uint8_t)(type | 27);
    (*size)++;
    *data++ = (uint8_t)(type_size >> 56);
    (*size)++;
    *data++ = (uint8_t)(type_size >> 48);
    (*size)++;
    *data++ = (uint8_t)(type_size >> 40);
    (*size)++;
    *data++ = (uint8_t)(type_size >> 32);
    (*size)++;
    *data++ = (uint8_t)(type_size >> 24);
    (*size)++;
    *data++ = (uint8_t)(type_size >> 16);
    (*size)++;
    *data++ = (uint8_t)(type_size >> 8);
    (*size)++;
    *data++ = (uint8_t)type_size;
    (*size)++;
  }

  if(bytes) {
    memcpy(data, bytes, type_size);
    (*size) += type_size;
    return data + type_size;
  } else {
    return data;
  }
}
