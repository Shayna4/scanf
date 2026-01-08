#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <math.h>
#include <limits.h>
#include <float.h>

struct hms {
    int h;
    int m;
    int s;
};

/* ================= HELPERS ================= */
static int read_char(void) { return getchar(); }
static void unread_char(int c) { if (c != EOF) ungetc(c, stdin); }
static void skip_ws(void) {
    int c;
    while (isspace(c = read_char()));
    unread_char(c);
}
static int match_literal(char ch) {
    int c = read_char();
    if (c != ch) { unread_char(c); return 0; }
    return 1;
}

/* ---------- Integer Parsing ---------- */
static int parse_int(long long *out, int base, int width, long long min_limit, long long max_limit) {
    int c, neg = 0, used = 0;
    long long val = 0;

    skip_ws();
    c = read_char();

    if (c == '+' || c == '-') {
        neg = (c == '-');
        if (width > 0) width--;
        c = read_char();
    }

    while (c != EOF && (width < 0 || width-- > 0)) {
        int digit;
        if (isdigit(c)) digit = c - '0';
        else if (isalpha(c)) digit = toupper(c) - 'A' + 10;
        else break;
        if (digit >= base) break;
        
        // Check for overflow 
        long long limit = neg ? -min_limit : max_limit; 
        if (val > (limit - digit) / base) { 
            val = limit;  
            // Consume remaining valid digits without parsing  
            c = read_char();  
            while (c != EOF && (width < 0 || width-- > 0)) {  
                if (isdigit(c) || (isalpha(c) && toupper(c) - 'A' + 10 < base)) {  
                    c = read_char();  
                } else { 
                    break; 
                }  
            }  
           
            used = 1;  
            break; 
        } 

        val = val * base + digit;
        used = 1;
        c = read_char();
    }

    unread_char(c);
    if (!used) return 0;

    *out = neg ? -val : val;
    return 1;
}

/* ---------- Floating Parsing ---------- */
static int parse_float(long double *out, int width) {
    int c, neg = 0, used = 0;
    long double val = 0.0L, frac = 0.0L, div = 1.0L;

    skip_ws();
    c = read_char();

    if (c == '+' || c == '-') {
        neg = (c == '-');
        if (width > 0) width--;
        c = read_char();
    }

    /* integer part */
    while (isdigit(c) && (width < 0 || width-- > 0)) {
        val = val * 10 + (c - '0');
        used = 1;
        c = read_char();
    }

    /* fractional part */
    if (c == '.' && (width < 0 || width-- > 0)) {
        c = read_char();
        while (isdigit(c) && (width < 0 || width-- > 0)) {
            frac = frac * 10 + (c - '0');
            div *= 10;
            used = 1;
            c = read_char();
        }
        val += frac / div;
    }

    /* exponent */
    if ((c == 'e' || c == 'E') && (width < 0 || width-- > 0)) {
        int sign = 1, exp = 0;
        c = read_char();
        if (c == '+' || c == '-') {
            sign = (c == '-') ? -1 : 1;
            c = read_char();
        }
        if (!isdigit(c)) {
            unread_char(c);
            goto done;
        }
        while (isdigit(c) && (width < 0 || width-- > 0)) {
            exp = exp * 10 + (c - '0');
            c = read_char();
        }
        val *= powl(10.0L, sign * exp);
    }

done:
    unread_char(c);
    if (!used) return 0;

    *out = neg ? -val : val;
    return 1;
}



/* ================= scanfMine ================= */
// Custom scanf implementation
int scanfMine(const char *fmt, ...) {
    // Variable argument list
    va_list ap;
    va_start(ap, fmt);
    // Number of assigned fields
    int assigned = 0;
    // Process format string
    while (*fmt) {
        // Skip whitespace in format
        if (isspace(*fmt)) {
            // match any whitespace in input
            int c;
            while (isspace(c = getchar()));
            ungetc(c, stdin);
            fmt++;
            continue;
        }
        // Literal character
        if (*fmt != '%') {
            if (!match_literal(*fmt))
                break;
            fmt++;
            continue;
        }
        // Process format specifier
        fmt++;
        // Handle '%%' literal
        if (*fmt == '%') {
            if (!match_literal('%'))
                break;
            fmt++;
            continue;
        }
        // Check for assignment suppression
        int assign = 1;
        if (*fmt == '*') {
            assign = 0;
            fmt++;
        }
        // Parse width
        int width = 0;
        while (isdigit(*fmt))
            width = width * 10 + (*fmt++ - '0');
        if (width == 0) width = -1;
        // Handle length modifiers
        enum { NONE, HH, H, L, LL, BIGL } mod = NONE;
        if (*fmt == 'h'){
            if (*(fmt + 1) == 'h') mod = HH, fmt += 2;
            else mod = H, fmt++;
        }
        else if (*fmt == 'l') {
            if (*(fmt + 1) == 'l') mod = LL, fmt += 2;
            else mod = L, fmt++;
        }
        else if (*fmt == 'L') mod = BIGL, fmt++;
        // Handle conversion specifiers
        switch (*fmt++) {
        /* ---------- %d decimal integer ---------- */
        case 'd': {
            long long v;
            long long min_val = INT_MIN, max_val = INT_MAX; 
            if (mod == HH) { min_val = SCHAR_MIN; max_val = SCHAR_MAX; }  
            else if (mod == H) { min_val = SHRT_MIN; max_val = SHRT_MAX; }  
            else if (mod == L) { min_val = LONG_MIN; max_val = LONG_MAX; }  
            else if (mod == LL) { min_val = LLONG_MIN; max_val = LLONG_MAX; }  
            
            if (!parse_int(&v, 10, width, min_val, max_val)) goto done;  
            if (assign) {
                if (mod == HH) *va_arg(ap, signed char *) = (signed char)v;
                else if (mod == H) *va_arg(ap, short *) = (short)v;
                else if (mod == L) *va_arg(ap, long *) = (long)v;
                else if (mod == LL) *va_arg(ap, long long *) = v;
                else *va_arg(ap, int *) = (int)v;
                assigned++;
            }
            break;
        }
        /* ---------- %x hexadecimal integer ---------- */
        case 'x': {
            long long v;
            int c1, c2;

            skip_ws();

            int width_remaining = width;

            // optional 0x / 0X prefix
            c1 = getchar();
            if (c1 == '0') {
                if (width_remaining > 0) width_remaining--;
                c2 = getchar();
                if (c2 == 'x' || c2 == 'X') {
                    if (width_remaining > 0) width_remaining--;
                    // prefix consumed
                } else {
                    unread_char(c2);
                    unread_char(c1);
                    width_remaining = width;
                }
            } else {
                unread_char(c1);
            }

            // Determine limits based on modifier  
            long long min_val = 0, max_val = UINT_MAX;  
            if (mod == HH) { max_val = UCHAR_MAX; }  
            else if (mod == H) { max_val = USHRT_MAX; }  
            else if (mod == L) { max_val = ULONG_MAX; }  
            else if (mod == LL) { max_val = ULLONG_MAX; }  

            if (!parse_int(&v, 16, width_remaining, min_val, max_val))  
                goto done;
                
            // assign value
            if (assign) {
                if (mod == HH) *va_arg(ap, unsigned char *) = (unsigned char)v;  
                else if (mod == H) *va_arg(ap, unsigned short *) = (unsigned short)v;  
                else if (mod == L) *va_arg(ap, unsigned long *) = (unsigned long)v;
                else if (mod == LL) *va_arg(ap, unsigned long long *) = (unsigned long long)v;
                else *va_arg(ap, unsigned int *) = (unsigned int)v;
                assigned++;
            }
            break;
        }

        /// ---------- %f floating point number ----------
        case 'f': {
            long double v;
            if (!parse_float(&v, width)) goto done;
            if (assign) {
                if (mod == BIGL) {
                    *va_arg(ap, long double *) = v;
                }
                 else if (mod == L){
                     if (v > DBL_MAX) v = DBL_MAX;
                     else if (v < -DBL_MAX) v = -DBL_MAX;
                     *va_arg(ap, double *) = (double)v;
                 }
                 else{
                     if (v > FLT_MAX) v = FLT_MAX;
                     else if (v < -FLT_MAX) v = -FLT_MAX;
                     *va_arg(ap, float *) = (float)v;
                 }
                 assigned++;
            }
            break;
        }

        /* ---------- %k percentage ---------- */
        case 'k': {
            long double v;
            // parse float
            if (!parse_float(&v, width)) goto done;
            // expect '%' literal
            if (!match_literal('%')) goto done;
            if (assign) {
                *va_arg(ap, float *) = (float)(v / 100.0);
                assigned++;
            }
            break;
        }

        /* ---------- %v vector [a, b, c] ---------- */
        case 'v': {
            float *arr = assign ? va_arg(ap, float *) : NULL;
            int len = va_arg(ap, int);

            skip_ws();
            if (!match_literal('[')) goto done;
            // read elements
            for (int i = 0; i < len; i++) {
                long double v;
                if (!parse_float(&v, -1)) goto done;
                if (assign) arr[i] = (float)v;
                skip_ws();
                if (i < len - 1) {
                    if (!match_literal(',')) goto done;
                }
            }

            skip_ws();
            if (!match_literal(']')) goto done;
            if (assign) assigned++;
            break;
        }

        /* ---------- %t time ---------- */
        //accepts total seconds followed by 's' 
        // returbns struct hms with h,m,s fields
        case 't': {
            long long total_seconds;
            struct hms *out = assign ? va_arg(ap, struct hms *) : NULL;

            skip_ws();
            /* ignore width here */
            if (!parse_int(&total_seconds, 10, -1, 0, LLONG_MAX))
                goto done;

            if (total_seconds < 0)
                goto done;

            if (!match_literal('s'))
                goto done;

            int h = (int)(total_seconds / 3600);
            int m = (int)((total_seconds % 3600) / 60);
            int s = (int)(total_seconds % 60);

            if (assign) {
                out->h = h;
                out->m = m;
                out->s = s;
                assigned++;  // one field only
            }
            break;
        }
        /* ---------- %c character ---------- */
        case 'c': {
            int num_chars = (width > 0) ? width : 1; //new line
            char *s = assign ? va_arg(ap, char *) : NULL; //new line
            for (int i = 0; i < num_chars; i++) { //new line
                int c = getchar(); //new line
                if (c == EOF) goto done; //new line
                if (assign) s[i] = (char)c; //new line
            } //new line
            if (assign) assigned++; //new line
            break;
        }


        /* ---------- %s string ---------- */
        case 's': {
            char *s = assign ? va_arg(ap, char *) : NULL;
            skip_ws();
            int c, used = 0;
            int count = 0;
            while ((c = getchar()) != EOF && !isspace(c)) {
                if (width >0 && count >= width) break;
                if (assign) *s++ = c;
                used = 1;
                count++;
            }
            unread_char(c);
            if (!used) goto done;
            if (assign){
                *s = '\0';
                assigned++;
            }
            break;
        }
        default:
            goto done;
        }
    }

done:
    // End variable argument processing
    va_end(ap);
    return assigned;
}
/* ================= Test Functions ================= */