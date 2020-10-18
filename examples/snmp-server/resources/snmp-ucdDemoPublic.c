/*
 * Copyright (C) 2020 Yago Fontoura do Rosario <yago.rosario@hotmail.com.br>
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

#include "snmp-api.h"

/*---------------------------------------------------------------------------*/
#define UCDDEMOPUBLICSTRINGSTR      "BeforeSet"
#define UCDDEMOPUBLICSTRINGSTRMAX   64
static char ucdDemoPublicStringStr[UCDDEMOPUBLICSTRINGSTRMAX];
static uint32_t ucdDemoPublicStringLength;
/*---------------------------------------------------------------------------*/
static void
ucdDemoPublicString_read_handler(snmp_varbind_t *varbind, snmp_oid_t *oid);
static int
ucdDemoPublicString_write_handler(snmp_varbind_t *varbind);

MIB_READ_WRITE_RESOURCE(ucdDemoPublicString, ucdDemoPublicString_read_handler, ucdDemoPublicString_write_handler, 1, 3, 6, 1, 4, 1, 2021, 14, 1, 1, 2, 0);

static void
ucdDemoPublicString_read_handler(snmp_varbind_t *varbind, snmp_oid_t *oid)
{
  snmp_api_set_string(varbind, oid, ucdDemoPublicStringStr, ucdDemoPublicStringLength);
}
static int
ucdDemoPublicString_write_handler(snmp_varbind_t *varbind)
{
  const char *string;
  uint32_t string_length;

  snmp_api_get_string(varbind, &string, &string_length);

  if(string_length > UCDDEMOPUBLICSTRINGSTRMAX) {
    return 0;
  }

  memcpy(ucdDemoPublicStringStr, string, string_length);
  ucdDemoPublicStringLength = string_length;

  return 1;
}
/*---------------------------------------------------------------------------*/
void
ucdDemoPublic_init()
{
  memset(ucdDemoPublicStringStr, 0, UCDDEMOPUBLICSTRINGSTRMAX);
  strcpy(ucdDemoPublicStringStr, UCDDEMOPUBLICSTRINGSTR);
  ucdDemoPublicStringLength = strlen(UCDDEMOPUBLICSTRINGSTR);

  snmp_api_add_resource(&ucdDemoPublicString);
}
/*---------------------------------------------------------------------------*/