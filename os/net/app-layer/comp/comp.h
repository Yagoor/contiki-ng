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
 * \addtogroup apps
 * @{
 *
 * \defgroup comp CoMP (Constrained Monitoring Protocol)
 * @{
 *
 * This is an implementation of the Constrained Monitoring Protocol
 */

#ifndef COMP_H_
#define COMP_H_

#include "contiki.h"

#include "comp-conf.h"

/**
 * \defgroup CoMPDefine CoMP Defines
 * @{
 */

/**
 * @brief COMP Version 1 code
 */
#define COMP_VERSION_1 0

/**
 * @brief COMP Version 2c code
 */
#define COMP_VERSION_2C 1

/**
 * @brief
 */
#define COMP_PDU_GET 0

/**
 * @brief
 */
#define COMP_PDU_GETNEXT 1

/**
 * @brief
 */
#define COMP_PDU_RESPONSE 2

/**
 * @brief
 */
#define COMP_PDU_SET 3

/**
 * @brief
 */
#define COMP_PDU_V1TRAP 4

/**
 * @brief
 */
#define COMP_PDU_GETBULK 5

/**
 * @brief
 */
#define COMP_PDU_INFORM 6

/**
 * @brief
 */
#define COMP_PDU_TRAP 7

/**
 * @brief
 */
#define COMP_PDU_REPORT 8

/**
 * @brief
 */
#define COMP_ERROR_NO_ERROR 0

/**
 * @brief
 */
#define COMP_ERROR_TOO_BIG 1

/**
 * @brief
 */
#define COMP_ERROR_NO_SUCH_NAME 2

/**
 * @brief
 */
#define COMP_ERROR_BAD_VALUE 3

/**
 * @brief
 */
#define COMP_ERROR_READ_ONLY 4

/**
 * @brief
 */
#define COMP_ERROR_GEN_ERR 5

/**
 * @brief
 */
#define COMP_ERROR_NO_ACCESS 6

/**
 * @brief
 */
#define COMP_ERROR_WRONG_TYPE 7

/**
 * @brief
 */
#define COMP_ERROR_WRONG_LENGTH 8

/**
 * @brief
 */
#define COMP_ERROR_WRONG_ENCODING 9

/**
 * @brief
 */
#define COMP_ERROR_WRONG_VALUE 10

/**
 * @brief
 */
#define COMP_ERROR_NO_CREATION 11

/**
 * @brief
 */
#define COMP_ERROR_INCONSISTENT_VALUE 12

/**
 * @brief
 */
#define COMP_ERROR_RESOURCE_UNAVAILABLE 13

/**
 * @brief
 */
#define COMP_ERROR_COMMIT_FAILED 14

/**
 * @brief
 */
#define COMP_ERROR_UNDO_FAILED 15

/**
 * @brief
 */
#define COMP_ERROR_AUTHORIZATION_ERROR 16

/**
 * @brief
 */
#define COMP_ERROR_NOT_WRITABLE 17

/**
 * @brief
 */
#define COMP_ERROR_INCONSISTENT_NAME 18

/**
 * @brief
 */
#define SNMP_NO_SUCH_OBJECT 0

/**
 * @brief
 */
#define SNMP_NO_SUCH_INSTANCE 1

/**
 * @brief
 */
#define SNMP_END_OF_MIB_VIEW 2

/**
 * @brief
 */
#define COMP_VERSION_MASK 0xF0

/**
 * @brief
 */
#define COMP_VERSION_POSITION 4

/**
 * @brief
 */
#define COMP_OPERATION_MASK 0x0F

/**
 * @brief
 */
#define COMP_OPERATION_POSITION 0

/**
 * @brief
 */
#define COMP_ERROR_CODE_MASK 0xF8

/**
 * @brief
 */
#define COMP_ERROR_CODE_POSITION 3

/**
 * @brief
 */
#define COMP_ERROR_INDEX_MASK 0x07

/**
 * @brief
 */
#define COMP_ERROR_INDEX_POSITION 0

/**
 * @brief
 */
#define COMP_NON_REPEATERS_MASK 0xF0

/**
 * @brief
 */
#define COMP_NON_REPEATERS_POSITION 4

/**
 * @brief
 */
#define COMP_MAX_REPETITIONS_MASK 0x0F

/**
 * @brief
 */
#define COMP_MAX_REPETITIONS_POSITION 0

/**
 * @brief
 */
#define COMP_TYPE_INTEGER 0

/**
 * @brief
 */
#define COMP_TYPE_STRING 1

/**
 * @brief
 */
#define COMP_TYPE_OID 2

/**
 * @brief
 */
#define SNMP_TYPE_NO_SUCH_OBJECT 3

/**
 * @brief
 */
#define COMP_TYPE_NO_SUCH_INSTANCE 4

/**
 * @brief
 */
#define SNMP_TYPE_END_OF_MIB_VIEW 5

/** @}*/

/**
 * \defgroup CoMPStructs CoMP Structs
 * @{
 */

/**
 * @brief
 */
typedef struct comp_pdu_data_s {
  /**
   * @brief
   */
  uint32_t oid[COMP_MSG_OID_MAX_LEN];
  /**
   * @brief
   */
  uint8_t value_type;
  /**
   * @brief
   */
  union comp_pdu_data_value_u {
    /**
     * @brief
     */
    uint32_t integer;
    /**
     * @brief
     */
    struct comp_pdu_data_value_string {
      /**
       * @brief
       */
      const char *string;
      /**
       * @brief
       */
      uint32_t length;
    } string;
    /**
     * @brief
     */
    uint32_t *oid;
  } value;
} comp_pdu_data_t;

/**
 * @brief
 */
typedef struct comp_pdu_s {
  /**
   * @brief
   */
  comp_pdu_data_t data[COMP_MAX_NR_VALUES];
  /**
   * @brief
   */
  uint8_t data_length;
} comp_pdu_t;

/**
 * @brief
 */
typedef struct comp_header_s {
  /**
   * @brief
   */
  uint8_t version;
  /**
   * @brief
   */
  uint8_t operation;
  /**
   * @brief
   */
  uint8_t request_id;
  /**
   * @brief
   */
  union comp_extra_headers_u {
    /**
     * @brief
     */
    struct compv1_v2_extra_header_s {
      /**
       * @brief
       */
      uint8_t error_code;
      /**
       * @brief
       */
      uint8_t error_index;
    } compv1_v2_extra_header;
    /**
     * @brief
     */
    struct compv2_getbulk_extra_header_s {
      /**
       * @brief
       */
      uint8_t non_repeaters;
      /**
       * @brief
       */
      uint8_t max_repetitions;
    } compv2_getbulk_extra_header;
  } comp_extra_headers;
} comp_header_t;

/**
 * @brief
 */
typedef struct comp_message_s {
  /**
   * @brief
   */
  comp_header_t header;
  /**
   * @brief
   */
  comp_pdu_t pdu;
} comp_message_t;

/** @}*/

/**
 * \defgroup COMPFunctions COMP Functions
 * @{
 */

/**
 * @brief Initializes the COMP engine
 */
void
comp_init();

/** @}*/

#endif /* COMP_H_ */
/** @} */
/** @} */
