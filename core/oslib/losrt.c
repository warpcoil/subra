#include "../include/oslib/losrt.h"

#define STD_SIZE	0
#define SHORT_SIZE	1
#define LONG_SIZE	2

/* Flag bit settings */
#define RESPECT_WIDTH	1  /* Fixed width wanted 	*/
#define ADD_PLUS	2  /* Add + for positive/floats */
#define SPACE_PAD	4  /* Padding possibility	*/
#define ZERO_PAD	8
#define LEFT_PAD 	16

#define INT_MAX 2147483647

int isnumber(char c,int base)
{
    static char *digits = "0123456789ABCDEF";
    if ((c >= '0' && c <= digits[base-1]))
        return(1);
    else return(0);
}

int tonumber(char c)
{
    if (c >= '0' && c <= '9') return(c - '0');
    else if (c >= 'A' && c <= 'F') return(c - 'A' + 10);
    else if (c >= 'a' && c <= 'f') return(c - 'a' + 10);
    else return(c);
}

char todigit(int c)
{
    if (c >= 0 && c <= 9) return(c+'0');
    else if (c >= 10 && c <= 15) return(c + 'A' - 10);
    else return(c);
}

unsigned long strtoul(const char *s, char **scan_end, int base)
{
    int value,overflow = 0;
    long unsigned result = 0,oldresult;
    /* Skip trailing zeros */
    while (*s == '0') s++;
    if (*s == 'x' && base == 16) {
        s++;
        while (*s == '0') s++;
    }
    /* Convert number */
    while (isnumber(*s,base)) {
        value = tonumber(*s++);
        if (value > base || value < 0) return(0);
        oldresult = result;
        result *= base;
        result += value;
        /* Detect overflow */
        if (oldresult > result) overflow = 1;
    }
    if (scan_end != 0L) *scan_end = (char *)s;
    if (overflow) result = INT_MAX;
    return(result);
}

long strtol(const char *s, char **scan_end, int base) {
    int value,overflow = 0;
    long result = 0,oldresult;
    bool neg = false;

    if (*s == '-'){
        neg = true;
        s++;
    }

    /* Skip trailing zeros */
    while (*s == '0') s++;
    if (*s == 'x' && base == 16) {
        s++;
        while (*s == '0') s++;
    }
    while (isnumber(*s,base)) {
        value = tonumber(*s++);
        if (value > base || value < 0) return(0);
        oldresult = result;
        result *= base;
        result += value;
        /* Detect overflow */
        if (oldresult > result) overflow = 1;
    }
    if (scan_end != 0L) *scan_end = (char *)s;

    if (neg) {
        result = -(result);
    }

    if (overflow) result = INT_MAX;
    return(result);
}

unsigned ucvt(unsigned long v,char *buffer,int base,int width,int flag)
{
    register int i = 0,j;
    unsigned ret = 0,abs_base;
    unsigned long abs_v;
    char tmp[12];
    /* Extract the less significant digit */
    /* Put it into temporary buffer       */
    /* It has to be local to have 	  */
    /* reentrant functions		  */
    /*
    MG: fix to handle zero correctly
    if (v == 0) {
    *buffer++ = '0';
    *buffer = 0;
    return(1);
    }
    */
    
    /* MG: is this required? */
    /* the vsprintf() function seems to call this procedure with */
    /* this flag inverted */
    flag ^= LEFT_PAD;
    
    abs_base = (base >= 0) ? base : -base;
    if (base < 0) abs_v = ((long)(v) < 0) ? -v : v;
    else abs_v = v;
    /* Process left space-padding flags */
    if (flag & ADD_PLUS || ((base < 0) && ((long)(v) < 0))) {
        ret++;
    }
    /* MG: fix to handle zero correctly */
    if (v == 0)
        tmp[i++]='0';
    else
        while (abs_v > 0) {
            tmp[i++] = todigit(abs_v % abs_base);
            abs_v = abs_v / abs_base;
        }
    ret += i;
    if ((flag & LEFT_PAD) && (flag & SPACE_PAD)) {
        j = ret;
        while (j < width) {
            *buffer++ = ' ';
            ret++;
            j++;
        }
    }
    /* Add plus if wanted */
    if (base < 0) {
        if (((long)(v)) < 0) *buffer++ = '-';
        else if (flag & ADD_PLUS) *buffer++ = '+';
    } else if (flag & ADD_PLUS) *buffer++ = '+';
    /* Process left zero-padding flags */
    if ((flag & LEFT_PAD) && (flag & ZERO_PAD)) {
        j = ret;
        while (j < width) {
            *buffer++ = '0';
            ret++;
            j++;
        }
    }
    /* Reverse temporary buffer into output buffer */
    /* If wanted left pad with zero/space; anyway only one at once is ok */
    for (j = i-1; j >= 0; j--) *buffer++ = tmp[j];
    if ((flag & (SPACE_PAD)) && !(flag & LEFT_PAD)) {
        /* If wanted, pad with space to specified width */
        j = ret;
        while (j < width) {
            *buffer++ = ' ';
            ret++;
            j++;
        }
    }
    /* Null terminate the output buffer */
    *buffer = 0;
    return(ret);
}

unsigned dcvt(long v,char *buffer,int base,int width,int flag)
{
    return(ucvt((unsigned long)(v),buffer,-base,width,flag));
}

int vsnprintf (char *buf, size_t n, const char *fmt, va_list parms) {
    int scanned = 0,w = 0,/*prec = 0,*/l = 0,size = 0;
    int n1 = 0;
    unsigned n2 = 0,parsing = 0,flag = 0;
    char *base = buf;
    char *sp = NULL;

    while ((*fmt != 0) && (scanned < n)) {
        if (*fmt != '%' && !parsing) {
            /* No token detected */
            *buf++ = *fmt++;
            scanned++;
        }
        else {
            /* We need to make a conversion */
            if (*fmt == '%') {
                fmt++;
                parsing = 1;
                w = 10;
                /*prec = 4;*/
                size = STD_SIZE;
                flag = 0;
            }
            /* Parse token */
            switch(*fmt) {
            case '%' : *buf++ = '%';
                scanned++;
                parsing = 0;
                break;
            case 'c' : *buf++ = va_arg(parms, char);
                scanned++;
                parsing = 0;
                break;
            case 'i' :
            case 'd' : switch (size) {
                case STD_SIZE : n1 = va_arg(parms, int);
                    break;
                case LONG_SIZE : n1 = va_arg(parms, long int);
                    break;
                case SHORT_SIZE : n1 = va_arg(parms, short int);
                    break;
                }
                l = dcvt(n1,buf,10,w,flag);
                scanned += l;
                buf += l;
                parsing = 0;
                break;
            case 'u' : switch (size) {
                case STD_SIZE : n2 = va_arg(parms, unsigned);
                    break;
                case LONG_SIZE : n2 = va_arg(parms, unsigned long);
                    break;
                case SHORT_SIZE : n2 = va_arg(parms, unsigned short);
                    break;
                }
                l = ucvt(n2,buf,10,w,flag);
                scanned += l;
                buf += l;
                parsing = 0;
                break;
            case 'x' : switch (size) {
                case STD_SIZE : n2 = va_arg(parms, unsigned);
                    break;
                case LONG_SIZE : n2 = va_arg(parms, unsigned long);
                    break;
                case SHORT_SIZE : n2 = va_arg(parms, unsigned short);
                    break;
                }
                l = ucvt(n2,buf,16,w,flag);
                scanned += l;
                buf += l;
                parsing = 0;
                break;
            case 's' : sp = va_arg(parms, char *);
                while (*sp != 0) {
                    *buf++ = *sp++;
                    l++;
                }
                scanned += l;
                parsing = 0;
                break;
            case 'l' : size = LONG_SIZE;
                break;
            case 'n' :
            case 'h' : size = SHORT_SIZE;
                break;
            case '+' : flag |= ADD_PLUS;
                break;
            case '-' : flag |= LEFT_PAD;
                break;
            case '.' : parsing = 2;
                flag |= RESPECT_WIDTH;
                break;
            case '1' :
            case '2' :
            case '3' :
            case '4' :
            case '5' :
            case '6' :
            case '7' :
            case '8' :
            case '9' :
            case '0' : if (parsing == 1) {
                    w = strtoul((char *)fmt,&base,10);
                    /* MG */
                    /* if the first numeral is zero a ZERO pad is */
                    /* required */
                    /* but not if LEFT_PAD is set*/
                    if (*fmt!='0'||flag&LEFT_PAD)
                        flag |= SPACE_PAD ;
                    else
                        flag |= ZERO_PAD ;
                    fmt = base-1;
                } else if (parsing == 2) {
                    /*prec = strtou(fmt,10,&base);*/
                    fmt = base-1;
                    parsing = 1;
                }
                break;
            default :  parsing = 0;
                break;
            }
            fmt++;
        }
    }
    *buf = 0;
    return(scanned);
}

int vksprintf(char *buf,const char *fmt,va_list parms)
{
    return vsnprintf(buf, MAX_DWORD, fmt, parms);
}

int ksprintf(char *buf,char *fmt,...)
{
    va_list parms;
    int result;

    va_start(parms,fmt);
    result = vksprintf(buf,fmt,parms);
    va_end(parms);
    return(result);
}

int snprintf(char * buf, size_t n, const char *fmt, ...) {
    va_list parms;
    int result;

    va_start(parms,fmt);
    result = vsnprintf(buf, n, fmt, parms);
    va_end(parms);
    return(result);
}

void halt(void)
{
    __asm__ ("hlt");
}

static void exit (int code) {
    halt();
}

void l1_exit(int code)
{
    extern void bios_restore(void);

    bios_restore();
    exit(code);
}

int message(const char *fmt,...)
{
    static char cbuf[500];
    va_list parms;
    int result;

    va_start(parms,fmt);
    result = vksprintf(cbuf,fmt,parms);
    va_end(parms);
    cputs(cbuf);
    return(result);
}

char *strcpy(char *dst,const char *src)
{
    char *retval = dst;
    while (*src != 0) *dst++ = *src++;
    *dst = 0;
    return(retval);
}

char *strncpy(char *dst,const char *src,int n) {

    char *retval = dst;

    while (*src != 0) {
        if (n == 0)
            return retval;

        *dst = *src;
        dst++;
        src++;
        n--;
    }

    if (n > 0) {
        while (n > 0) {
            *dst = 0;
            n--;
        }
    }

    return retval;

}

int strlen(const char *s)
{
    register int result = 0;
    while (*s != 0) s++ , result++;
    return(result);
}

char *strchr(const char *s,int c) {

    while (*s != 0) {
        if (*s == (char)c){
            return (char *)s;
        }
        s++;
    }

    return NULL;

}

char *strupr(char *s) {
    int len = strlen(s);

    for (int i=0; i<len; i++) {
        if ((s[i] >= 'a') && (s[i] <= 'z')) {
            s[i] -= 32;
        }
    }

    return s;
}

char *strlwr(char *s) {
    int len = strlen(s);

    for (int i=0; i<len; i++) {
        if ((s[i] >= 'A') && (s[i] <= 'Z')) {
            s[i] += 32;
        }
    }

    return s;
}

int strcmp(const char *s1,const char *s2) {
    while (*s1 == *s2) {
        if (*s1 == 0) return 0;
        s1++;
        s2++;
    }
    return *(unsigned const char *)s1 - *(unsigned const char *)(s2);
}

int strncmp(const char *s1,const char *s2,int n) {
    while(n--) {
        if(*s1 != *s2) {
            return *s1 - *s2;
        }
        else {
            ++s1;
            ++s2;
        }
    }

    return 0;
}

int fflush(FILE *stream) {
    stream=stream;
    //No flush -- internal use flush, is direct
    return 0;
}

int fputc(int c, FILE *stream) {
    stream=stream;
    cputc(c);
    return c;
}

int fwrite(const void * ptr, size_t size, size_t count, FILE * stream) {
    ptr=ptr;
    size=size;
    count=count;
    message("WARN: fwrite: %s:\n", stream->filename);
    message((char *)ptr);
    return size;
}

int fprintf(FILE *file, const char *fmt, ...) {

    cputs(file->filename);
    cputs(" - ");

    static char cbuf[500];
    va_list parms;
    int result;

    va_start(parms,fmt);
    result = vksprintf(cbuf,fmt,parms);
    va_end(parms);
    cputs(cbuf);
    return(result);
}

//char * strdup(const char *s)
int tolower(int c) {
    if ((c > 0x40) && (c < 0x5b)) {
        return c + 0x20;
    }

    return c;
}

int isalpha(int c) {
    if ((c > 0x40) && (c < 0x5b))
        return 1;

    if ((c > 0x60) && (c < 0x7b))
        return 1;

    return 0;
}

int isdigit(int c) {
    if ((c > 0x2f) && (c < 0x3a))
        return 1;

    return 0;
}

int isspace(int c) {
    if ((c == ' ') || (c == '\t') || (c == '\n') || (c == '\v') || (c == '\f') || (c == '\r'))
        return 1;

    return 0;
}

int isalnum(int c) {

    if (isalpha(c))
        return 1;

    if (isdigit(c))
        return 1;

    return 0;
}

int isxdigit(int c) {

    if (isdigit(c))
        return 1;

    if ((c > 0x40) && (c < 0x47))
        return 1;

    if ((c > 0x60) && (c < 0x67))
        return 1;

    return 0;
}

int isprint(int c) {

    if ((c > 0x1f) && (c < 0x7f))
        return 1;

    if ((c > 0x7f) && (c < 0xff)) //Extended ASCII
        return 1;

    return 0;
}

/* CGA compatible registers value */

#define CURSOR_POS_MSB		0x0E
#define CURSOR_POS_LSB		0x0F
#define CURSOR_START		0x0A
#define CURSOR_END		0x0B

/* CGA compatible registers */

#define CGA_INDEX_REG	0x3D4
#define CGA_DATA_REG	0x3D5

/* Standard tab size */

#define TAB_SIZE	8

/* Store bios settings */

static unsigned char bios_start,bios_end;
BYTE bios_x, bios_y, bios_attr;

/* Access directly to video & BIOS memory through linear addressing */

/* Active video page-buffer */
#define PAGE_SIZE		2048

int active_page = 0;
int visual_page = 0;

void bios_save(void)
{
    /* This function must be called to init CONSole output */

    bios_attr = lmempeekb((LIN_ADDR)0xB8000 + 159);
    bios_x = lmempeekb((LIN_ADDR)0x00450);
    bios_y = lmempeekb((LIN_ADDR)0x00451);
    bios_end = lmempeekb((LIN_ADDR)0x00460);
    bios_start = lmempeekb((LIN_ADDR)0x00461);
    active_page = visual_page = 0;
}

void getcursorxy(int *x, int *y)
{
    *x = bios_x;
    *y = bios_y;
}

int get_attr(void)
{
    return bios_attr;
}

void cursor(int start,int end)
{
    /* Same thing as above; Set cursor scan line */
    outp(CGA_INDEX_REG, CURSOR_START);
    outp(CGA_DATA_REG, start);
    outp(CGA_INDEX_REG, CURSOR_END);
    outp(CGA_DATA_REG, end);
}

void bios_restore(void)
{
    lmempokeb((LIN_ADDR)0x00450,bios_x);
    lmempokeb((LIN_ADDR)0x00451,bios_y);
    place(bios_x,bios_y);
    cursor(bios_start, bios_end);
}

void place(int x,int y)
{
    unsigned short cursor_word = x + y*80 + active_page*PAGE_SIZE;
    /* Set cursor position				  */
    /* CGA is programmed writing first the Index register */
    /* to specify what internal register we are accessing */
    /* Then we load the Data register with the wanted val */
    outp(CGA_INDEX_REG,CURSOR_POS_LSB);
    outp(CGA_DATA_REG,cursor_word & 0xFF);
    outp(CGA_INDEX_REG,CURSOR_POS_MSB);
    outp(CGA_DATA_REG,(cursor_word >> 8) & 0xFF);
    /* Adjust temporary cursor bios position */
    bios_x = x;
    bios_y = y;
}


void _scroll(char attr,int x1,int y1,int x2,int y2)
{
    register int x,y;
    WORD xattr = (((WORD)attr) << 8) + ' ',w;
    LIN_ADDR v = (LIN_ADDR)(0xB8000 + active_page*(2*PAGE_SIZE));

    for (y = y1+1; y <= y2; y++)
        for (x = x1; x <= x2; x++) {
            w = lmempeekw((LIN_ADDR)(v + 2*(y*80+x)));
            lmempokew((LIN_ADDR)(v + 2*((y-1)*80+x)),w);
        }
    for (x = x1; x <= x2; x++)
        lmempokew((LIN_ADDR)(v + 2*((y-1)*80+x)),xattr);
}

void scroll(void)
{
    _scroll(bios_attr,0,0,79,24);
}

void cputc(char c)
{
    static unsigned short scan_x,x,y;
    LIN_ADDR v = (LIN_ADDR)(0xB8000 + active_page*(2*PAGE_SIZE));
    x = bios_x;
    y = bios_y;
    switch (c) {
    case '\t' : x += 8;
        if (x >= 80) {
            x = 0;
            if (y == 24) scroll();
            else y++;
        } else {
            scan_x = 0;
            while ((scan_x+8) < x) scan_x += 8;
            x = scan_x;
        }
        break;
    case '\n' : x = 0;
        if (y == 24) scroll();
        else y++;
        break;
    case '\b' : x--;
        lmempokeb((LIN_ADDR)(v + 2*(x + y*80)),' ');
        x++;
        break;
    default   : lmempokeb((LIN_ADDR)(v + 2*(x + y*80)),c);
        x++;
        if (x > 80) {
            x = 0;
            if (y == 24) scroll();
            else y++;
        }
    }
    place(x,y);
}


void cputs(char *s)
{
    char c;
    while (*s != '\0') {
        c = *s++;
        cputc(c);
    }
}
