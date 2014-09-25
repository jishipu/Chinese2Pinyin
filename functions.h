#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include "sds.h"

void utf8_to_pinyin(sds in, sds out, FILE *fp);
int is_text_utf8(const char* str, long length);

#endif // FUNCTIONS_H_INCLUDED
