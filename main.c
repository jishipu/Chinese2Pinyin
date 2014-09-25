#include <stdio.h>
#include <stdlib.h>
#include "sds.h"
#include "functions.h"

int main(int argc, char** argv)
{
    if(argc != 2) {
        fprintf(stderr, "Usage: ./Chinese2Pinyin words\n");
        exit(1);
    }

    sds words = sdsnew(argv[1]);
    int len = sdslen(words);
    if(!is_text_utf8(words, len)) {
        printf("%s\n", words);
        exit(1);
    }
    FILE* fp;
    if(NULL == (fp = fopen("PinyinData.txt", "r"))) {
        fprintf(stderr, "open file error\n");
        exit(1);
    }
    sds out = sdsempty();
    utf8_to_pinyin(words, out, fp);
    printf("%s\n", out);
    sdsfree(words);
    sdsfree(out);
    fclose(fp);
    return 0;
}

