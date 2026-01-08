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
#include "scanf_mine_3.c"
#include <assert.h>

 

int test_char(void);
int test_hex(void);
int test_string(void);
int test_decimal(void);
int test_float(void);
int test_combo(void);
int test_dec_mod(void);
int test_vector(void);
int test_percent(void);
int test_hex_mod(void);
int test_float_mod(void);
int test_hex_fail(void);
int test_combo(void);
int test_width_numbers(void);
int test_width_hex(void);
int test_width_vector_string(void);
int test_width_time(void);
int test_width_mix(void);
int test_combo_modifiers(void);
int test_combo_exponent_hex(void);
int test_combo_modifiers(void);
int test_combo_reordered(void);
int test_combo_edge_cases(void);
void test_star_modifier(void);



int main(void){
    //return test_all();
    //test_hex_mod();
    //test_float_mod();
    //test_hex_fail();  //fail
    //test_dec_mod();
    //test_star_modifier();
    printf("==== Running scanfMine Tests ====\n\n");

    int total = 0;
    //test_char();
    //test_hex();
    //test_string();
    //test_decimal();
    //test_float();
    //test_percent();
    //test_vector();
    //test_combo();
    //test_width_numbers();  // :passed
    //test_width_hex();   // :passed
    //test_width_vector_string(); //  :passed
    test_width_time();    //:passed
    test_combo_modifiers();  //:passed
    test_combo_reordered();  // :passed
    test_combo_edge_cases();   // :passed
    //printf("Everything passed!\n");
    return 0;

    printf("\n==== Tests Completed ====\n");
    printf("Total assigned fields across all tests: %d\n", total);

    return 0;
    
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
    ASSERT(n == 1, "decimal return count");
    ASSERT(d == 67, "decimal value");
    printf("PASS: decimal\n");

    return n;
}
int test_float(void) {
    printf("print 3.14\n");
    float f;
    int n = scanfMine("%f", &f);

    ASSERT(n == 1, "float return count");
    ASSERT(fabs(f - 3.14f) < 0.0001f, "float value");
    printf("PASS: float\n");

    return n;
}

int test_percent(void) {
    printf("print 75%%\n");
    float f;
    int n = scanfMine("%k", &f);
    ASSERT(n == 1, "probability return count");
    ASSERT(fabs(f - 0.75f) < 0.0001f, "probability value");
    printf("PASS: probability\n");

    return n;
}

int test_vector(void) {
    printf("print [1.0, 2.0, 3.0]\n");
    float arr[3];
    int n = scanfMine("%v", arr, 3);
    ASSERT(n == 1, "vector return count");
    ASSERT(fabs(arr[0] - 1.0f) < 0.0001f, "vector element 0");
    ASSERT(fabs(arr[1] - 2.0f) < 0.0001f, "vector element 1");
    ASSERT(fabs(arr[2] - 3.0f) < 0.0001f, "vector element 2");
    printf("PASS: vector\n");

    return n;
}

int test_dec_mod(void) {
  printf("Testing %%d with modifiers...\n");
  signed char sc;
  short sh;
  int i;
  long l;
  long long ll;
  int n;
  printf("print 127\n");
  n = scanfMine("%hhd", &sc); // input: "127"
  ASSERT(n == 1 && sc == 127, "signed char %hhd");
  printf("print -32000\n");
  n = scanfMine("%hd", &sh); // input: "-32000"
  ASSERT(n == 1 && sh == -32000, "short %hd");
  printf("print 12345\n");
  n = scanfMine("%d", &i); // input: "12345"
  ASSERT(n == 1 && i == 12345, "int %d");
  printf("print 123456789\n");
  n = scanfMine("%ld", &l); // input: "123456789"
  ASSERT(n == 1 && l == 123456789, "long %ld");
  printf("print -9876543210\n");
  n = scanfMine("%lld", &ll); // input: "-9876543210"
  ASSERT(n == 1 && ll == -9876543210LL, "long long %lld");
  printf("PASS: %%d modifiers\n");
  return 1;
}


//why suddenly problems it worked yesterday???
int test_combo(void) {
    printf("print a 5B 6.8 67 hello 67%% [1.0, 2.5, 3.9] 5767s\n");
    char c;
    unsigned int x;
    float f;
    int d;
    char s[100];
    float prob;
    float arr[3];
    struct hms time;

    int n = scanfMine(" %c %x %f %d %s %k %v %t", &c, &x, &f, &d, s, &prob, arr, 3, &time);

    ASSERT(n == 8, "combo return count"); //how much should this be?
    ASSERT(c == 'a', "combo char");
    ASSERT(x == 0x5B, "combo hex");
    ASSERT(fabs(f - 6.8f) < 0.0001f, "combo float");
    ASSERT(d == 67, "combo decimal");
    ASSERT(strcmp(s, "hello") == 0, "combo string");
    ASSERT(fabs(prob - 0.67f) < 0.0001f, "combo probability");
    ASSERT(fabs(arr[0] - 1.0f) < 0.0001f, "combo vector element 0");
    ASSERT(fabs(arr[1] - 2.5f) < 0.0001f, "combo vector element 1");
    ASSERT(fabs(arr[2] - 3.9f) < 0.0001f, "combo vector element 2");
    ASSERT(time.h == 1, "combo time hours");
    ASSERT(time.m == 36, "combo time minutes");
    ASSERT(time.s == 7, "combo time seconds");

    printf("PASS: combo\n");  

    return n;
}
int test_hex_mod(void) {
  printf("Testing %%x with modifiers...\n");

  unsigned char uc;
  unsigned short us;
  unsigned int ui;
  unsigned long ul;
  unsigned long long ull;
  int n;

  printf("print ff\n");
  n = scanfMine("%hhx", &uc);   // input: "ff"
  ASSERT(n == 1 && uc == 0xFF, "unsigned char %hhx");

  printf("print ffff\n");
  n = scanfMine("%hx", &us);    // input: "ffff"
  ASSERT(n == 1 && us == 0xFFFF, "unsigned short %hx");

  printf("print 1a2b3c\n");
  n = scanfMine("%x", &ui);     // input: "0x1a2b3c"
  ASSERT(n == 1 && ui == 0x1A2B3C, "unsigned int %x");

  printf("print deadbeef\n");
  n = scanfMine("%lx", &ul);    // input: "deadbeef"
  ASSERT(n == 1 && ul == 0xDEADBEEF, "unsigned long %lx");

  printf("print 123456789abcdef\n");
  n = scanfMine("%llx", &ull);  // input: "0x123456789abcdef"
  ASSERT(n == 1 && ull == 0x123456789ABCDEFULL, "unsigned long long %llx");

  printf("PASS: %%x modifiers\n");
  return 1;
}

int test_float_mod(void) {
  printf("Testing %%f with modifiers...\n");

  float f;
  double d;
  long double ld;
  int n;

  printf("print 1.25\n");
  n = scanfMine("%f", &f);      // input: "1.25"
  ASSERT(n == 1 && fabsf(f - 1.25f) < 1e-6, "float %f");

  printf("print -2.5e2\n");
  n = scanfMine("%lf", &d);     // input: "-2.5e2"
  ASSERT(n == 1 && fabs(d + 250.0) < 1e-9, "double %lf");

  printf("print 3.1415926535\n");
  n = scanfMine("%Lf", &ld);    // input: "3.1415926535"
  ASSERT(n == 1 && fabsl(ld - 3.1415926535L) < 1e-12, "long double %Lf");

  printf("PASS: %%f modifiers\n");
  return 1;
}


//need to fix n should be -1 and no assignment on fail i think
int test_hex_fail(void) {
  unsigned int x = 123;
  int n;
  printf("print zzz\n");
  n = scanfMine("%x", &x);
  ASSERT(n == 0 && x == 0x7b, "hex failure does not assign");
  return 1;
}

int test_combo_basic(void) {
    printf("Test 1: Basic combo\n");
    char c;
    unsigned int x;
    float f;
    int d;
    char s[100];
    float prob;
    float arr[3];
    struct hms time;

    printf("print a 5B 6.8 67 hello 67%% [1.0, 2.5, 3.9] 5767s\n");

    int n = scanfMine(" %c %x %f %d %s %k %v %t",
                      &c, &x, &f, &d, s, &prob, arr, 3, &time);

    ASSERT(n == 8, "combo return count");
    ASSERT(c == 'a', "combo char");
    ASSERT(x == 0x5B, "combo hex");
    ASSERT(fabs(f - 6.8f) < 0.0001f, "combo float");
    ASSERT(d == 67, "combo decimal");
    ASSERT(strcmp(s, "hello") == 0, "combo string");
    ASSERT(fabs(prob - 0.67f) < 0.0001f, "combo probability");
    ASSERT(fabs(arr[0] - 1.0f) < 0.0001f, "combo vector 0");
    ASSERT(fabs(arr[1] - 2.5f) < 0.0001f, "combo vector 1");
    ASSERT(fabs(arr[2] - 3.9f) < 0.0001f, "combo vector 2");
    ASSERT(time.h == 1, "combo time hours");
    ASSERT(time.m == 36, "combo time minutes");
    ASSERT(time.s == 7, "combo time seconds");

    printf("PASS: combo_basic\n");
    return n;
}



/* Test 3: Mix modifiers, h, l, ll, L */
int test_combo_modifiers(void) {
    printf("Test 3: Modifiers\n");
    short sh;
    long l;
    long long ll;
    long double ld;
    char s[20];
    struct hms t;

    printf("print 1234 12345 1234567890 1.23e3 test 7200s\n");

    int n = scanfMine("%hd %ld %lld %Lf %s %t",
                      &sh, &l, &ll, &ld, s, &t);

    ASSERT(n == 6, "combo modifiers count");
    ASSERT(sh == 1234, "combo short");
    ASSERT(l == 12345L, "combo long");
    ASSERT(ll == 1234567890LL, "combo long long");
    ASSERT(fabs(ld - 1230.0L) < 0.01L, "combo long double");
    ASSERT(strcmp(s, "test") == 0, "combo string modifier");
    ASSERT(t.h == 2, "combo time h");
    ASSERT(t.m == 0, "combo time m");
    ASSERT(t.s == 0, "combo time s");

    printf("PASS: combo_modifiers\n");
    return n;
}

/* Test 4: Reordered fields */
int test_combo_reordered(void) {
    printf("Test 4: Reordered fields\n");
    float v[2];
    unsigned int x;
    char s[10];
    float p;
    struct hms t;

    printf("print [7.0,8.5] 0xFF hello 25%% 3600s\n");

    int n = scanfMine("%v %x %s %k %t", v, 2, &x, s, &p, &t);

    ASSERT(n == 5, "combo reordered count");
    ASSERT(fabs(v[0]-7.0f)<0.01f, "vector v0 reordered");
    ASSERT(fabs(v[1]-8.5f)<0.01f, "vector v1 reordered");
    ASSERT(x == 0xFF, "hex reordered");
    ASSERT(strcmp(s, "hello")==0, "string reordered");
    ASSERT(fabs(p-0.25f)<0.001f, "prob reordered");
    ASSERT(t.h == 1 && t.m == 0 && t.s == 0, "time reordered");

    printf("PASS: combo_reordered\n");
    return n;
}

/* Test 5: Single char, float in scientific, percent, vector length 1 */
int test_combo_edge_cases(void) {
    printf("Test 5: Edge cases\n");
    char c;
    float f;
    float arr[1];
    float prob;

    printf("print z 9.81e-1 99%% [2.5] 60s\n");

    struct hms t;
    int n = scanfMine("%c %f %k %v %t", &c, &f, &prob, arr, 1, &t);
    ASSERT(n == 5, "combo edge count");
    ASSERT(c=='z', "combo char edge");
    ASSERT(fabs(f-0.981f)<0.001f, "combo float edge");
    ASSERT(fabs(prob-0.99f)<0.001f, "combo probability edge");
    ASSERT(fabs(arr[0]-2.5f)<0.001f, "combo vector edge");
    ASSERT(t.h==0 && t.m==1 && t.s==0, "combo time edge");

    printf("PASS: combo_edge_cases\n");
    return n;
}
/* Test 6: Width-limited integers and floats */
int test_width_numbers(void) {
    printf("Test 6: Width-limited numbers\n");
    char c;
    int d;
    float f;
    char s[10];

    printf("print x123 45678 hello\n");

    // %3d: read only 3 digits for d, %5f: read up to 5 chars for float
    int n = scanfMine("%c%3d %5f %2s", &c, &d, &f, s);
    printf("n=%d\n", n);
    printf("c=%c\n", c);
    printf("d=%d\n", d);
    printf("f=%f\n", f);
    printf("s=%s\n", s);

    // x = 'x', d = 123 (width-limited), f = 45678.0 (5 chars), s = "he"
    ASSERT(n == 4, "width numbers count");
    ASSERT(c == 'x', "width char");
    ASSERT(d == 123, "width int");
    ASSERT(fabs(f - 45678.0f) < 0.01f, "width float");
    ASSERT(strcmp(s, "he") == 0, "width string");
    printf("PASS: width_numbers\n");
    return n;
}

/* Test 7: Width-limited hex */
int test_width_hex(void) {
    printf("Test 7: Width-limited hex\n");
    unsigned int x;

    printf("print 0x123ABCD\n");

    int n = scanfMine("%4x", &x);  // width = 4 hex digits only

    // Only '12' should be read, ignoring 'ABCD'
    ASSERT(n == 1, "width hex count");
    ASSERT(x == 0x12, "width hex value");

    printf("PASS: width_hex\n");
    return n;
}

/* Test 8: Width-limited vector and string */
int test_width_vector_string(void) {
    printf("Test 8: Width-limited vector & string\n");
    float arr[3];
    char s[5];

    printf("print [1.23456,7.89,3.21] helloworld\n");

    int n = scanfMine("%v %3s", arr, 3, s);
    printf("n=%d\n", n);
    printf("arr[0]=%f, arr[1]=%f, arr[2]=%f\n", arr[0], arr[1], arr[2]);
    printf("s=%s\n", s);

    // Vector: read full float values (width=-1), string: only 3 chars
    ASSERT(n == 2, "width vector-string count");
    ASSERT(fabs(arr[0]-1.23456f) < 0.001f, "vector 0 width");
    ASSERT(fabs(arr[1]-7.89f) < 0.001f, "vector 1 width");
    ASSERT(fabs(arr[2]-3.21f) < 0.001f, "vector 2 width");
    ASSERT(strcmp(s, "hel") == 0, "string width");

    printf("PASS: width_vector_string\n");
    return n;
}

/* Test 9: Width-limited time */
int test_width_time(void) {
    printf("Test 9: Width-limited time\n");
    struct hms t;

    printf("print 0072s\n");

    int n = scanfMine("%3t", &t); // width = 3 digits max for seconds

    ASSERT(n == 1, "width time count");
    ASSERT(t.h == 0 && t.m == 1 && t.s == 12, "time width conversion");

    printf("PASS: width_time\n");
    return n;
}







void test_star_modifier(void) {
    printf("Running %%* tests...\n");

    // ---------- Test 1: Skip integer ----------
    {
        int a = 0, b = 0;
        printf("print 10 20\n");
        int n = scanfMine("%*d %d", &b);
        assert(n == 1);
        assert(b == 20);
    }

    // ---------- Test 2: Skip hex ----------
    {
        unsigned x = 0, y = 0;
        printf("print 0xFF 123\n");
        int n = scanfMine("%*x %d", &y);
        printf("n=%d, y=%u\n", n, y);
        assert(n == 1);
        assert(y == 123);
    }

    // ---------- Test 3: Skip float ----------
    {
        float f = 0.0f;
        printf("print 1.23 4.56\n");
        int n = scanfMine("%*f %f", &f);
        assert(n == 1);
        assert(fabs(f - 4.56f) < 0.001f);
    }
  }
    