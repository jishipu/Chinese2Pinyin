#include <stdbool.h>
#include <stdio.h>
#include <memory.h>
#include "functions.h"

#define MAX_PINYIN_LEN 6

void utf8_to_pinyin(sds in, sds out, FILE *fp)
{
    int start = 19968;
    char* words = in;
    char buf[MAX_PINYIN_LEN];
    memset(buf, 0, MAX_PINYIN_LEN);
    while(*words != '\0')
    {
        if((*words >> 7) == 0)
        {
            sdscatlen(out, words, 1);
            words++;
        }
        else if((*words & 0xE0) == 0xC0)
        {
            sdscatlen(out, words, 2);
            words += 2;
        }
        else if((*words & 0xF0) == 0xE0)
        {
            if(*(words + 1) != '\0' && *(words + 2) != '\0')
            {
                int tmp = (((int)(*words & 0x0F)) << 12) | (((int)(*(words+1) & 0x3F)) << 6) | (*(words+2) & 0x3F);
                if(tmp > 19967 && tmp < 40870)
                {
                    int offset = (tmp - start) * MAX_PINYIN_LEN;
                    fseek(fp, offset, SEEK_SET);
                    fread(buf, MAX_PINYIN_LEN, 1, fp);
                    sdscatlen(out, buf, MAX_PINYIN_LEN);
                    sdstrim(out, " ");  //remove white spaces at the first and at the end
                    words += 3;
                }
                else
                {
                    sdscatlen(out, words, 3);
                    words += 3;
                }
            }
            else
            {
                sdscatlen(out, words, 3);
                words += 3;
            }
        }
        else if((*words & 0xF8) == 0xF0)
        {
            sdscatlen(out, words, 4);
            words += 4;
        }
        else if((*words & 0xFC) == 0xF8)
        {
            sdscatlen(out, words, 5);
            words += 5;
        }
        else if((*words & 0xFE) == 0xFC)
        {
            sdscatlen(out, words, 6);
            words += 6;
        }
        else
        {
            sdscatlen(out, words, 1);
            words++;
            break;
        }
    }
}

int is_text_utf8(const char* str, long length)
{
    int i;
    int nBytes=0;//UFT8可用1-6个字节编码,ASCII用一个字节
    unsigned char chr;
    bool bAllAscii=true; //如果全部都是ASCII, 说明不是UTF-8
    for(i=0; i<length; i++)
    {
        chr= *(str+i);
        if( (chr&0x80) != 0 ) // 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,但用一个字节存,最高位标记为0,o0xxxxxxx
            bAllAscii= false;
        if(nBytes==0) //如果不是ASCII码,应该是多字节符,计算字节数
        {
            if(chr>=0x80)
            {
                if(chr>=0xFC&&chr<=0xFD)
                    nBytes=6;
                else if(chr>=0xF8)
                    nBytes=5;
                else if(chr>=0xF0)
                    nBytes=4;
                else if(chr>=0xE0)
                    nBytes=3;
                else if(chr>=0xC0)
                    nBytes=2;
                else
                {
                    return false;
                }
                nBytes--;
            }
        }
        else //多字节符的非首字节,应为 10xxxxxx
        {
            if( (chr&0xC0) != 0x80 )
            {
                return false;
            }
            nBytes--;
        }
    }
    if( nBytes > 0 ) //违反规则
    {
        return false;
    }

    if( bAllAscii ) //如果全部都是ASCII, 说明不是UTF-8
    {
        return false;
    }
    return true;
}
