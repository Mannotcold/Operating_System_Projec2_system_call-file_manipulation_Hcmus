#include "syscall.h"

int
main()
{
  char c;
  PrintString("Enter character: ");
  c = ReadChar();
  PrintChar(c);
  PrintChar('\n');
  PrintChar('\n');

  Halt();
  /* not reached */
}