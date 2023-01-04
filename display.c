#include "display.h"
#include <stdarg.h>
#include <stdio.h>
#include <time.h>

void display(char* str, ...)
{
    va_list ap;
    char out[512];
    va_start(ap, str);
    printf("\033[1A"); // カーソルを1行だけ上に移動
    printf("\033[2K"); // カーソル位置の行をクリア
    printf("\r"); // 行先頭に移動
    vsprintf(out, str, ap);
    printf("%s", out);
    fflush(stdout);
}

/*
void display(char* str)
{
    // printf("\033[1A"); // カーソルを1行だけ上に移動
    printf("\033[2K"); // カーソル位置の行をクリア
    printf("\r"); // 行先頭に移動
    printf("%s", str);
}
*/