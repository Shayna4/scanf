#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <math.h>

struct hms {
    int h;
    int m;
    int s;
};

/* ================= HELPERS ================= */
// Read a character from input
static int read_char(void) {
    return getchar();
}
// Unread a character back to input
static void unread_char(int c) {
    if (c != EOF) ungetc(c, stdin);
}
// Skip whitespace characters
static void skip_ws(void) {
    int c;
    while (isspace(c = read_char()))
        ;
    unread_char(c);
}
// Match a literal character from input
static int match_literal(char ch) {
    int c = read_char();
    if (c != ch) {
        unread_char(c);
        return 0;
    }
    return 1;
}

/* ---------- Integer Parsing ---------- */
// Parse integer of given base and width
static int parse_int(long long *out, int base, int width) {
    int c, neg = 0, used = 0;
    long long val = 0;
    // Skip leading whitespace
    skip_ws();
    c = getchar();
    // Check for sign
    if (c == '-' || c == '+') {
        neg = (c == '-');
        width--;
        c = getchar();
    }
    // Parse digits
    while (c != EOF && (width < 0 || width-- > 0)) {
        int digit;
        if (isdigit(c)) digit = c - '0';
        else if (isalpha(c)) digit = toupper(c) - 'A' + 10;
        else break;

        if (digit >= base) break;

        val = val * base + digit;
        used = 1;
        c = getchar();
    }
    // Unread last character
    unread_char(c);
    
    if (!used) return 0;
    *out = neg ? -val : val;
    return 1;
}

/* ---------- Floating Parsing ---------- */
// Parse floating-point number with given width
static int parse_float(long double *out, int width) {
    int c, neg = 0, used = 0;
    long double val = 0.0L, frac = 0.0L, div = 1.0L;
    // Skip leading whitespace
    skip_ws();
    c = getchar();
    // Check for sign
    if (c == '+' || c == '-') {
        neg = (c == '-');
        if (width > 0) width--;
        c = getchar();
    }

    /* integer part */
    while (isdigit(c) && width != 0) {
        val = val * 10 + (c - '0');
        used = 1;
        if (width > 0) width--;
        c = getchar();
    }

    /* fractional part */
    if (c == '.' && width != 0) {
        if (width > 0) width--;
        c = getchar();
        
        while (isdigit(c) && width != 0) {
            frac = frac * 10 + (c - '0');
            div *= 10;
            used = 1;
            if (width > 0) width--;
            c = getchar();
        }
        val += frac / div;
    }

    /* exponent */
    if ((c == 'e' || c == 'E') && width != 0) {
        int save_width = width;
        int sign = 1, exp = 0;
        // consume 'e'/'E'
        if (width > 0) width--;
        int d = getchar();
        // check for exponent sign
        if (d == '+' || d == '-') {
            sign = (d == '-') ? -1 : 1;
            if (width > 0) width--;
            d = getchar();
        }
        // parse exponent digits
        if (!isdigit(d)) {
            unread_char(d);
            unread_char(c);
            goto done;
        }
        // restore width for exponent digits
        while (isdigit(d) && width != 0) {
            exp = exp * 10 + (d - '0');
            if (width > 0) width--;
            d = getchar();
        }
        // apply exponent
        unread_char(d);
        val *= powl(10.0L, sign * exp);
    }
    else {
        // no exponent, unread last char
        unread_char(c);
    }
    done:

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
        enum { NONE, H, L, LL, BIGL } mod = NONE;
        if (*fmt == 'h') mod = H, fmt++;
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
            if (!parse_int(&v, 10, width)) goto done;
            if (assign) {
                if (mod == L) *va_arg(ap, long *) = (long)v;
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

            int width_digits = width; // save original width for digits

            // optional 0x / 0X prefix
            c1 = getchar();
            if (c1 == '0') {
                c2 = getchar();
                if (c2 != 'x' && c2 != 'X') {
                    unread_char(c2);
                    unread_char(c1);
                }
                // else do nothing, prefix consumed
            } else {
                unread_char(c1);
            }


            if (!parse_int(&v, 16, width_digits)) // use digits-only width
                goto done;
            // assign value
            if (assign) {
                if (mod == L) *va_arg(ap, unsigned long *) = (unsigned long)v;
                else if (mod == LL) *va_arg(ap, unsigned long long *) = (unsigned long long)v;
                else *va_arg(ap, unsigned int *) = (unsigned int)v;
                assigned++;
            }
            break;
        }


        /// ---------- %u unsigned decimal integer ----------
        case 'f': {
            long double v;
            if (!parse_float(&v, width)) goto done;
            if (assign) {
                if (mod == BIGL) *va_arg(ap, long double *) = v;
                else if (mod == L) *va_arg(ap, double *) = (double)v;
                else *va_arg(ap, float *) = (float)v;
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
            if (!parse_int(&total_seconds, 10, -1))
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
        /* ---------- %s string ---------- */
        case 'c': {
            int c = getchar();
            if (c == EOF) goto done;
            if (assign) {
                *va_arg(ap, char *) = (char)c;
                assigned++;
            }
            break;
        }

        /* ---------- %s string ---------- */
        case 's': {
            char *s = assign ? va_arg(ap, char *) : NULL;
            skip_ws();
            int c, used = 0;
            while ((c = getchar()) != EOF &&
                   !isspace(c) &&
                   width-- != 0) {
                if (assign) *s++ = c;
                used = 1;
            }
            unread_char(c);
            if (!used) goto done;
            if (assign) *s = '\0';
            assigned++;
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