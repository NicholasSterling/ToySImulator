/*
 * goodies.h -- some types and defines I like to use.
 */

// Slick NULL; use it only where 0 means you're not supplying a parameter.
#define _ 0

// Boolean flags.
typedef unsigned char Bool;
#define TRUE  1
#define FALSE 0

// Short names for unsigned types.
typedef unsigned char  Uchar;
typedef unsigned short Ushort;
typedef unsigned int   Uint;
typedef unsigned long  Ulong;

// Some convenient sizes.
#define K *1024
#define M *(1024*1024)

// Global declarators.
// Use these in a header file to declare globals.  Include the header file
// wherever you want, with GLOBALS defined in just one of the places, where
// you want the storage allocated.
#ifdef GLOBALS
#  define GLOBAL(type_var) type_var
#  define GLOBAL_INIT(type_var,value) type_var = value
#else
#  define GLOBAL(type_var) extern type_var
#  define GLOBAL_INIT(type_var,value) extern type_var
#endif

int find_ms1b(Ulong val);
void err(  const char* fmt, ...);
void fatal(const char* fmt, ...);
