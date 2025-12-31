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

//#include "scanfMine.h"


int scanfMine(const char *format,...);
int test_char(void);
int test_hex(void);
int test_string(void);
int test_decimal(void);
int test_float(void);
int test_combo(void);


int main(void){
    //test_char();
    //test_hex();
    //test_string();
    //test_decimal();
    //test_float();
    test_combo();
    return 0;
}



/*
int main(void)
{
  char c;
  unsigned int x;
  char str[100];
  char str2[100];
  float f;
  float f1;
  int d;
  int d1;
  printf("print char, hex, string, float, decimal:\n");
  int n = scanfMine("%c %x %f %d %s",&c,&x, &f, &d,str);
  //printf("f");
  //scanfMine("%f",&f1);
  //printf("d");
  //scanfMine("%d",&d1);
  printf("number = %d\n",n);
  printf("your char is %c\n",c);
  printf("hex: %x\n",x);
  printf("str: %s\n",str);
  printf("float: %f\n",f);
  printf("decimal: %d",d);
  printf("print string:\n");
  //int j = scanfMine("%s",str2);
  //printf("number = %d\n",j);
  //printf("str2: %s\n",str2);
  //printf("decimal %d\n", d1);
  //printf("fact %f\n",f1);
  return 0;
}
*/

int scanfMine(const char *format,...)
{
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
    /*
     // Check for modifier
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
    */

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
      unsigned int val = 0;
      while (isxdigit(c)){
	  val *= 16;
	  if (isdigit(c)) val += c - '0';
	  else val += toupper(c) - 'A'+10;
	  c = getchar();
      }
      if (!isxdigit(c) && c != EOF) ungetc(c, stdin);
      *n = val;
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

      /*
      ///after this includes modifiers
      intmax_t val = 0;
      int pos = 1;
      while (isspace(c = getchar()));

      if(c == '-') {pos = -1; c = getchar();}
      else if (c == '+') c = getchar();
      while (isdigit(c)){
	  val = val*10 + (c - '0');
	  c = getchar();
      }
      
      if (!isdigit(c) && c != EOF) ungetc(c, stdin);
      val *= pos;

      switch (mod) {
      case HH:  *va_arg(pointer, signed char *) = (signed char)val; break;
      case H:   *va_arg(pointer, short *) = (short)val; break;
      case L:   *va_arg(pointer, long *) = (long)val; break;
      case LL:  *va_arg(pointer, long long *) = (long long)val; break;
      case J:   *va_arg(pointer, intmax_t *) = val; break;
      case Z:   *va_arg(pointer, size_t *) = (size_t)val; break;
      case T:   *va_arg(pointer, ptrdiff_t *) = (ptrdiff_t)val; break;
      default:  *va_arg(pointer, int *) = (int)val; break;
      }
      */
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
      

      /*
      //after this includes modifiers
      long double val = 0.0;
      int pos = 1;
      long double fract = 1;
     
      while (isspace(c = getchar()));
      if(c == '-'){pos = -1; c= getchar();}
      else if (c == '+') c = getchar();

      while (isdigit(c)){
	val = val*10 + (c - '0');
	c = getchar();
      }
      if (c == '.'){
	c = getchar();
	while (isdigit(c)){
	  val = val*10 + (c - '0');
	  fract *= 10;
	  c = getchar();
	}
      }
      val = pos*val/fract;
      switch (mod) {
      case L:   *va_arg(pointer, double *) = (double)val; break;
      case CAPITAL_L: *va_arg(pointer, long double *) = val; break;
      default:  *va_arg(pointer, float *) = (float)val; break;
      }
      */
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

      // check if percentage
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




#define ASSERT(cond,msg) \
    if (!(cond)) { printf("FAIL: %s\n", msg); return 1; }


int test_char(void) {
    printf("print A\n");
    char c;
    int n = scanfMine(" %c", &c);
    ASSERT(n == 1, "char return count");
    ASSERT(c == 'A', "char");
    printf("PASS: char\n");
    return n;
}
int test_hex(void) {
    printf("print 5A\n");
    unsigned int x;
    int n = scanfMine("%x", &x);

    ASSERT(n == 1, "hex return count");
    ASSERT(x == 0x5A, "hex value");
    printf("PASS: hex\n");

    return n;
}
int test_string(void) {
    printf("print Hello\n");
    char str[100];
    int n = scanfMine("%s", str);

    ASSERT(n == 1, "string return count");
    ASSERT(strcmp(str, "Hello") == 0, "string value");
    printf("PASS: string\n");

    return n;
}
int test_decimal(void) {
    printf("print 67\n");
    int d = 67;
    int n = scanfMine("%d", &d);
    printf("n=%d\n", n);
    printf("d=%d\n", d);

    ASSERT(n == 1, "decimal return count");
    ASSERT(d == 67, "decimal value");
    printf("PASS: decimal\n");

    return n;
}
int test_float(void) {
    printf("print 3.14\n");
    float f;
    int n = scanfMine("%f", &f);
    printf("n=%d\n", n);
    printf("f=%f\n", f);

    ASSERT(n == 1, "float return count");
    ASSERT(fabs(f - 3.14f) < 0.0001f, "float value");
    printf("PASS: float\n");

    return n;
}

int test_prob(void) {
    printf("print 75%%\n");
    float f;
    int n = scanfMine("%p", &f);
    printf("n=%d\n", n);
    printf("f=%f\n", f);

    ASSERT(n == 1, "probability return count");
    ASSERT(fabs(f - 0.75f) < 0.0001f, "probability value");
    printf("PASS: probability\n");

    return n;
}

int test_vector(void) {
    printf("print 1.0 2.0 3.0\n");
    float arr[3];
    int n = scanfMine("%v", arr, 3);
    printf("n=%d\n", n);
    printf("arr[0]=%f\n", arr[0]);
    printf("arr[1]=%f\n", arr[1]);
    printf("arr[2]=%f\n", arr[2]);

    ASSERT(n == 1, "vector return count");
    ASSERT(fabs(arr[0] - 1.0f) < 0.0001f, "vector element 0");
    ASSERT(fabs(arr[1] - 2.0f) < 0.0001f, "vector element 1");
    ASSERT(fabs(arr[2] - 3.0f) < 0.0001f, "vector element 2");
    printf("PASS: vector\n");

    return n;
}

int test_combo(void) {
    printf("print a 5B 6.8 67 hello 67%% 1.0 2.5 3.9\n");
    char c;
    unsigned int x;
    float f;
    int d;
    char s[100];
    float prob;
    float arr[3];

    int n = scanfMine(" %c %x %f %d %s %p %v", &c, &x, &f, &d, s, &prob, arr, 3);
    printf("n=%d\n", n);
    printf("c=%c\n", c);
    printf("x=%x\n", x);
    printf("f=%f\n", f);
    printf("d=%d\n", d);
    printf("s=%s\n", s);  
    printf("prob=%f\n", prob);
    printf("arr[0]=%f\n", arr[0]);
    printf("arr[1]=%f\n", arr[1]);
    printf("arr[2]=%f\n", arr[2]);

    ASSERT(n == 7, "combo return count");
    ASSERT(c == 'a', "combo char");
    ASSERT(x == 0x5B, "combo hex");
    ASSERT(fabs(f - 6.8f) < 0.0001f, "combo float");
    ASSERT(d == 67, "combo decimal");
    ASSERT(strcmp(s, "hello") == 0, "combo string");
    ASSERT(fabs(prob - 0.67f) < 0.0001f, "combo probability");
    ASSERT(fabs(arr[0] - 1.0f) < 0.0001f, "combo vector element 0");
    ASSERT(fabs(arr[1] - 2.5f) < 0.0001f, "combo vector element 1");
    ASSERT(fabs(arr[2] - 3.0f) < 0.0001f, "combo vector element 2");
    printf("PASS: combo\n");  

    return n;
}

