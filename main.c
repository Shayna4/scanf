#include <stdio.h>
#include <stdarg.h>
#include <ctype.h> 
#include <stdint.h>
#include <stddef.h>
#include "scanfMine.h"

int readFloat();
int readInt();
int readHex();
int readChar();
int readString();
int readOne();
int readTwo();
int readThree();

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
  int j = scanfMine("%s",str2);
  printf("number = %d\n",j);
  printf("str2: %s\n",str2);
  //printf("decimal %d\n", d1);
  //printf("fact %f\n",f1);
  return 0;
}

int scanfMine(const char *format,...)
{
  va_list pointer;
  va_start(pointer, format);
  
  //two
  int c;
  int counted = 0;
  
  while (*format != '\0'){
    if (*format != '%'){
      c = getchar();
      if (c != *format){
	break;
      }
      format++;
      continue;
    }
    format++;
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


    //skip leading whitespace for all
    switch(*format){
    case 'c':{
      char *w = va_arg(pointer, char *);
      while(isspace(c= getchar()));
      *w = c;
      counted ++;
      break;
    }
    case 's':{
      char *w = va_arg(pointer, char*);
      while (isspace(c = getchar())); 
      *w++ = c;
      while (((c = getchar()) != EOF) && (!isspace(c))){
	*w++ = c;
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
	  else val += tolower(c) - 'a'+10;
	  c = getchar();
	}
      *n = val;
      counted++;
      break;
    }
      
    case 'd':{
      intmax_t val = 0;
      int pos = 1;
      while (isspace(c = getchar()));

      if(c == '-') {pos = -1; c = getchar();}
      else if (c == '+') c = getchar();
      while (isdigit(c)){
	val = val*10 + (c - '0');
	c = getchar();
      }
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
      
      counted++;
      break;
    }
    case 'f':{
      float *n = va_arg(pointer, float *);
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
      
      counted++;
      break;
    }
    case '1':{
      while (isspace(c = getchar()));
      c = getchar();
      counted++;
      break;
    }
    case '2':{
      while (isspace(c = getchar()));
      c = getchar();
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
