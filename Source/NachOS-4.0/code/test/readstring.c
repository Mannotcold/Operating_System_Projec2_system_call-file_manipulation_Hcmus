#include "syscall.h"

int
main()
{
  int n;
  char* s;

  PrintString("Enter length of string: ");
  n = ReadNum();

  PrintString("Enter string: ");
  ReadString(s, n);
  PrintString(s);
  PrintChar('\n');
  PrintChar('\n');

  Halt();
  /* not reached */
}