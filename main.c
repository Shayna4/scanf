#include <stdio.h>

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

int scanf(const char *format,...)
{
  //
  va_list pointers;
  va_start(pointers, format);
  
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

    switch(*format){
    case 'c':{
    }
    case 's':{
    }
    case 'x':{
    }
    case 'd':{
    }
    case 'f':{
    }
    case '1':{
    }
    case '2':{
    }
    case '3':{
    }
      
  }

  va_end(pointers);

  }
  return 0;
}
