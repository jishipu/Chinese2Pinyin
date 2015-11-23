/*
 * Copyright (C) 2015 Leslie Zhai <xiang.zhai@i-soft.com.cn>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <iostream>
#include <cstdio>

#include "sds.h"
#include "functions.h"

int main(int argc, char *argv[])
{
    sds words = nullptr;
    sds out = nullptr;
    FILE *fp = nullptr;
    int len = -1;

    words = sdsnew(argv[1] ? argv[1] : "我是中国人");
    len = sdslen(words);
    if (!is_text_utf8(words, len)) {
        std::cerr << "ERROR: " << words << " is not utf8 text" << std::endl;
        goto exit;
    }
    if (nullptr == (fp = fopen(CHINESE2PINYIN_DIR "/PinyinData.txt", "r"))) {
        std::cerr << "ERROR: failed to open file" << std::endl;
        goto exit;
    }
    out = sdsempty();
    utf8_to_pinyin(words, out, fp);
    std::cout << out << std::endl;
exit:
    if (words) {
        sdsfree(words);
        words = nullptr;
    }
    if (out) {
        sdsfree(out);
        out = nullptr;
    }
    if (fp) {
        fclose(fp);
        fp = nullptr;
    }
    
    return 0;
}
