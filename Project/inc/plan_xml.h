#ifndef PLAN_XML_H
#define PLAN_XML_H

#include "plan_include.h"

typedef struct
{
  uint8_t *str;
  uint8_t len;
} string;


void parse_xml(char *str, uint16_t len);
void get_string(char *buff, string *str, char pos);



#endif