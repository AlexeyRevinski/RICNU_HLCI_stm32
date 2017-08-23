#include "plan_xml.h"

uint8_t curstring[50];
uint8_t len = 0;

void parse_xml(char *str, uint16_t len)
{
  uint8_t c;
  for(uint8_t i=0;i<len;i++)
  {
    c = str[i];
    switch(c)
    {
      // Special characters
    case '<':
      break;
    case '>':
      break;
    case '&':
      break;
    case '\'':
      break;
    case '\"':
      break;
    case '?':
      break;
    case '!':
      break;
    case '=':
      break;
    default:
      curstring[len]=c; len++;
      break;
    }
  }
}

void get_string(char *buff, string *str, char pos)
{
  ;  
}

