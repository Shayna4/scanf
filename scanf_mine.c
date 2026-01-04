#include <stdio.h>
#include <stdarg.h>
#include <ctype.h> 
#include <stdint.h>
#include <stddef.h>
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
      while (isspace(c = getchar())); 
      //*w++ = c;
      while ((c != EOF) && (!isspace(c))){
        *w++ = c;
        c = getchar();
      }
      *w = '\0';
      counted++;
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
      }}
      unsigned int val = 0;
      while (isxdigit(c)){
	  val *= 16;
	  if (isdigit(c)) val += c - '0';
	  else val += toupper(c) - 'A'+10;
	  c = getchar();
      }
      if (!isxdigit(c) && c != EOF) ungetc(c, stdin);
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
      counted++;
      break;
    }
      
    case 'd':{
      int *n = va_arg(pointer, int *);
      int v = 0, neg = 0;

      // skip leading whitespace
      do {
          c = getchar();
      } while (isspace(c));

      // handle optional sign
      if (c == '-') { neg = 1; c = getchar(); }
      else if (c == '+') { c = getchar(); }

      // make sure first char is a digit
      if (!isdigit(c)) {
          // input is invalid, return counted so far
          return counted;
      }

      // now accumulate digits
      do {
          v = v*10 + (c - '0');
      } while (isdigit(c = getchar()));

      *n = neg ? -v : v;


      switch (mod) {
          case HH: *va_arg(pointer, signed char *) = (signed char)v; break;
          case H:  *va_arg(pointer, short *) = (short)v; break;
          case L:  *va_arg(pointer, long *) = (long)v; break;
          case LL: *va_arg(pointer, long long *) = v; break;
          case J:  *va_arg(pointer, intmax_t *) = v; break;
          case Z:  *va_arg(pointer, size_t *) = (size_t)v; break;
          case T:  *va_arg(pointer, ptrdiff_t *) = (ptrdiff_t)v; break;
          default: *va_arg(pointer, int *) = (int)v; break;
      }
      
      if (c != EOF) ungetc(c, stdin);
      counted++;
      break;
    }
    case 'f':{
      float *n = va_arg(pointer, float *);
      float v = 0.0f;
      int neg = 0;
      int after_dot = 0;
      float div = 1.0f;

      // skip leading whitespace
      do { c = getchar(); } while (isspace(c));

      // optional sign
      if (c == '-') { neg = 1; c = getchar(); }
      else if (c == '+') { c = getchar(); }

      // at least one digit or a dot
      if (!isdigit(c) && c != '.') {
          // invalid input
          return counted;
      }

      // parse digits
      while (isdigit(c) || c == '.') {
          if (c == '.') {
              after_dot = 1;
              c = getchar();
              continue;
          }

          if (!after_dot) v = v * 10.0f + (c - '0');
          else {
              div *= 10.0f;
              v += (c - '0') / div;
          }

          c = getchar();
      }

      *n = neg ? -v : v;
    

      switch (mod) {
          case NONE: *va_arg(pointer, float *) = (float)v; break;
          case L:    *va_arg(pointer, double *) = (double)v; break;
          case CAPITAL_L: *va_arg(pointer, long double *) = v; break;
          default: *va_arg(pointer, float *) = (float)v; break;
      }
      if (c != EOF) ungetc(c, stdin);
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
