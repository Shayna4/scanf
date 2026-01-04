
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h> 
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <math.h>

int scanfMine(const char *format,...) {
  va_list pointer;
  va_start(pointer, format);
  
  //two
  int c;
  int counted = 0;
  
  while (*format != '\0'){
    if (*format != '%'){
      if(isspace(*format)){
	while (isspace(c = getchar()));
  ungetc(c, stdin); 
	format++;
	continue;
      }else{
      c = getchar();
      if (c != *format){
	break;
      }
      format++;
      continue;
      }
    }
    format++;
    
     // Check for modifier - can have at most one modifier
    enum {NONE, HH, H, L, LL, J, Z, T, CAPITAL_L} mod = NONE;
    if (*format == 'h') {
      if (*(format + 1) == 'h') { mod = HH; format += 2; }
      else { mod = H; format++; }
    }
    else if (*format == 'l') {
      if (*(format + 1) == 'l') { mod = LL; format += 2; }
      else { mod = L; format++; }
    }
    else if (*format == 'j') { mod = J; format++; }
    else if (*format == 'z') { mod = Z; format++; }
    else if (*format == 't') { mod = T; format++; }
    else if (*format == 'L') { mod = CAPITAL_L; format++; }
    

    //skip leading whitespace for all
    switch(*format){
    case 'c':{
      char *w = va_arg(pointer, char *);
      //how does the real scanf handle spaces?
      //while(isspace(c= getchar()));
      c = getchar();
      *w = c;
      counted ++;
      break;
    }
    case 's':{
      char *w = va_arg(pointer, char*);
      int matched = 0;
      while (isspace(c = getchar())); 
      //*w++ = c;
      while ((c != EOF) && (!isspace(c))){
        matched = 1;
        *w++ = c;
        c = getchar();
      }
      *w = '\0';
      if (matched) counted++;
      break;
    }
    case 'x':{
      //might be a prob if more than one char
      unsigned int *n = va_arg(pointer, unsigned int *);
      
      while (isspace(c = getchar()));
      if (c == '0'){
        int c = getchar();
        if (c == 'x' || c == 'X'){
          c = getchar();
        }else{
          //unget the char after 0 -> c is is 0 which is valid hex digit
          ungetc(c, stdin);
      }
    }
        unsigned int val = 0;
        int matched = 0;
      while (isxdigit(c)){
	  val *= 16;
	  if (isdigit(c)) val += c - '0';
	  else val += toupper(c) - 'A'+10;
	  c = getchar();
      }
      if (!isxdigit(c) && c != EOF) ungetc(c, stdin);
      matched = 1;
      *n = val;
      switch (mod) {
        case HH: *va_arg(pointer, unsigned char *) = (unsigned char)val; break;
        case H:  *va_arg(pointer, unsigned short *) = (unsigned short)val; break;
        case L:  *va_arg(pointer, unsigned long *) = (unsigned long)val; break;
        case LL: *va_arg(pointer, unsigned long long *) = val; break;
        case J:  *va_arg(pointer, uintmax_t *) = val; break;
        case Z:  *va_arg(pointer, size_t *) = (size_t)val; break;
        case T:  *va_arg(pointer, ptrdiff_t *) = (ptrdiff_t)val; break;
        default: *va_arg(pointer, unsigned int *) = (unsigned int)val; break;
                }
    
    if (!matched) {
        if (c != EOF) ungetc(c, stdin);
    break; // input not valid → don't increment counted
    }
      counted++;
      break;
    }

    case 'd': {
    int neg = 0;
    long long val = 0; // use long long to safely accumulate large numbers
    int matched = 0;

    // skip leading whitespace
    do { c = getchar(); } while (isspace(c));

    // optional sign
    if (c == '-') { neg = 1; c = getchar(); }
    else if (c == '+') { c = getchar(); }

    // check for at least one digit
    if (!isdigit(c)) {
        if (c != EOF) ungetc(c, stdin); // put the non-digit back
        break; // scanf would stop here, counted not incremented
    }

    // accumulate digits
    while (isdigit(c)) {
        matched = 1;
        val = val * 10 + (c - '0');
        c = getchar();
    }

    if (c != EOF) ungetc(c, stdin); // put back first non-digit

    if (!matched) break; // no digits, do not increment counted

    if (neg) val = -val;

    // assign based on modifier
    switch (mod) {
        case HH: *va_arg(pointer, signed char *) = (signed char)val; break;
        case H:  *va_arg(pointer, short *) = (short)val; break;
        case L:  *va_arg(pointer, long *) = (long)val; break;
        case LL: *va_arg(pointer, long long *) = (long long)val; break;
        case J:  *va_arg(pointer, intmax_t *) = (intmax_t)val; break;
        case Z:  *va_arg(pointer, size_t *) = (size_t)val; break;
        case T:  *va_arg(pointer, ptrdiff_t *) = (ptrdiff_t)val; break;
        default: *va_arg(pointer, int *) = (int)val; break;
    }

    counted++; // only increment if we successfully read a number
    break;
}
   case 'f': {
    int neg = 0, exp_neg = 0, matched = 0;
    double val = 0.0, frac_div = 1.0;
    int c;

    // skip leading whitespace
    do { c = getchar(); } while (isspace(c));

    // optional sign
    if (c == '-') { neg = 1; c = getchar(); }
    else if (c == '+') c = getchar();

    // parse integer part
    while (isdigit(c)) {
        matched = 1;
        val = val * 10.0 + (c - '0');
        c = getchar();
    }

    // parse fraction part
    if (c == '.') {
        c = getchar();
        while (isdigit(c)) {
            matched = 1;
            frac_div *= 10.0;
            val += (c - '0') / frac_div;
            c = getchar();
        }
    }

    // parse exponent part
    if (c == 'e' || c == 'E') {
        c = getchar();
        if (c == '-') { exp_neg = 1; c = getchar(); }
        else if (c == '+') c = getchar();

        int exp_val = 0, exp_matched = 0;
        while (isdigit(c)) {
            exp_val = exp_val * 10 + (c - '0');
            exp_matched = 1;
            c = getchar();
        }

        if (exp_matched) {
            double factor = pow(10.0, exp_val);
            if (exp_neg) factor = 1.0 / factor;
            val *= factor;
        } else {
            // Invalid exponent: put back all characters
            ungetc(c, stdin);
        }
    }

    if (!matched) {
        if (c != EOF) ungetc(c, stdin);
        break;  // nothing matched, do not increment counted
    }

    if (c != EOF) ungetc(c, stdin);
    if (neg) val = -val;

    // assign based on modifier
    switch (mod) {
        case L:         *va_arg(pointer, double *) = val; break;
        case CAPITAL_L: *va_arg(pointer, long double *) = (long double)val; break;
        default:        *va_arg(pointer, float *) = (float)val; break;
    }

    counted++;
    break;
}

    
    case 'p': {
      float *val = va_arg(pointer, float *);
      float v = 0.0f;
      int after_dot = 0;
      float div = 1.0f;

      // skip leading whitespace
      do { c = getchar(); } while (isspace(c));

      // optional sign
      int neg = 0;
      if (c == '-') { neg = 1; c = getchar(); }
      else if (c == '+') c = getchar();

      // read number (digits and optional dot)
      if (!isdigit(c) && c != '.') return counted;
      while (isdigit(c) || c == '.') {
          if (c == '.') { after_dot = 1; c = getchar(); continue; }
          if (!after_dot) v = v * 10.0f + (c - '0');
          else { div *= 10.0f; v += (c - '0') / div; }
          c = getchar();
      }

      if (c == '%') { v /= 100.0f; c = getchar(); }

      *val = neg ? -v : v;
      counted++;
      break;

    }
    case 'v': {
      float *arr = va_arg(pointer, float *);
      int length = va_arg(pointer, int); // number of elements to read
      int i = 0;

      while (i < length) {
          // skip whitespace
          do { c = getchar(); } while (isspace(c));

          float v = 0.0f, div = 1.0f;
          int neg = 0, after_dot = 0;

          if (c == '-') { neg = 1; c = getchar(); }
          else if (c == '+') c = getchar();

          if (!isdigit(c) && c != '.') break;

          while (isdigit(c) || c == '.') {
              if (c == '.') { after_dot = 1; c = getchar(); continue; }
              if (!after_dot) v = v * 10.0f + (c - '0');
              else { div *= 10.0f; v += (c - '0') / div; }
              c = getchar();
          }

          arr[i++] = neg ? -v : v;
      }
      counted++;
      break;
  }

    case '3':{
      while (isspace(c = getchar()));
      c = getchar();
      counted++;
      break;
    }
      
  }
  format++;
  }
  va_end(pointer);
  return counted;
}
