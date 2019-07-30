/*
 * Copyright (c) 2013, Institute for Pervasive Computing, ETH Zurich
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
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 */

/**
 * \file
 *      Example resource
 * \author
 *      Matthias Kovatsch <kovatsch@inf.ethz.ch>
 */

#include <stdlib.h>
#include <string.h>
#include "coap-engine.h"

/* Log configuration */
#include "coap-log.h"
#define LOG_MODULE "RES SYSTEM"
#define LOG_LEVEL  LOG_LEVEL_DBG

static void res_get_handler(coap_message_t *request, coap_message_t *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset);

/*
 * A handler function named [resource name]_handler must be implemented for each RESOURCE.
 * A buffer for the response payload is provided through the buffer pointer. Simple resources can ignore
 * preferred_size and offset, but must respect the REST_MAX_CHUNK_SIZE limit for the buffer.
 * If a smaller block size is requested for CoAP, the REST framework automatically splits the data.
 */
RESOURCE(res_system,
         "title=\"SNMPv2-MIB/system: ?info=\"\"..\";rt=\"Text\"",
         res_get_handler,
         NULL,
         NULL,
         NULL);

static char *sysDescr = "\"sysDescr\":\""CONTIKI_VERSION_STRING "\"";
static char *sysObjectID = "\"sysObjectID\":\"1.3.6.1.4.1.54352\"";
/* Max length of uint64_t + key */
static char sysUpTime[34];
static char *sysContact = "\"sysContact\":\"Contiki-NG, https://github.com/contiki-ng/contiki-ng\"";
static char *sysName = "\"sysName\":\"Contiki-NG - "CONTIKI_TARGET_STRING "\"";
static char *sysLocation = "\"sysLocation\":\"\"";
/* Max length of uint64_t + key */
static char sysServices[36];
static char *sysRes = "\"system\":\"sysDescr,sysObjectID,sysUpTime,sysContact,sysName,sysLocation,sysServices\"";

static char *
_res_get_handler(coap_message_t *request, coap_message_t *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset, const char *info, size_t len)
{

  if(len == strlen("sysDescr") && !strncmp(info, "sysDescr", len)) {
    return sysDescr;
  } else if(len == strlen("sysObjectID") && !strncmp(info, "sysObjectID", len)) {
    return sysObjectID;
  } else if(len == strlen("sysUpTime") && !strncmp(info, "sysUpTime", len)) {
    sprintf(sysUpTime, "\"sysUpTime\":%lu", clock_seconds() * 100);
    return sysUpTime;
  } else if(len == strlen("sysContact") && !strncmp(info, "sysContact", len)) {
    return sysContact;
  } else if(len == strlen("sysName") && !strncmp(info, "sysName", len)) {
    return sysName;
  } else if(len == strlen("sysLocation") && !strncmp(info, "sysLocation", len)) {
    return sysLocation;
  } else if(len == strlen("sysServices") && !strncmp(info, "sysServices", len)) {
    sprintf(sysServices, "\"sysServices\":%lu", clock_seconds() * 100);
    return sysServices;
  }

  return sysRes;
}
static void
_init_response(uint8_t *buffer, int *length)
{
  *length = 0;
  sprintf((char *)buffer + *length, "{");
  (*length)++;
}
static void
_add_response(uint8_t *buffer, char *buf, int *length)
{
  int str_len;

  str_len = strlen(buf);
  if((*length) + str_len < REST_MAX_CHUNK_SIZE) {
    if((*length) > 1) {
      sprintf((char *)buffer + *length, ",");
      (*length)++;
    }
    sprintf((char *)buffer + *length, "%s", buf);
    (*length) += str_len;
  }
}
static void
_end_response(uint8_t *buffer, int *length)
{
  sprintf((char *)buffer + *length, "}");
  (*length)++;
}
static void
res_get_handler(coap_message_t *request, coap_message_t *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset)
{
  const char *info = NULL;
  char *buf;
  int length, len;
  const char *current, *last;

  _init_response(buffer, &length);

  /* The query string can be retrieved by rest_get_query() or parsed for its key-value pairs. */
  len = coap_get_query_variable(request, "info", &info);
  if(len > 0) {
    last = current = info;
    while(len > 0) {
      /* Found */
      if(*current == ',') {
        buf = _res_get_handler(request, response, buffer, preferred_size, offset, last, current - last);
        _add_response(buffer, buf, &length);
        current++;
        last = current;
      } else {
        current++;
      }

      len--;
    }
    buf = _res_get_handler(request, response, buffer, preferred_size, offset, last, current - last);
    _add_response(buffer, buf, &length);
  } else {
    buf = _res_get_handler(request, response, buffer, preferred_size, offset, "", 1);
    _add_response(buffer, buf, &length);
  }

  _end_response(buffer, &length);
  coap_set_header_content_format(response, APPLICATION_JSON);
  coap_set_header_etag(response, (uint8_t *)&length, 1);
  coap_set_payload(response, buffer, length);
}
