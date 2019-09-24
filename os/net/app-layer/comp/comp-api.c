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
 *      The public API for the Contiki-NG COMP implementation
 * \author
 *      Yago Fontoura do Rosario <yago.rosario@hotmail.com.br
 */

#include "contiki.h"

#include "comp-api.h"

#include <string.h>

static void
comp_api_replace_oid(comp_pdu_data_t *pdu_data, uint32_t *oid)
{
  uint8_t i;

  i = 0;
  while(oid[i] != ((uint32_t)-1)) {
    pdu_data->oid[i] = oid[i];
    i++;
  }
  pdu_data->oid[i] = ((uint32_t)-1);
}
/*---------------------------------------------------------------------------*/
void
comp_api_set_string(comp_pdu_data_t *pdu_data, uint32_t *oid, char *string)
{

  comp_api_replace_oid(pdu_data, oid);
  pdu_data->value_type = COMP_TYPE_STRING;
  pdu_data->value.string.string = string;
  pdu_data->value.string.length = strlen(string);
}
/*---------------------------------------------------------------------------*/
void
comp_api_set_time_ticks(comp_pdu_data_t *pdu_data, uint32_t *oid, uint32_t integer)
{

  comp_api_replace_oid(pdu_data, oid);
  pdu_data->value_type = COMP_TYPE_INTEGER;
  pdu_data->value.integer = integer;
}
/*---------------------------------------------------------------------------*/
void
comp_api_set_oid(comp_pdu_data_t *pdu_data, uint32_t *oid, uint32_t *ret_oid)
{

  comp_api_replace_oid(pdu_data, oid);
  pdu_data->value_type = COMP_TYPE_OID;
  pdu_data->value.oid = ret_oid;
}
/*---------------------------------------------------------------------------*/
void
comp_api_add_resource(comp_mib_resource_t *new_resource)
{
  return comp_mib_add(new_resource);
}
