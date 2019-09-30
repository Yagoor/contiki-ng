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
 *      The Contiki-NG COMP implementation engine
 * \author
 *      Yago Fontoura do Rosario <yago.rosario@hotmail.com.br
 */

#include "contiki.h"

#include "comp.h"
#include "comp-oid.h"
#include "comp-mib.h"
#include "comp-engine.h"

#include "sys/log.h"
#define LOG_MODULE "COMP [engine]"
#define LOG_LEVEL LOG_LEVEL_COMP

int
comp_engine_decompress_oids(comp_message_t *message)
{
  uint8_t i, j, k;
  uint32_t oid[COMP_MSG_OID_MAX_LEN];

  for(i = 0; i < message->pdu.data_length; i++) {
    /*
     * Zero is the base oid for compression and decompression
     */
    if(i == 0) {
      continue;
    }

    for(j = 0; message->pdu.data[i].oid[j] != -1; j++) {
      oid[j] = message->pdu.data[i].oid[j];
    }
    oid[j] = -1;

    for(j = 0, k = 0; oid[j] != -1; j++, k++) {
      if(j == 0) {
        for(; k < oid[j]; k++) {
          message->pdu.data[i].oid[k] = message->pdu.data[0].oid[k];
        }
        k--;
      } else {
        message->pdu.data[i].oid[k] = oid[j];
      }
    }

    message->pdu.data[i].oid[k] = -1;
  }

  return 1;
}
/*---------------------------------------------------------------------------*/
int
comp_engine_encode(comp_message_t *message, uint8_t *buffer, uint32_t *buffer_len, uint8_t with_oids)
{
  uint8_t i, j;

  *buffer_len = 0;

  *buffer = 0x00;
  *buffer |= COMP_VERSION_MASK & (message->header.version << COMP_VERSION_POSITION);
  *buffer |= COMP_OPERATION_MASK & (COMP_PDU_RESPONSE << COMP_OPERATION_POSITION);
  buffer++;
  (*buffer_len)++;

  *buffer = 0x00;
  *buffer |= message->header.request_id;
  buffer++;
  (*buffer_len)++;

  *buffer = 0x00;
  *buffer |= COMP_ERROR_CODE_MASK & (message->header.comp_extra_headers.compv1_v2_extra_header.error_code << COMP_ERROR_CODE_POSITION);
  *buffer |= COMP_ERROR_INDEX_MASK & (message->header.comp_extra_headers.compv1_v2_extra_header.error_index << COMP_ERROR_INDEX_POSITION);
  buffer++;
  (*buffer_len)++;

  buffer = cbor_write_type_size(buffer, buffer_len, CBOR_TOKEN_TYPE_ARRAY, message->pdu.data_length);
  for(i = 0; i < message->pdu.data_length; i++) {
    if(with_oids) {
      buffer = cbor_write_type_size(buffer, buffer_len, CBOR_TOKEN_TYPE_ARRAY, 2);

      for(j = 0; message->pdu.data[i].oid[j] != -1; j++) {
      }
      buffer = cbor_write_type_size(buffer, buffer_len, CBOR_TOKEN_TYPE_ARRAY, j);
      for(j = 0; message->pdu.data[i].oid[j] != -1; j++) {
        buffer = cbor_write_type_size(buffer, buffer_len, CBOR_TOKEN_TYPE_UNSIGNED, message->pdu.data[i].oid[j]);
      }
    }

    switch(message->pdu.data[i].value_type) {
    case COMP_TYPE_INTEGER:
      buffer = cbor_write_type_size(buffer, buffer_len, CBOR_TOKEN_TYPE_UNSIGNED, message->pdu.data[i].value.integer);
      break;
    case COMP_TYPE_STRING:
      buffer = cbor_write_type_size_bytes(buffer, buffer_len, CBOR_TOKEN_TYPE_TEXT, message->pdu.data[i].value.string.string, message->pdu.data[i].value.string.length);
      break;
    case COMP_TYPE_OID:
      for(j = 0; message->pdu.data[i].value.oid[j] != -1; j++) {
      }
      buffer = cbor_write_type_size(buffer, buffer_len, CBOR_TOKEN_TYPE_ARRAY, j);
      for(j = 0; message->pdu.data[i].value.oid[j] != -1; j++) {
        buffer = cbor_write_type_size(buffer, buffer_len, CBOR_TOKEN_TYPE_UNSIGNED, message->pdu.data[i].value.oid[j]);
      }
      break;
    case COMP_TYPE_NO_SUCH_INSTANCE:
      buffer = cbor_write_type_size(buffer, buffer_len, CBOR_TOKEN_TYPE_TAG, 280);
      buffer = cbor_write_type_size(buffer, buffer_len, CBOR_TOKEN_TYPE_UNSIGNED, SNMP_NO_SUCH_INSTANCE);
      break;
    case SNMP_TYPE_END_OF_MIB_VIEW:
      buffer = cbor_write_type_size(buffer, buffer_len, CBOR_TOKEN_TYPE_TAG, 280);
      buffer = cbor_write_type_size(buffer, buffer_len, CBOR_TOKEN_TYPE_UNSIGNED, SNMP_END_OF_MIB_VIEW);
      break;
    }
  }

  return 1;
}
/*---------------------------------------------------------------------------*/

int
comp_engine_decode_pdu(comp_message_t *message, uint8_t *buff)
{
  uint8_t i, j, size, oid_size;
  struct cbor_token token;

  buff = cbor_read_token(buff, &token);
  if(!buff) {
    return 0;
  }
  size = token.integer;

  for(i = 0; i < size; i++) {
    buff = cbor_read_token(buff, &token);
    if(!buff) {
      return 0;
    }

    oid_size = token.integer;
    for(j = 0; j < oid_size; j++) {

      buff = cbor_read_token(buff, &token);
      if(!buff) {
        return 0;
      }

      if(i < COMP_MAX_NR_VALUES) {
        message->pdu.data[i].oid[j] = token.integer;
      }
    }

    if(i < COMP_MAX_NR_VALUES) {
      message->pdu.data[i].oid[j] = -1;
    }
  }

  message->pdu.data_length = i < COMP_MAX_NR_VALUES ? i : COMP_MAX_NR_VALUES;

  return comp_engine_decompress_oids(message);
}
/*---------------------------------------------------------------------------*/

int
comp_engine_process(comp_message_t *message, uint8_t *buff, uint8_t *buffer, uint32_t *buffer_len, uint8_t with_oids)
{
  uint8_t i;
  comp_mib_resource_t *resource;

  LOG_DBG("comp_engine_decode_pdu\n");
  comp_engine_decode_pdu(message, buff);
  LOG_DBG("comp_engine_decode_pdu\n");

  for(i = 0; i < message->pdu.data_length; i++) {
    resource = with_oids ? comp_mib_find_next(message->pdu.data[i].oid) : comp_mib_find(message->pdu.data[i].oid);
    if(!resource) {
      switch(message->header.version) {
      case COMP_VERSION_1:
        message->header.comp_extra_headers.compv1_v2_extra_header.error_code = COMP_ERROR_NO_SUCH_NAME;
        message->header.comp_extra_headers.compv1_v2_extra_header.error_index = i + 1;
        break;
      case COMP_VERSION_2C:
        message->pdu.data[i].value_type = COMP_TYPE_NO_SUCH_INSTANCE;
        break;
      default:
        message->header.comp_extra_headers.compv1_v2_extra_header.error_code = COMP_ERROR_NO_SUCH_NAME;
        message->header.comp_extra_headers.compv1_v2_extra_header.error_index = i + 1;
        break;
      }
    } else {
      resource->handler(&(message->pdu.data[i]), resource->oid);
    }
  }

  return comp_engine_encode(message, buffer, buffer_len, with_oids);
}
/*---------------------------------------------------------------------------*/

int
comp_engine_process_bulk(comp_message_t *message, uint8_t *buff, uint8_t *buffer, uint32_t *buffer_len)
{
  comp_mib_resource_t *resource;
  uint8_t i, j, non_repeaters, max_repetitions, repeater, original_varbinds_length;
  uint32_t oid[COMP_MAX_NR_VALUES][COMP_MSG_OID_MAX_LEN];

  comp_engine_decode_pdu(message, buff);

  original_varbinds_length = message->pdu.data_length;
  non_repeaters = message->header.comp_extra_headers.compv2_getbulk_extra_header.non_repeaters;
  max_repetitions = message->header.comp_extra_headers.compv2_getbulk_extra_header.max_repetitions;

  for(i = 0; i < original_varbinds_length; i++) {
    comp_oid_copy(oid[i], message->pdu.data[i].oid);
  }

  message->pdu.data_length = 0;
  for(i = 0; i < original_varbinds_length; i++) {
    if(i >= non_repeaters) {
      break;
    }

    resource = comp_mib_find_next(oid[i]);
    if(!resource) {
      switch(message->header.version) {
      case COMP_VERSION_1:
        message->header.comp_extra_headers.compv1_v2_extra_header.error_code = COMP_ERROR_NO_SUCH_NAME;
        message->header.comp_extra_headers.compv1_v2_extra_header.error_index = i + 1;
        break;
      case COMP_VERSION_2C:
        message->pdu.data[i].value_type = SNMP_TYPE_END_OF_MIB_VIEW;
        break;
      default:
        message->header.comp_extra_headers.compv1_v2_extra_header.error_code = COMP_ERROR_NO_SUCH_NAME;
        message->header.comp_extra_headers.compv1_v2_extra_header.error_index = i + 1;
        break;
      }
    } else {
      if(message->pdu.data_length < COMP_MAX_NR_VALUES) {
        resource->handler(&(message->pdu.data[message->pdu.data_length]), resource->oid);
        message->pdu.data_length++;
      }
    }
  }

  for(i = 0; i < max_repetitions; i++) {
    repeater = 0;
    for(j = non_repeaters; j < original_varbinds_length; j++) {
      resource = comp_mib_find_next(oid[j]);
      if(!resource) {
        switch(message->header.version) {
        case COMP_VERSION_1:
          message->header.comp_extra_headers.compv1_v2_extra_header.error_code = COMP_ERROR_NO_SUCH_NAME;
          message->header.comp_extra_headers.compv1_v2_extra_header.error_index = message->pdu.data_length + 1;
          break;
        case COMP_VERSION_2C:
          if(message->pdu.data_length < COMP_MAX_NR_VALUES) {
            message->pdu.data[message->pdu.data_length].value_type = SNMP_TYPE_END_OF_MIB_VIEW;
            comp_oid_copy(message->pdu.data[message->pdu.data_length].oid, oid[j]);
            message->pdu.data_length++;
          }
          break;
        default:
          message->header.comp_extra_headers.compv1_v2_extra_header.error_code = COMP_ERROR_NO_SUCH_NAME;
          message->header.comp_extra_headers.compv1_v2_extra_header.error_index = i + 1;
          break;
        }
      } else {
        if(message->pdu.data_length < COMP_MAX_NR_VALUES) {
          resource->handler(&(message->pdu.data[message->pdu.data_length]), resource->oid);
          message->pdu.data_length++;
          comp_oid_copy(oid[j], resource->oid);
          repeater++;
        }
      }
    }
    if(repeater == 0) {
      break;
    }
  }

  return comp_engine_encode(message, buffer, buffer_len, 1);
}
/*---------------------------------------------------------------------------*/

int
comp_engine_get(comp_message_t *message, uint8_t *buff, uint8_t *buffer, uint32_t *buffer_len)
{
  return comp_engine_process(message, buff, buffer, buffer_len, 0);
}
/*---------------------------------------------------------------------------*/

int
comp_engine_get_next(comp_message_t *message, uint8_t *buff, uint8_t *buffer, uint32_t *buffer_len)
{
  return comp_engine_process(message, buff, buffer, buffer_len, 1);
}
/*---------------------------------------------------------------------------*/

int
comp_engine_get_bulk(comp_message_t *message, uint8_t *buff, uint8_t *buffer, uint32_t *buffer_len)
{
  return comp_engine_process_bulk(message, buff, buffer, buffer_len);
}
/*---------------------------------------------------------------------------*/

int
comp_engine(uint8_t *buff, uint32_t buff_len, uint8_t *buffer, uint32_t *buffer_len)
{
  static comp_message_t message;

  message.header.version = (COMP_VERSION_MASK & *buff) >> COMP_VERSION_POSITION;
  LOG_DBG("Version: %hu\n", message.header.version);

  message.header.operation = (COMP_OPERATION_MASK & *buff) >> COMP_OPERATION_POSITION;
  LOG_DBG("Operation: %hu\n", message.header.operation);

  buff++;
  buff_len--;

  message.header.request_id = *buff;
  LOG_DBG("Request ID: %hu\n", message.header.request_id);

  buff++;
  buff_len--;

  /*
   * Decode the rest of the reader before going into private engines
   * Return 0 in unhandled type
   */
  switch(message.header.operation) {
  case COMP_PDU_GET:
  case COMP_PDU_GETNEXT:
    message.header.comp_extra_headers.compv1_v2_extra_header.error_code = (COMP_ERROR_CODE_MASK & *buff) >> COMP_ERROR_CODE_POSITION;
    LOG_DBG("Error Code: %hu\n", message.header.comp_extra_headers.compv1_v2_extra_header.error_code);

    message.header.comp_extra_headers.compv1_v2_extra_header.error_index = (COMP_ERROR_INDEX_MASK & *buff) >> COMP_ERROR_INDEX_POSITION;
    LOG_DBG("Error Index: %hu\n", message.header.comp_extra_headers.compv1_v2_extra_header.error_index);

    buff++;
    buff_len--;

    break;
  case COMP_PDU_RESPONSE:
    LOG_INFO("Received response message... Returning\n");
    return 0;
  case COMP_PDU_SET:
    LOG_INFO("Received set message... Returning\n");
    return 0;
  case COMP_PDU_V1TRAP:
    return 0;
    LOG_INFO("Received v1 trap message... Returning\n");
  case COMP_PDU_GETBULK:
    message.header.comp_extra_headers.compv2_getbulk_extra_header.non_repeaters = (COMP_NON_REPEATERS_MASK & *buff) >> COMP_NON_REPEATERS_POSITION;
    LOG_DBG("Non Repeaters: %hu\n", message.header.comp_extra_headers.compv2_getbulk_extra_header.non_repeaters);

    message.header.comp_extra_headers.compv2_getbulk_extra_header.max_repetitions = (COMP_MAX_REPETITIONS_MASK & *buff) >> COMP_MAX_REPETITIONS_POSITION;
    LOG_DBG("Max Repetitions: %hu\n", message.header.comp_extra_headers.compv2_getbulk_extra_header.max_repetitions);

    buff++;
    buff_len--;

    break;
  case COMP_PDU_INFORM:
    LOG_INFO("Received inform message... Returning\n");
    return 0;
  case COMP_PDU_TRAP:
    LOG_INFO("Received trap message... Returning\n");
    return 0;
  case COMP_PDU_REPORT:
    LOG_INFO("Received report message... Returning\n");
    return 0;
  }

  switch(message.header.operation) {
  case COMP_PDU_GET:
    return comp_engine_get(&message, buff, buffer, buffer_len);
  case COMP_PDU_GETNEXT:
    return comp_engine_get_next(&message, buff, buffer, buffer_len);
  case COMP_PDU_GETBULK:
    return comp_engine_get_bulk(&message, buff, buffer, buffer_len);
  }

  return 1;
}
/*---------------------------------------------------------------------------*/

