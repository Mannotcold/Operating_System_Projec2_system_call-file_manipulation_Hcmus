/* add.c
 *	Simple program to test whether the systemcall interface works.
 *	
 *	Just do a add syscall that adds two values and returns the result.
 *
 */

#include "syscall.h"



int
main()
{
    PrintNum(RandomNum());
    PrintChar('\n');
    PrintChar('\n');

    Halt();
    /* not reached */
}
