#include <stdio.h>
#include <stdarg.h>
#include <ctype.h> 
#include <stdint.h>
#include <stddef.h>





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
  return 0;
}

int scanfMine(const char *format,...)
{
  //
  va_list pointers;
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
    
    switch(*format){
    case 'c':{
      char *w = va_args(pointer, char *);
      c = getchar();
      counted ++;
      break;
    }
    case 's':{
      char *w = va_args(pointer, char*);
      while (isspace(c = getchar())); 
      counted++;
      break;
    }
    case 'x':{
      unsigned int *n = va_args(pointer, int *);
      while (isspace(c = getchar()));
      unsigned int val = 0;
      while (isxdigit(c){
	  val *= 16;
	  if (isdigit(c)) val += c - '0';
	  else val += tolower(c) - 'a'+10;
	  c = getchar()
	}
      counted++;
      break;
    }
    case 'd':{
      //long
      while (isspace(c = getchar()));
      c = getchar();
      counted++;
      break;
    }
    case 'f':{
      float *n = va_args(pointer, float *n);
      while (isspace(c = getchar()));
      c = getchar();
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
  va_end(pointers);

  }
  return 0;
}
