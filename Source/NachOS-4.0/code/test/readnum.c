#include "syscall.h"

int
main()
{
  int n;
  PrintString("Enter number: ");
  n = ReadNum();
  PrintNum(n);
  PrintChar('\n');
  PrintChar('\n');

  Halt();
  /* not reached */
}