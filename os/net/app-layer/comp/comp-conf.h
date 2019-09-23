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
 *      COMP Configurable Macros
 * \author
 *      Yago Fontoura do Rosario <yago.rosario@hotmail.com.br
 */

/**
 * \addtogroup comp
 * @{
 */

#ifndef COMP_CONF_H_
#define COMP_CONF_H_

/**
 * \defgroup COMPConfs COMP Configurable Defines
 * @{
 */

#ifdef COMP_CONF_MSG_OID_MAX_LEN
/**
 * \brief Configurable maximum number of IDs in one OID
 */
#define COMP_MSG_OID_MAX_LEN COMP_CONF_MSG_OID_MAX_LEN
#else
/**
 * \brief Default maximum number of IDs in one OID
 */
#define COMP_MSG_OID_MAX_LEN 16
#endif

#ifdef COMP_CONF_MAX_NR_VALUES
/**
 * \brief Configurable maximum number of OIDs in one response
 */
#define COMP_MAX_NR_VALUES COMP_CONF_MAX_NR_VALUES
#else
/**
 * \brief Default maximum number of OIDs in one response
 */
#define COMP_MAX_NR_VALUES 2
#endif

#ifdef COMP_CONF_MAX_PACKET_SIZE
/**
 * \brief Configurable maximum size of the packet in bytes
 */
#define COMP_MAX_PACKET_SIZE COMP_CONF_MAX_PACKET_SIZE
#else
/**
 * \brief Default maximum size of the packet in bytes
 */
#define COMP_MAX_PACKET_SIZE 512
#endif

#ifdef COMP_CONF_PORT
/**
 * \brief Configurable COMP port
 */
#define COMP_PORT COMP_CONF_PORT
#else
/**
 * \brief Default COMP port
 */
#define COMP_PORT 20001
#endif

/*@}*/

#endif /* COMP_CONF_H_ */
/** @} */
