#include <stdio.h>
#include <stdarg.h>
#include "cc_dbg.h"
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>

enum DBG_COLOR
{
    NORMAL = 0,
    RED,//1
    YELLOW,//2
    PURE,//3
    GREEN,//4
    BBLUE,//5
    BRED,//6
    BYELLOW,//7
    BPURE,//8
    BGREEN,//9
    DBG_COLOR_END
};

#define DBG_FILE "/oem/dbg"

int check_dbg()
{
    int dbg_level = -1;
    if (access(DBG_FILE, F_OK) == 0)
    {
        FILE * fp = fopen(DBG_FILE, "r");
        fscanf(fp, "%d\n", &dbg_level);
        fclose(fp);
    }
    return dbg_level;
}

static int cprintf(const char *format, ...)
{
    va_list args;
    int rv;
    FILE* fp;

    fp = fopen("/dev/console", "a");
    if(!fp)
        return -1;

    va_start(args, format);
    rv = vfprintf(fp, format, args);
    va_end(args);

    fclose(fp);

    return rv;
}

inline int _dbg_printf(int color,const char * func,const int line, const char *format, ...) 
{
    int rv = -1;
    int dbg_level = check_dbg();
    if (color <= dbg_level)
    {
        if ((dbg_level < 10) || (dbg_level >= 10 && color == (dbg_level - 10)))
        {
            va_list args;
            struct timeval tv;
            int format_len = 0;
            gettimeofday(&tv, NULL);
            switch(color)
            {
                case GREEN:cprintf("\033[;32m");break;
                case RED:cprintf("\033[;31m");break;
                case YELLOW:cprintf("\033[;33m");break;
                case PURE:cprintf("\033[;35m");break;
                case BGREEN:cprintf("\033[42;30m");break;
                case BRED:cprintf("\033[41;30m");break;
                case BYELLOW:cprintf("\033[43;30m");break;
                case BPURE:cprintf("\033[45;30m");break;
                case BBLUE:cprintf("\033[44;30m");break;
                default:cprintf("\033[;37m");break;
            }
            cprintf("[%ld.%ld][%s]:%d ", tv.tv_sec, tv.tv_usec / 1000, func, line);
            va_start(args, format);
            format_len = strlen(format) + 1;
            char * buf = malloc(format_len);
            vsnprintf(buf, format_len, format, args);
            va_end(args); 
            if (buf[format_len - 2] == '\n') 
            {   
                buf[format_len - 2] = '\0';
            }
            rv = cprintf("%s\033[0m\n", buf);
            free(buf);
        }
    }
    return rv;
}

#define dbg_printf(color, ...) _dbg_printf(color, __func__, __LINE__, __VA_ARGS__)  

int main()
{
    dbg_printf(NORMAL, "hello world");
    dbg_printf(GREEN, "hello world\n");
    dbg_printf(RED, "hello world");
    dbg_printf(YELLOW, "hello world");
    dbg_printf(PURE, "hello world");
    return 0;
}