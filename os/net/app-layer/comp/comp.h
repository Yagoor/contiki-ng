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
#define COMP_VERSION_1  0
/**
 * @brief COMP Version 2c code
 */
#define COMP_VERSION_2C 1

/** @}*/

/**
 * \defgroup CoMPStructs CoMP Structs
 * @{
 */

/**
 * @brief
 */
#define COMP_HEADER_VERSION(v) v >> 4

/**
 * @brief
 */
#define COMP_HEADER_OPERATION(o) o & 1111

/**
 * @brief
 */
#define COMP_EXTRA_HEADER_ERROR_CODE(ec) ec >> 3

/**
 * @brief
 */
#define COMP_EXTRA_HEADER_ERROR_INDEX(ei) ei & 3

/**
 * @brief
 */
#define COMP_EXTRA_HEADER_NON_REPEATERS(nr) nr >> 4

/**
 * @brief
 */
#define COMP_EXTRA_HEADER_MAX_REPETITIONS(mr) mr & 4

/**
 * @brief
 */
typedef struct comp_header_s {
  /**
   * @brief
   */
  uint8_t version_and_operation;
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
      uint8_t error_code_and_error_index;
    } compv1_v2_extra_header;
    /**
     * @brief
     */
    struct compv2_getbulk_extra_header_s {
      /**
       * @brief
       */
      uint8_t non_repeaters_and_max_repetitions;
    } compv2_getbulk_extra_header;
  } comp_extra_headers;
} comp_header_t;

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
